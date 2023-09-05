#include "pch.h"
#include "Config.h"
#include "CyberFsr.h"
#include "Util.h"

NVSDK_NGX_API NVSDK_NGX_Result NVSDK_NGX_D3D11_Init_Ext(unsigned long long InApplicationId, const wchar_t* InApplicationDataPath,
	ID3D11Device* InDevice, const NVSDK_NGX_FeatureCommonInfo* InFeatureInfo, NVSDK_NGX_Version InSDKVersion,
	unsigned long long unknown0)
{
	return NVSDK_NGX_Result_Success;
}

NVSDK_NGX_Result NVSDK_NGX_D3D11_Init(unsigned long long InApplicationId, const wchar_t* InApplicationDataPath, ID3D11Device* InDevice, const NVSDK_NGX_FeatureCommonInfo* InFeatureInfo, NVSDK_NGX_Version InSDKVersion)
{
	return NVSDK_NGX_D3D11_Init_Ext(0x1337, InApplicationDataPath, nullptr, InFeatureInfo, InSDKVersion, 0);
}

NVSDK_NGX_Result NVSDK_NGX_D3D11_Init_ProjectID(const char* InProjectId, NVSDK_NGX_EngineType InEngineType, const char* InEngineVersion, const wchar_t* InApplicationDataPath, ID3D11Device* InDevice, const NVSDK_NGX_FeatureCommonInfo* InFeatureInfo, NVSDK_NGX_Version InSDKVersion)
{
	return NVSDK_NGX_D3D11_Init_Ext(0x1337, InApplicationDataPath, InDevice, InFeatureInfo, InSDKVersion, 0);
}

NVSDK_NGX_Result NVSDK_NGX_D3D11_Init_with_ProjectID(const char* InProjectId, NVSDK_NGX_EngineType InEngineType, const char* InEngineVersion, const wchar_t* InApplicationDataPath, ID3D11Device* InDevice, const NVSDK_NGX_FeatureCommonInfo* InFeatureInfo, NVSDK_NGX_Version InSDKVersion)
{
	return NVSDK_NGX_D3D11_Init_Ext(0x1337, InApplicationDataPath, InDevice, InFeatureInfo, InSDKVersion, 0);
}

NVSDK_NGX_Result NVSDK_CONV NVSDK_NGX_D3D11_Shutdown(void)
{
	CyberFsrContext::instance()->NvParameterInstance->Params.clear();
	CyberFsrContext::instance()->Contexts.clear();
	return NVSDK_NGX_Result_Success;
}

NVSDK_NGX_Result NVSDK_CONV NVSDK_NGX_D3D11_Shutdown1(ID3D11Device* InDevice)
{
	return NVSDK_NGX_D3D11_Shutdown();
}

NVSDK_NGX_Result NVSDK_NGX_D3D11_GetParameters(NVSDK_NGX_Parameter** OutParameters)
{
	*OutParameters = CyberFsrContext::instance()->NvParameterInstance->AllocateParameters();
	return NVSDK_NGX_Result_Success;
}

//currently it's kind of hack still needs a proper implementation 
NVSDK_NGX_Result NVSDK_NGX_D3D11_GetCapabilityParameters(NVSDK_NGX_Parameter** OutParameters)
{
	*OutParameters = NvParameter::instance()->AllocateParameters();
	return NVSDK_NGX_Result_Success;
}

//currently it's kind of hack still needs a proper implementation
NVSDK_NGX_Result NVSDK_NGX_D3D11_AllocateParameters(NVSDK_NGX_Parameter** OutParameters)
{
	*OutParameters = NvParameter::instance()->AllocateParameters();
	return NVSDK_NGX_Result_Success;
}

//currently it's kind of hack still needs a proper implementation
NVSDK_NGX_Result NVSDK_NGX_D3D11_DestroyParameters(NVSDK_NGX_Parameter* InParameters)
{
	NvParameter::instance()->DeleteParameters((NvParameter*)InParameters);
	return NVSDK_NGX_Result_Success;
}

NVSDK_NGX_Result NVSDK_NGX_D3D11_GetScratchBufferSize(NVSDK_NGX_Feature InFeatureId,
	const NVSDK_NGX_Parameter* InParameters, size_t* OutSizeInBytes)
{
	*OutSizeInBytes = ffxFsr2GetScratchMemorySizeDX11();
	return NVSDK_NGX_Result_Success;
}

void Fsr2MessageCallback_DX11(FfxFsr2MsgType type, const wchar_t* message)
{
	switch (type) {
	case FFX_FSR2_MESSAGE_TYPE_ERROR:
		printf("[ERROR] %ls\n", message);
		break;
	case FFX_FSR2_MESSAGE_TYPE_WARNING:
		printf("[WARNING] %ls\n", message);
		break;
	}

}

NVSDK_NGX_Result NVSDK_NGX_D3D11_CreateFeature(ID3D11DeviceContext* InDevCtx, NVSDK_NGX_Feature InFeatureID, NVSDK_NGX_Parameter* InParameters, NVSDK_NGX_Handle** OutHandle)
{
	const auto inParams = static_cast<const NvParameter*>(InParameters);

	ID3D11Device* device;
	InDevCtx->GetDevice(&device);

	auto instance = CyberFsrContext::instance();
	auto& config = instance->MyConfig;
	auto deviceContext = CyberFsrContext::instance()->CreateContext();
	deviceContext->ViewMatrix = ViewMatrixHook::Create(*config);
#ifdef DEBUG_FEATURES
	deviceContext->DebugLayer = std::make_unique<DebugOverlay>(device, InCmdList);
#endif

	* OutHandle = &deviceContext->Handle;

	auto initParams = deviceContext->FsrContextDescription;

	const size_t scratchBufferSize = ffxFsr2GetScratchMemorySizeDX11();
	deviceContext->ScratchBuffer = std::vector<unsigned char>(scratchBufferSize);
	auto scratchBuffer = deviceContext->ScratchBuffer.data();

	FfxErrorCode errorCode = ffxFsr2GetInterfaceDX11(&initParams.callbacks, device, scratchBuffer, scratchBufferSize);
	FFX_ASSERT(errorCode == FFX_OK);

	initParams.device = ffxGetDeviceDX11(device);
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

#ifdef _DEBUG
	initParams.flags |= FFX_FSR2_ENABLE_DEBUG_CHECKING;
	initParams.fpMessage = Fsr2MessageCallback_DX11;
#endif // DEBUG

	errorCode = ffxFsr2ContextCreate(&deviceContext->FsrContext, &initParams);
	FFX_ASSERT(errorCode == FFX_OK);

	return NVSDK_NGX_Result_Success;
}

NVSDK_NGX_Result NVSDK_NGX_D3D11_ReleaseFeature(NVSDK_NGX_Handle* InHandle)
{
	auto deviceContext = CyberFsrContext::instance()->Contexts[InHandle->Id].get();
	FfxErrorCode errorCode = ffxFsr2ContextDestroy(&deviceContext->FsrContext);
	FFX_ASSERT(errorCode == FFX_OK);
	CyberFsrContext::instance()->DeleteContext(InHandle);
	return NVSDK_NGX_Result_Success;
}

NVSDK_NGX_Result NVSDK_NGX_D3D11_GetFeatureRequirements(IDXGIAdapter* Adapter, const NVSDK_NGX_FeatureDiscoveryInfo* FeatureDiscoveryInfo, NVSDK_NGX_FeatureRequirement* OutSupported)
{
	*OutSupported = NVSDK_NGX_FeatureRequirement();
	OutSupported->FeatureSupported = NVSDK_NGX_FeatureSupportResult_Supported;
	OutSupported->MinHWArchitecture = 0;
	//Some windows 10 os version
	strcpy_s(OutSupported->MinOSVersion, "10.0.19045.2728");
	return NVSDK_NGX_Result_Success;
}

NVSDK_NGX_Result NVSDK_NGX_D3D11_EvaluateFeature(ID3D11DeviceContext* InDevCtx, const NVSDK_NGX_Handle* InFeatureHandle, const NVSDK_NGX_Parameter* InParameters, PFN_NVSDK_NGX_ProgressCallback InCallback)
{
	ID3D11Device* device;
	InDevCtx->GetDevice(&device);
	auto instance = CyberFsrContext::instance();
	auto& config = instance->MyConfig;
	auto deviceContext = CyberFsrContext::instance()->Contexts[InFeatureHandle->Id].get();

	const auto inParams = static_cast<const NvParameter*>(InParameters);

	auto* fsrContext = &deviceContext->FsrContext;

	FfxFsr2DispatchDescription dispatchParameters = {};
	dispatchParameters.commandList = InDevCtx;
	auto motionVectors = (ID3D11Resource*)inParams->MotionVectors;
	auto color = (ID3D11Resource*)inParams->Color;
	dispatchParameters.color = ffxGetResourceDX11(fsrContext, color, (wchar_t*)L"FSR2_InputColor");
	dispatchParameters.depth = ffxGetResourceDX11(fsrContext, (ID3D11Resource*)inParams->Depth, (wchar_t*)L"FSR2_InputDepth");
	dispatchParameters.motionVectors = ffxGetResourceDX11(fsrContext, motionVectors, (wchar_t*)L"FSR2_InputMotionVectors");
	if (!config->AutoExposure)
		dispatchParameters.exposure = ffxGetResourceDX11(fsrContext, (ID3D11Resource*)inParams->ExposureTexture, (wchar_t*)L"FSR2_InputExposure");

	//Not sure if these two actually work
	if (!config->DisableReactiveMask.value_or(false))
	{
		dispatchParameters.reactive = ffxGetResourceDX11(fsrContext, (ID3D11Resource*)inParams->InputBiasCurrentColorMask, (wchar_t*)L"FSR2_InputReactiveMap");
		dispatchParameters.transparencyAndComposition = ffxGetResourceDX11(fsrContext, (ID3D11Resource*)inParams->TransparencyMask, (wchar_t*)L"FSR2_TransparencyAndCompositionMap");
	}

	dispatchParameters.output = ffxGetResourceDX11(fsrContext, (ID3D11Resource*)inParams->Output, (wchar_t*)L"FSR2_OutputUpscaledColor", FFX_RESOURCE_STATE_UNORDERED_ACCESS);

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

#ifdef DEBUG_FEATURES
	deviceContext->DebugLayer->AddText(L"DLSS2FSR", DirectX::XMFLOAT2(1.0, 1.0));
	deviceContext->DebugLayer->Render(InCmdList);
#endif

	return NVSDK_NGX_Result_Success;
}