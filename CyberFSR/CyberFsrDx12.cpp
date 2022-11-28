#include "pch.h"
#include "Config.h"
#include "CyberFsr.h"
#include "DirectXHooks.h"
#include "Util.h"

NVSDK_NGX_API NVSDK_NGX_Result NVSDK_NGX_D3D12_Init_Ext(unsigned long long InApplicationId, const wchar_t* InApplicationDataPath,
	ID3D12Device* InDevice, const NVSDK_NGX_FeatureCommonInfo* InFeatureInfo, NVSDK_NGX_Version InSDKVersion,
	unsigned long long unknown0)
{
	return NVSDK_NGX_Result_Success;
}

NVSDK_NGX_Result NVSDK_NGX_D3D12_Init(unsigned long long InApplicationId, const wchar_t* InApplicationDataPath, ID3D12Device* InDevice, const NVSDK_NGX_FeatureCommonInfo* InFeatureInfo, NVSDK_NGX_Version InSDKVersion)
{
	return NVSDK_NGX_D3D12_Init_Ext(InApplicationId, InApplicationDataPath, InDevice, InFeatureInfo, InSDKVersion, 0);
}

NVSDK_NGX_API NVSDK_NGX_Result NVSDK_NGX_D3D12_Init_ProjectID(const char* InProjectId, NVSDK_NGX_EngineType InEngineType, const char* InEngineVersion, const wchar_t* InApplicationDataPath, ID3D12Device* InDevice, const NVSDK_NGX_FeatureCommonInfo* InFeatureInfo, NVSDK_NGX_Version InSDKVersion)
{
	return NVSDK_NGX_D3D12_Init_Ext(0x1337, InApplicationDataPath, InDevice, InFeatureInfo, InSDKVersion, 0);
}

NVSDK_NGX_Result NVSDK_NGX_D3D12_Init_with_ProjectID(const char* InProjectId, NVSDK_NGX_EngineType InEngineType, const char* InEngineVersion, const wchar_t* InApplicationDataPath, ID3D12Device* InDevice, const NVSDK_NGX_FeatureCommonInfo* InFeatureInfo, NVSDK_NGX_Version InSDKVersion)
{
	return NVSDK_NGX_D3D12_Init_Ext(0x1337, InApplicationDataPath, InDevice, InFeatureInfo, InSDKVersion, 0);
}

NVSDK_NGX_Result NVSDK_CONV NVSDK_NGX_D3D12_Shutdown(void)
{
	CyberFsrContext::instance()->NvParameterInstance->Params.clear();
	CyberFsrContext::instance()->Contexts.clear();
	return NVSDK_NGX_Result_Success;
}

NVSDK_NGX_Result NVSDK_CONV NVSDK_NGX_D3D12_Shutdown1(ID3D12Device* InDevice)
{
	CyberFsrContext::instance()->NvParameterInstance->Params.clear();
	CyberFsrContext::instance()->Contexts.clear();
	return NVSDK_NGX_Result_Success;
}

//currently it's kind of hack but better than what it was previously -- External Memory Tracking
NVSDK_NGX_Result NVSDK_NGX_D3D12_GetParameters(NVSDK_NGX_Parameter** OutParameters)
{
	*OutParameters = CyberFsrContext::instance()->NvParameterInstance->AllocateParameters();
	return NVSDK_NGX_Result_Success;
}

//currently it's kind of hack still needs a proper implementation 
NVSDK_NGX_Result NVSDK_NGX_D3D12_GetCapabilityParameters(NVSDK_NGX_Parameter** OutParameters)
{
	*OutParameters = NvParameter::instance()->AllocateParameters();
	return NVSDK_NGX_Result_Success;
}

//currently it's kind of hack still needs a proper implementation
NVSDK_NGX_Result NVSDK_NGX_D3D12_AllocateParameters(NVSDK_NGX_Parameter** OutParameters)
{
	*OutParameters = NvParameter::instance()->AllocateParameters();
	return NVSDK_NGX_Result_Success;
}

//currently it's kind of hack still needs a proper implementation
NVSDK_NGX_Result NVSDK_NGX_D3D12_DestroyParameters(NVSDK_NGX_Parameter* InParameters)
{
	NvParameter::instance()->DeleteParameters((NvParameter*)InParameters);
	return NVSDK_NGX_Result_Success;
}

NVSDK_NGX_Result NVSDK_NGX_D3D12_GetScratchBufferSize(NVSDK_NGX_Feature InFeatureId,
	const NVSDK_NGX_Parameter* InParameters, size_t* OutSizeInBytes)
{
	*OutSizeInBytes = ffxFsr2GetScratchMemorySizeDX12();
	return NVSDK_NGX_Result_Success;
}

NVSDK_NGX_Result NVSDK_NGX_D3D12_CreateFeature(ID3D12GraphicsCommandList* InCmdList, NVSDK_NGX_Feature InFeatureID,
	const NVSDK_NGX_Parameter* InParameters, NVSDK_NGX_Handle** OutHandle)
{
	const auto inParams = NvParameter::instance()->Cast((NvParameter*)InParameters); // call to __RTDynamicCast

	ID3D12Device* device;
	InCmdList->GetDevice(IID_PPV_ARGS(&device));

	auto instance = CyberFsrContext::instance();
	auto config = instance->MyConfig;
	auto deviceContext = CyberFsrContext::instance()->CreateContext();
	deviceContext->ViewMatrix = ViewMatrixHook::Create(*config);
#ifdef DEBUG_FEATURES
	deviceContext->DebugLayer = std::make_unique<DebugOverlay>(device, InCmdList);
#endif

	* OutHandle = &deviceContext->Handle;

	auto initParams = deviceContext->FsrContextDescription;

	const size_t scratchBufferSize = ffxFsr2GetScratchMemorySizeDX12();
	deviceContext->ScratchBuffer = std::vector<unsigned char>(scratchBufferSize);
	auto scratchBuffer = deviceContext->ScratchBuffer.data();

	FfxErrorCode errorCode = ffxFsr2GetInterfaceDX12(&initParams.callbacks, device, scratchBuffer, scratchBufferSize);
	FFX_ASSERT(errorCode == FFX_OK);

	initParams.device = ffxGetDeviceDX12(device);
	initParams.maxRenderSize.width = inParams->Width;
	initParams.maxRenderSize.height = inParams->Height;
	initParams.displaySize.width = inParams->OutWidth;
	initParams.displaySize.height = inParams->OutHeight;

	initParams.flags = 0;
	if (config->DepthInverted.value_or(inParams->DepthInverted))
	{
		initParams.flags |= FFX_FSR2_ENABLE_DEPTH_INVERTED;
	}
	if (config->AutoExposure.value_or(inParams->AutoExposure))
	{
		initParams.flags |= FFX_FSR2_ENABLE_AUTO_EXPOSURE;
	}
	if (config->HDR.value_or(inParams->Hdr))
	{
		initParams.flags |= FFX_FSR2_ENABLE_HIGH_DYNAMIC_RANGE;
	}
	if (config->JitterCancellation.value_or(inParams->JitterMotion))
	{
		initParams.flags |= FFX_FSR2_ENABLE_MOTION_VECTORS_JITTER_CANCELLATION;
	}
	if (config->DisplayResolution.value_or(!inParams->LowRes))
	{
		initParams.flags |= FFX_FSR2_ENABLE_DISPLAY_RESOLUTION_MOTION_VECTORS;
	}
	if (config->InfiniteFarPlane.value_or(false))
	{
		initParams.flags |= FFX_FSR2_ENABLE_DEPTH_INFINITE;
	}

	errorCode = ffxFsr2ContextCreate(&deviceContext->FsrContext, &initParams);
	FFX_ASSERT(errorCode == FFX_OK);

	HookSetComputeRootSignature(InCmdList);

	return NVSDK_NGX_Result_Success;
}

NVSDK_NGX_Result NVSDK_NGX_D3D12_ReleaseFeature(NVSDK_NGX_Handle* InHandle)
{
	auto deviceContext = CyberFsrContext::instance()->Contexts[InHandle->Id].get();
	FfxErrorCode errorCode = ffxFsr2ContextDestroy(&deviceContext->FsrContext);
	FFX_ASSERT(errorCode == FFX_OK);
	CyberFsrContext::instance()->DeleteContext(InHandle);
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
	auto instance = CyberFsrContext::instance();
	auto config = instance->MyConfig;
	auto deviceContext = CyberFsrContext::instance()->Contexts[InFeatureHandle->Id].get();

	if (orgRootSig)
	{
		const auto inParams = NvParameter::instance()->Cast((NvParameter*)InParameters); // call to __RTDynamicCast 

		auto* fsrContext = &deviceContext->FsrContext;

		FfxFsr2DispatchDescription dispatchParameters = {};
		dispatchParameters.commandList = ffxGetCommandListDX12(InCmdList);
		dispatchParameters.color = ffxGetResourceDX12(fsrContext, (ID3D12Resource*)inParams->Color, (wchar_t*)L"FSR2_InputColor");
		dispatchParameters.depth = ffxGetResourceDX12(fsrContext, (ID3D12Resource*)inParams->Depth, (wchar_t*)L"FSR2_InputDepth");
		dispatchParameters.motionVectors = ffxGetResourceDX12(fsrContext, (ID3D12Resource*)inParams->MotionVectors, (wchar_t*)L"FSR2_InputMotionVectors");
		dispatchParameters.exposure = ffxGetResourceDX12(fsrContext, (ID3D12Resource*)inParams->ExposureTexture, (wchar_t*)L"FSR2_InputExposure");

		//Not sure if these two actually work
		if (!config->DisableReactiveMask.value_or(false))
		{
			dispatchParameters.reactive = ffxGetResourceDX12(fsrContext, (ID3D12Resource*)inParams->InputBiasCurrentColorMask, (wchar_t*)L"FSR2_InputReactiveMap");
			dispatchParameters.transparencyAndComposition = ffxGetResourceDX12(fsrContext, (ID3D12Resource*)inParams->TransparencyMask, (wchar_t*)L"FSR2_TransparencyAndCompositionMap");
		}

		dispatchParameters.output = ffxGetResourceDX12(fsrContext, (ID3D12Resource*)inParams->Output, (wchar_t*)L"FSR2_OutputUpscaledColor", FFX_RESOURCE_STATE_UNORDERED_ACCESS);

		dispatchParameters.jitterOffset.x = inParams->JitterOffsetX;
		dispatchParameters.jitterOffset.y = inParams->JitterOffsetY;

		dispatchParameters.motionVectorScale.x = (float)inParams->MVScaleX;
		dispatchParameters.motionVectorScale.y = (float)inParams->MVScaleY;

		dispatchParameters.reset = inParams->ResetRender;

		float sharpness = Util::ConvertSharpness(inParams->Sharpness, config->SharpnessRange);
		dispatchParameters.enableSharpening = config->EnableSharpening.value_or(inParams->EnableSharpening);
		dispatchParameters.sharpness = config->Sharpness.value_or(sharpness);

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
#ifdef DEBUG_FEATURES
	deviceContext->DebugLayer->AddText(L"DLSS2FSR", DirectX::XMFLOAT2(1.0, 1.0));
	deviceContext->DebugLayer->Render(InCmdList);
#endif

	myCommandList = InCmdList;

	return NVSDK_NGX_Result_Success;
}