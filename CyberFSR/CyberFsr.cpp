#include "pch.h"
#include "Config.h"
#include "CyberFsr.h"
#include "DirectXHooks.h"
#include "Util.h"

NVSDK_NGX_Result NVSDK_NGX_D3D12_Init_Ext(unsigned long long InApplicationId, const wchar_t* InApplicationDataPath,
	ID3D12Device* InDevice, const NVSDK_NGX_FeatureCommonInfo* InFeatureInfo, NVSDK_NGX_Version InSDKVersion,
	unsigned long long unknown0)
{
	return NVSDK_NGX_Result_Success;
}

NVSDK_NGX_Result NVSDK_NGX_D3D12_Init(unsigned long long InApplicationId, const wchar_t* InApplicationDataPath, ID3D12Device* InDevice, const NVSDK_NGX_FeatureCommonInfo* InFeatureInfo, NVSDK_NGX_Version InSDKVersion)
{
	return NVSDK_NGX_D3D12_Init_Ext(InApplicationId, InApplicationDataPath, InDevice, InFeatureInfo, InSDKVersion, 0);
}

NVSDK_NGX_Result NVSDK_CONV NVSDK_NGX_D3D12_Shutdown(void)
{
	CyberFsrContext::instance().Parameters.clear();
	CyberFsrContext::instance().Contexts.clear();
	return NVSDK_NGX_Result_Success;
}

NVSDK_NGX_Result NVSDK_CONV NVSDK_NGX_D3D12_Shutdown1(ID3D12Device* InDevice)
{
	CyberFsrContext::instance().Parameters.clear();
	CyberFsrContext::instance().Contexts.clear();
	return NVSDK_NGX_Result_Success;
}

//Deprecated Parameter Function - Internal Memory Tracking
NVSDK_NGX_Result NVSDK_NGX_D3D12_GetParameters(NVSDK_NGX_Parameter** OutParameters)
{
	*OutParameters = CyberFsrContext::instance().AllocateParameter<Dx12ParameterImpl>();
	return NVSDK_NGX_Result_Success;
}

//TODO External Memory Tracking
NVSDK_NGX_Result NVSDK_NGX_D3D12_GetCapabilityParameters(NVSDK_NGX_Parameter** OutParameters)
{
	*OutParameters = new Dx12ParameterImpl();
	return NVSDK_NGX_Result_Success;
}

//TODO
NVSDK_NGX_Result NVSDK_NGX_D3D12_AllocateParameters(NVSDK_NGX_Parameter** OutParameters)
{
	*OutParameters = new Dx12ParameterImpl();
	return NVSDK_NGX_Result_Success;
}

//TODO
NVSDK_NGX_Result NVSDK_NGX_D3D12_DestroyParameters(NVSDK_NGX_Parameter* InParameters)
{
	delete InParameters;
	return NVSDK_NGX_Result_Success;
}

//TODO
NVSDK_NGX_Result NVSDK_NGX_D3D12_GetScratchBufferSize(NVSDK_NGX_Feature InFeatureId,
	const NVSDK_NGX_Parameter* InParameters, size_t* OutSizeInBytes)
{
	return NVSDK_NGX_Result_Success;
}

NVSDK_NGX_Result NVSDK_NGX_D3D12_CreateFeature(ID3D12GraphicsCommandList* InCmdList, NVSDK_NGX_Feature InFeatureID,
	const NVSDK_NGX_Parameter* InParameters, NVSDK_NGX_Handle** OutHandle)
{
	const auto inParams = dynamic_cast<const Dx12ParameterImpl*>(InParameters);

	ID3D12Device* device;
	InCmdList->GetDevice(IID_PPV_ARGS(&device));
	auto deviceContext = CyberFsrContext::instance().CreateContext();
	deviceContext->Config = std::make_unique<Config>("nvngx.ini");
	deviceContext->ViewMatrix = ViewMatrixHook::Create(*deviceContext->Config);

	*OutHandle = &deviceContext->Handle;

	FfxFsr2ContextDescription initParams = {};
	const size_t scratchBufferSize = ffxFsr2GetScratchMemorySizeDX12();
	void* scratchBuffer = malloc(scratchBufferSize);
	FfxErrorCode errorCode = ffxFsr2GetInterfaceDX12(&initParams.callbacks, device, scratchBuffer, scratchBufferSize);
	FFX_ASSERT(errorCode == FFX_OK);

	initParams.device = ffxGetDeviceDX12(device);
	initParams.maxRenderSize.width = inParams->Width;
	initParams.maxRenderSize.height = inParams->Height;
	initParams.displaySize.width = inParams->OutWidth;
	initParams.displaySize.height = inParams->OutHeight;

	initParams.flags = 0;
	if (deviceContext->Config->DepthInverted.value_or(inParams->DepthInverted))
	{
		initParams.flags |= FFX_FSR2_ENABLE_DEPTH_INVERTED;
	}
	if (deviceContext->Config->AutoExposure.value_or(inParams->AutoExposure))
	{
		initParams.flags |= FFX_FSR2_ENABLE_AUTO_EXPOSURE;
	}
	if (deviceContext->Config->HDR.value_or(inParams->Hdr))
	{
		initParams.flags |= FFX_FSR2_ENABLE_HIGH_DYNAMIC_RANGE;
	}
	if (deviceContext->Config->JitterCancellation.value_or(inParams->JitterMotion))
	{
		initParams.flags |= FFX_FSR2_ENABLE_MOTION_VECTORS_JITTER_CANCELLATION;
	}
	if (deviceContext->Config->DisplayResolution.value_or(!inParams->LowRes))
	{
		initParams.flags |= FFX_FSR2_ENABLE_DISPLAY_RESOLUTION_MOTION_VECTORS;
	}

	deviceContext->FsrContext = std::make_unique<FfxFsr2Context>();

	ffxFsr2ContextCreate(deviceContext->FsrContext.get(), &initParams);

	HookSetComputeRootSignature(InCmdList);

	return NVSDK_NGX_Result_Success;
}

NVSDK_NGX_Result NVSDK_NGX_D3D12_ReleaseFeature(NVSDK_NGX_Handle* InHandle)
{
	auto deviceContext = CyberFsrContext::instance().Contexts[InHandle->Id];
	FfxErrorCode errorCode = ffxFsr2ContextDestroy(deviceContext->FsrContext.get());
	FFX_ASSERT(errorCode == FFX_OK);
	CyberFsrContext::instance().DeleteContext(InHandle);
	return NVSDK_NGX_Result_Success;
}

NVSDK_NGX_Result NVSDK_NGX_D3D12_EvaluateFeature(ID3D12GraphicsCommandList* InCmdList, const NVSDK_NGX_Handle* InFeatureHandle, const NVSDK_NGX_Parameter* InParameters, PFN_NVSDK_NGX_ProgressCallback InCallback)
{
	ID3D12RootSignature* orgRootSig = nullptr;

	rootSigMutex.lock();
	if (commandListVector.contains(InCmdList))
	{
		orgRootSig = commandListVector[InCmdList];
	}
	else
	{
		printf("Cant find the RootSig\n");
	}
	rootSigMutex.unlock();

	ID3D12Device* device;
	InCmdList->GetDevice(IID_PPV_ARGS(&device));
	auto deviceContext = CyberFsrContext::instance().Contexts[InFeatureHandle->Id];

	if (orgRootSig)
	{
		const auto inParams = dynamic_cast<const Dx12ParameterImpl*>(InParameters);

		auto* fsrContext = deviceContext->FsrContext.get();

		FfxFsr2DispatchDescription dispatchParameters = {};
		dispatchParameters.commandList = ffxGetCommandListDX12(InCmdList);
		dispatchParameters.color = ffxGetResourceDX12(fsrContext, inParams->Color, (wchar_t*)L"FSR2_InputColor");
		dispatchParameters.depth = ffxGetResourceDX12(fsrContext, inParams->Depth, (wchar_t*)L"FSR2_InputDepth");
		dispatchParameters.motionVectors = ffxGetResourceDX12(fsrContext, inParams->MotionVectors, (wchar_t*)L"FSR2_InputMotionVectors");
		dispatchParameters.exposure = ffxGetResourceDX12(fsrContext, inParams->ExposureTexture, (wchar_t*)L"FSR2_InputExposure");

		//Not sure if these two actually work
		dispatchParameters.reactive = ffxGetResourceDX12(fsrContext, inParams->InputBiasCurrentColorMask, (wchar_t*)L"FSR2_InputReactiveMap");
		dispatchParameters.transparencyAndComposition = ffxGetResourceDX12(fsrContext, inParams->TransparencyMask, (wchar_t*)L"FSR2_TransparencyAndCompositionMap");

		dispatchParameters.output = ffxGetResourceDX12(fsrContext, inParams->Output, (wchar_t*)L"FSR2_OutputUpscaledColor", FFX_RESOURCE_STATE_UNORDERED_ACCESS);

		dispatchParameters.jitterOffset.x = inParams->JitterOffsetX;
		dispatchParameters.jitterOffset.y = inParams->JitterOffsetY;

		dispatchParameters.motionVectorScale.x = (float)inParams->MVScaleX;
		dispatchParameters.motionVectorScale.y = (float)inParams->MVScaleY;

		dispatchParameters.reset = inParams->ResetRender;

		float sharpness = Util::ConvertSharpness(inParams->Sharpness, deviceContext->Config->SharpnessRange);
		dispatchParameters.enableSharpening = deviceContext->Config->EnableSharpening.value_or(inParams->EnableSharpening);
		dispatchParameters.sharpness = deviceContext->Config->Sharpness.value_or(sharpness);

		//deltatime hax
		static double lastFrameTime;
		double currentTime = Util::MillisecondsNow();
		double deltaTime = (currentTime - lastFrameTime);
		lastFrameTime = currentTime;

		dispatchParameters.frameTimeDelta = (float)deltaTime;
		dispatchParameters.preExposure = 1.0f;
		dispatchParameters.renderSize.width = inParams->Width;
		dispatchParameters.renderSize.height = inParams->Height;

		//Hax Zone
		dispatchParameters.cameraFar = deviceContext->ViewMatrix->GetFarPlane();
		dispatchParameters.cameraNear = deviceContext->ViewMatrix->GetNearPlane();
		dispatchParameters.cameraFovAngleVertical = DirectX::XMConvertToRadians(deviceContext->ViewMatrix->GetFov());
		FfxErrorCode errorCode = ffxFsr2ContextDispatch(fsrContext, &dispatchParameters);
		FFX_ASSERT(errorCode == FFX_OK);

		InCmdList->SetComputeRootSignature(orgRootSig);
	}

	myCommandList = InCmdList;

	return NVSDK_NGX_Result_Success;
}

NVSDK_NGX_Result NVSDK_CONV NVSDK_NGX_DLSS_GetOptimalSettingsCallback(NVSDK_NGX_Parameter* InParams)
{
	auto* params = (Dx12ParameterImpl*)InParams;
	params->EvaluateRenderScale();
	return NVSDK_NGX_Result_Success;
}

void CyberFsrContext::DeleteParameter(NVSDK_NGX_Parameter* parameter)
{
	auto it = std::find(Parameters.begin(), Parameters.end(), parameter);
	Parameters.erase(it);
}

FeatureContext* CyberFsrContext::CreateContext()
{
	auto dCtx = new FeatureContext();
	dCtx->Handle.Id = rand();
	Contexts[dCtx->Handle.Id] = dCtx;

	return dCtx;
}

void CyberFsrContext::DeleteContext(NVSDK_NGX_Handle* handle)
{
	auto handleId = handle->Id;

	auto it = std::find_if(Contexts.begin(), Contexts.end(),
		[&handleId](const auto& p) { return p.first == handleId; });
	Contexts.erase(it);
}
