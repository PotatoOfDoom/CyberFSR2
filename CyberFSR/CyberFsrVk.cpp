#include "pch.h"
#include "Config.h"
#include "CyberFsr.h"
#include "DirectXHooks.h"
#include "Util.h"

NVSDK_NGX_API NVSDK_NGX_Result NVSDK_NGX_VULKAN_Init(unsigned long long InApplicationId, const wchar_t* InApplicationDataPath, VkInstance InInstance, VkPhysicalDevice InPD, VkDevice InDevice, const NVSDK_NGX_FeatureCommonInfo* InFeatureInfo, NVSDK_NGX_Version InSDKVersion)
{
	CyberFsrContext::instance()->VulkanDevice = InDevice;
	CyberFsrContext::instance()->VulkanInstance = InInstance;
	CyberFsrContext::instance()->VulkanPhysicalDevice = InPD;
	return NVSDK_NGX_Result_Success;
}

NVSDK_NGX_API NVSDK_NGX_Result NVSDK_CONV NVSDK_NGX_VULKAN_Shutdown(void)
{
	CyberFsrContext::instance()->VulkanDevice = nullptr;
	CyberFsrContext::instance()->VulkanInstance = nullptr;
	CyberFsrContext::instance()->VulkanPhysicalDevice = nullptr;
	CyberFsrContext::instance()->Parameters.clear();
	CyberFsrContext::instance()->Contexts.clear();
	return NVSDK_NGX_Result_Success;
}

NVSDK_NGX_API NVSDK_NGX_Result NVSDK_CONV NVSDK_NGX_VULKAN_Shutdown1(VkDevice InDevice)
{
	CyberFsrContext::instance()->VulkanDevice = nullptr;
	CyberFsrContext::instance()->VulkanInstance = nullptr;
	CyberFsrContext::instance()->VulkanPhysicalDevice = nullptr;
	CyberFsrContext::instance()->Parameters.clear();
	CyberFsrContext::instance()->Contexts.clear();
	return NVSDK_NGX_Result_Success;
}

NVSDK_NGX_Result NVSDK_NGX_VULKAN_GetParameters(NVSDK_NGX_Parameter** OutParameters)
{
	*OutParameters = CyberFsrContext::instance()->AllocateParameter<NvParameter>();
	return NVSDK_NGX_Result_Success;
}

NVSDK_NGX_API NVSDK_NGX_Result NVSDK_CONV NVSDK_NGX_VULKAN_AllocateParameters(NVSDK_NGX_Parameter** OutParameters)
{
	*OutParameters = new NvParameter();
	return NVSDK_NGX_Result_Success;
}

NVSDK_NGX_API NVSDK_NGX_Result NVSDK_CONV NVSDK_NGX_VULKAN_GetCapabilityParameters(NVSDK_NGX_Parameter** OutParameters)
{
	*OutParameters = new NvParameter();
	return NVSDK_NGX_Result_Success;
}

NVSDK_NGX_API NVSDK_NGX_Result NVSDK_CONV NVSDK_NGX_VULKAN_DestroyParameters(NVSDK_NGX_Parameter* InParameters)
{
	delete InParameters;
	return NVSDK_NGX_Result_Success;
}

NVSDK_NGX_API NVSDK_NGX_Result NVSDK_CONV NVSDK_NGX_VULKAN_GetScratchBufferSize(NVSDK_NGX_Feature InFeatureId, const NVSDK_NGX_Parameter* InParameters, size_t* OutSizeInBytes)
{
	return NVSDK_NGX_Result_Success;
}

NVSDK_NGX_API NVSDK_NGX_Result NVSDK_CONV NVSDK_NGX_VULKAN_CreateFeature(VkCommandBuffer InCmdBuffer, NVSDK_NGX_Feature InFeatureID, const NVSDK_NGX_Parameter* InParameters, NVSDK_NGX_Handle** OutHandle)
{
	return NVSDK_NGX_VULKAN_CreateFeature1(CyberFsrContext::instance()->VulkanDevice, InCmdBuffer, InFeatureID, InParameters, OutHandle);
}

NVSDK_NGX_API NVSDK_NGX_Result NVSDK_CONV NVSDK_NGX_VULKAN_CreateFeature1(VkDevice InDevice, VkCommandBuffer InCmdList, NVSDK_NGX_Feature InFeatureID, const NVSDK_NGX_Parameter* InParameters, NVSDK_NGX_Handle** OutHandle)
{
	const auto inParams = dynamic_cast<const NvParameter*>(InParameters);

	auto instance = CyberFsrContext::instance();
	auto config = instance->MyConfig;
	auto deviceContext = instance->CreateContext();
	deviceContext->ViewMatrix = ViewMatrixHook::Create(*config);

	*OutHandle = &deviceContext->Handle;

	FfxFsr2ContextDescription initParams = {};
	const size_t scratchBufferSize = ffxFsr2GetScratchMemorySizeVK(instance->VulkanPhysicalDevice);
	void* scratchBuffer = malloc(scratchBufferSize);

	FfxErrorCode errorCode = ffxFsr2GetInterfaceVK(&initParams.callbacks, scratchBuffer, scratchBufferSize, instance->VulkanPhysicalDevice, vkGetDeviceProcAddr);
	FFX_ASSERT(errorCode == FFX_OK);

	initParams.device = ffxGetDeviceVK(InDevice);
	initParams.maxRenderSize.width = inParams->Width;
	initParams.maxRenderSize.height = inParams->Height;
	initParams.displaySize.width = inParams->OutWidth;
	initParams.displaySize.height = inParams->OutHeight;
	initParams.flags = (inParams->DepthInverted) ? FFX_FSR2_ENABLE_DEPTH_INVERTED : 0
		| (inParams->AutoExposure) ? FFX_FSR2_ENABLE_AUTO_EXPOSURE : 0
		| (inParams->Hdr) ? FFX_FSR2_ENABLE_HIGH_DYNAMIC_RANGE : 0
		| (inParams->JitterMotion) ? FFX_FSR2_ENABLE_MOTION_VECTORS_JITTER_CANCELLATION : 0
		| (!inParams->LowRes) ? FFX_FSR2_ENABLE_DISPLAY_RESOLUTION_MOTION_VECTORS : 0;

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

	deviceContext->FsrContext = std::make_unique<FfxFsr2Context>();

	ffxFsr2ContextCreate(deviceContext->FsrContext.get(), &initParams);
	return NVSDK_NGX_Result_Success;
}

NVSDK_NGX_API NVSDK_NGX_Result NVSDK_CONV NVSDK_NGX_VULKAN_ReleaseFeature(NVSDK_NGX_Handle* InHandle)
{
	auto deviceContext = CyberFsrContext::instance()->Contexts[InHandle->Id];
	FfxErrorCode errorCode = ffxFsr2ContextDestroy(deviceContext->FsrContext.get());
	FFX_ASSERT(errorCode == FFX_OK);
	CyberFsrContext::instance()->DeleteContext(InHandle);
	return NVSDK_NGX_Result_Success;
}

NVSDK_NGX_API NVSDK_NGX_Result NVSDK_CONV NVSDK_NGX_VULKAN_EvaluateFeature(VkCommandBuffer InCmdList, const NVSDK_NGX_Handle* InFeatureHandle, const NVSDK_NGX_Parameter* InParameters, PFN_NVSDK_NGX_ProgressCallback InCallback)
{
	auto instance = CyberFsrContext::instance();
	auto config = instance->MyConfig;
	auto deviceContext = CyberFsrContext::instance()->Contexts[InFeatureHandle->Id];
	const auto inParams = dynamic_cast<const NvParameter*>(InParameters);

	auto color = (NVSDK_NGX_Resource_VK*)inParams->Color;
	auto depth = (NVSDK_NGX_Resource_VK*)inParams->Depth;
	auto motionVectors = (NVSDK_NGX_Resource_VK*)inParams->MotionVectors;
	auto exposureTexture = (NVSDK_NGX_Resource_VK*)inParams->ExposureTexture;
	auto inputBiasColorMask = (NVSDK_NGX_Resource_VK*)inParams->InputBiasCurrentColorMask;
	auto transparencyMask = (NVSDK_NGX_Resource_VK*)inParams->TransparencyMask;
	auto output = (NVSDK_NGX_Resource_VK*)inParams->Output;

	auto* fsrContext = deviceContext->FsrContext.get();
	FfxFsr2DispatchDescription dispatchParameters = {};
	dispatchParameters.commandList = ffxGetCommandListVK(InCmdList);
	dispatchParameters.color = ffxGetTextureResourceVK(fsrContext, color->Resource.ImageViewInfo.Image, color->Resource.ImageViewInfo.ImageView, color->Resource.ImageViewInfo.Width, color->Resource.ImageViewInfo.Height, color->Resource.ImageViewInfo.Format, (wchar_t*)L"FSR2_InputColor");
	dispatchParameters.depth = ffxGetTextureResourceVK(fsrContext, depth->Resource.ImageViewInfo.Image, depth->Resource.ImageViewInfo.ImageView, depth->Resource.ImageViewInfo.Width, depth->Resource.ImageViewInfo.Height, depth->Resource.ImageViewInfo.Format, (wchar_t*)L"FSR2_InputDepth");
	dispatchParameters.motionVectors = ffxGetTextureResourceVK(fsrContext, motionVectors->Resource.ImageViewInfo.Image, motionVectors->Resource.ImageViewInfo.ImageView, motionVectors->Resource.ImageViewInfo.Width, motionVectors->Resource.ImageViewInfo.Height, motionVectors->Resource.ImageViewInfo.Format, (wchar_t*)L"FSR2_InputMotionVectors");
	dispatchParameters.exposure = ffxGetTextureResourceVK(fsrContext, exposureTexture->Resource.ImageViewInfo.Image, exposureTexture->Resource.ImageViewInfo.ImageView, exposureTexture->Resource.ImageViewInfo.Width, exposureTexture->Resource.ImageViewInfo.Height, exposureTexture->Resource.ImageViewInfo.Format, (wchar_t*)L"FSR2_InputExposure");
	dispatchParameters.reactive = ffxGetTextureResourceVK(fsrContext, inputBiasColorMask->Resource.ImageViewInfo.Image, inputBiasColorMask->Resource.ImageViewInfo.ImageView, inputBiasColorMask->Resource.ImageViewInfo.Width, inputBiasColorMask->Resource.ImageViewInfo.Height, inputBiasColorMask->Resource.ImageViewInfo.Format, (wchar_t*)L"FSR2_InputReactiveMap");
	
	if (transparencyMask == nullptr)
		dispatchParameters.transparencyAndComposition = ffxGetTextureResourceVK(fsrContext, nullptr, nullptr, 1, 1, VK_FORMAT_UNDEFINED, (wchar_t*)L"FSR2_TransparencyAndCompositionMap");
	else
		dispatchParameters.transparencyAndComposition = ffxGetTextureResourceVK(fsrContext, transparencyMask->Resource.ImageViewInfo.Image, transparencyMask->Resource.ImageViewInfo.ImageView, transparencyMask->Resource.ImageViewInfo.Width, transparencyMask->Resource.ImageViewInfo.Height, inputBiasColorMask->Resource.ImageViewInfo.Format, (wchar_t*)L"FSR2_TransparencyAndCompositionMap");
	
	dispatchParameters.output = ffxGetTextureResourceVK(fsrContext, output->Resource.ImageViewInfo.Image, output->Resource.ImageViewInfo.ImageView, output->Resource.ImageViewInfo.Width, output->Resource.ImageViewInfo.Height, output->Resource.ImageViewInfo.Format, (wchar_t*)L"FSR2_OutputUpscaledColor", FFX_RESOURCE_STATE_UNORDERED_ACCESS);

	dispatchParameters.jitterOffset.x = inParams->JitterOffsetX;
	dispatchParameters.jitterOffset.y = inParams->JitterOffsetY;
	dispatchParameters.motionVectorScale.x = (float)inParams->MVScaleX;
	dispatchParameters.motionVectorScale.y = (float)inParams->MVScaleY;

	dispatchParameters.reset = inParams->ResetRender;

	float sharpness = Util::ConvertSharpness(inParams->Sharpness, config->SharpnessRange);
	dispatchParameters.enableSharpening = config->EnableSharpening.value_or(inParams->EnableSharpening);
	dispatchParameters.sharpness = config->Sharpness.value_or(sharpness);

	static double lastFrameTime;
	double currentTime = Util::MillisecondsNow();
	double deltaTime = (currentTime - lastFrameTime);
	lastFrameTime = currentTime;

	dispatchParameters.frameTimeDelta = (float)deltaTime;
	dispatchParameters.preExposure = 1.0f;
	dispatchParameters.renderSize.width = inParams->Width;
	dispatchParameters.renderSize.height = inParams->Height;
	
	dispatchParameters.cameraFar = deviceContext->ViewMatrix->GetFarPlane();
	dispatchParameters.cameraNear = deviceContext->ViewMatrix->GetNearPlane();
	dispatchParameters.cameraFovAngleVertical = DirectX::XMConvertToRadians(deviceContext->ViewMatrix->GetFov());
	FfxErrorCode errorCode = ffxFsr2ContextDispatch(fsrContext, &dispatchParameters);
	FFX_ASSERT(errorCode == FFX_OK);
	return NVSDK_NGX_Result_Success;
}