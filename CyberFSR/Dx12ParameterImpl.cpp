#include "pch.h"
#include "Util.h"
#include "Dx12ParameterImpl.h"

void Dx12ParameterImpl::Set(const char* InName, unsigned long long InValue)
{
	//TODO
}

void Dx12ParameterImpl::Set(const char* InName, float InValue)
{
	switch (Util::NvParameterToEnum(InName))
	{
	case Util::NvParameter::MV_Scale_X:
		MVScaleX = InValue;
		break;
	case Util::NvParameter::MV_Scale_Y:
		MVScaleY = InValue;
		break;
	case Util::NvParameter::Jitter_Offset_X:
		JitterOffsetX = InValue;
		break;
	case Util::NvParameter::Jitter_Offset_Y:
		JitterOffsetY = InValue;
		break;
	case Util::NvParameter::Sharpness:
		Sharpness = InValue;
		break;
	}
}

void Dx12ParameterImpl::Set(const char* InName, double InValue)
{
	//TODO
}

void Dx12ParameterImpl::Set(const char* InName, unsigned int InValue)
{
	Set(InName, static_cast<int>(InValue));
}

void Dx12ParameterImpl::Set(const char* InName, int InValue)
{
	switch (Util::NvParameterToEnum(InName))
	{
	case Util::NvParameter::Width:
		Width = InValue;
		break;
	case Util::NvParameter::Height:
		Height = InValue;
		break;
	case Util::NvParameter::PerfQualityValue:
		PerfQualityValue = static_cast<NVSDK_NGX_PerfQuality_Value>(InValue);
		break;
	case Util::NvParameter::RTXValue:
		RTXValue = InValue;
		break;
	case Util::NvParameter::Reset:
		ResetRender = InValue;
		break;
	case Util::NvParameter::OutWidth:
		OutWidth = InValue;
		break;
	case Util::NvParameter::OutHeight:
		OutHeight = InValue;
		break;
	case Util::NvParameter::DLSS_Feature_Create_Flags:
		Hdr = InValue & NVSDK_NGX_DLSS_Feature_Flags_IsHDR;
		EnableSharpening = InValue & NVSDK_NGX_DLSS_Feature_Flags_DoSharpening;
		DepthInverted = InValue & NVSDK_NGX_DLSS_Feature_Flags_DepthInverted;
		JitterMotion = InValue & NVSDK_NGX_DLSS_Feature_Flags_MVJittered;
		LowRes = InValue & NVSDK_NGX_DLSS_Feature_Flags_MVLowRes;
		//thats all for now
		break;
	}
}

void Dx12ParameterImpl::Set(const char* InName, ID3D11Resource* InValue)
{
	//TODO
}

void Dx12ParameterImpl::Set(const char* InName, ID3D12Resource* InValue)
{
	switch (Util::NvParameterToEnum(InName))
	{
	case Util::NvParameter::DLSS_Input_Bias_Current_Color_Mask:
		InputBiasCurrentColorMask = InValue;
		if (InValue)
			InputBiasCurrentColorMask->SetName(L"InputBiasColorMask");
		break;
	case Util::NvParameter::Color:
		Color = InValue;
		if (InValue)
			Color->SetName(L"Color");
		break;
	case Util::NvParameter::Depth:
		Depth = InValue;
		if (InValue)
			Depth->SetName(L"Depth");
		break;
	case Util::NvParameter::MotionVectors:
		MotionVectors = InValue;
		if (InValue)
			MotionVectors->SetName(L"MotionVectors");
		break;
	case Util::NvParameter::Output:
		Output = InValue;
		if (InValue)
			Output->SetName(L"Output");
		break;
	case Util::NvParameter::TransparencyMask:
		TransparencyMask = InValue;
		if (InValue)
			TransparencyMask->SetName(L"TransparencyMask");
		break;
	case Util::NvParameter::ExposureTexture:
		ExposureTexture = InValue;
		if (InValue)
			ExposureTexture->SetName(L"ExposureTexture");
		break;
	}
}

void Dx12ParameterImpl::Set(const char* InName, void* InValue)
{
	//TODO
}

NVSDK_NGX_Result Dx12ParameterImpl::Get(const char* InName, unsigned long long* OutValue) const
{
	return NVSDK_NGX_Result_Fail;
}

NVSDK_NGX_Result Dx12ParameterImpl::Get(const char* InName, float* OutValue) const
{
	switch (Util::NvParameterToEnum(InName))
	{
	case Util::NvParameter::Sharpness:
		*OutValue = Sharpness;
		break;
	default:
		*OutValue = 0.0f;
		return NVSDK_NGX_Result_FAIL_InvalidParameter;
	}

	return NVSDK_NGX_Result_Success;
}

NVSDK_NGX_Result Dx12ParameterImpl::Get(const char* InName, double* OutValue) const
{
	return NVSDK_NGX_Result_Fail;
}

NVSDK_NGX_Result Dx12ParameterImpl::Get(const char* InName, unsigned int* OutValue) const
{
	return Get(InName, reinterpret_cast<int*>(OutValue));
}

NVSDK_NGX_Result Dx12ParameterImpl::Get(const char* InName, int* OutValue) const
{
	switch (Util::NvParameterToEnum(InName))
	{
	case Util::NvParameter::SuperSampling_Available:
		*OutValue = 1;
		break;
	case Util::NvParameter::SuperSampling_NeedsUpdatedDriver:
		*OutValue = FALSE;
		break;
	case Util::NvParameter::SuperSampling_MinDriverVersionMinor:
	case Util::NvParameter::SuperSampling_MinDriverVersionMajor:
		*OutValue = 0;
		break;
	case Util::NvParameter::OutWidth:
		*OutValue = OutWidth;
		break;
	case Util::NvParameter::OutHeight:
		*OutValue = OutHeight;
		break;
	case Util::NvParameter::DLSS_Get_Dynamic_Max_Render_Width:
		*OutValue = OutWidth;
		break;
	case Util::NvParameter::DLSS_Get_Dynamic_Max_Render_Height:
		*OutValue = OutHeight;
		break;
	case Util::NvParameter::DLSS_Get_Dynamic_Min_Render_Width:
		*OutValue = OutWidth;
		break;
	case Util::NvParameter::DLSS_Get_Dynamic_Min_Render_Height:
		*OutValue = OutHeight;
		break;
	default:
		*OutValue = 0;
		return NVSDK_NGX_Result_FAIL_InvalidParameter;
	}

	return NVSDK_NGX_Result_Success;
}

NVSDK_NGX_Result Dx12ParameterImpl::Get(const char* InName, ID3D11Resource** OutValue) const
{
	return NVSDK_NGX_Result_Fail;
}

NVSDK_NGX_Result Dx12ParameterImpl::Get(const char* InName, ID3D12Resource** OutValue) const
{
	return NVSDK_NGX_Result_Fail;
}

NVSDK_NGX_Result NVSDK_CONV NVSDK_NGX_DLSS_GetOptimalSettingsCallback(NVSDK_NGX_Parameter* InParams);

NVSDK_NGX_Result Dx12ParameterImpl::Get(const char* InName, void** OutValue) const
{
	switch (Util::NvParameterToEnum(InName))
	{
	case Util::NvParameter::DLSSOptimalSettingsCallback:
		*OutValue = NVSDK_NGX_DLSS_GetOptimalSettingsCallback;
		break;
	default:
		*OutValue = nullptr;
		return NVSDK_NGX_Result_FAIL_InvalidParameter;
	}

	return NVSDK_NGX_Result_Success;
}

void Dx12ParameterImpl::Reset()
{
	//TODO
}

void Dx12ParameterImpl::EvaluateRenderScale()
{
	FfxFsr2QualityMode fsrQualityMode;

	switch (PerfQualityValue)
	{
	case NVSDK_NGX_PerfQuality_Value_MaxPerf:
		fsrQualityMode = FFX_FSR2_QUALITY_MODE_PERFORMANCE;
		break;
	case NVSDK_NGX_PerfQuality_Value_Balanced:
		fsrQualityMode = FFX_FSR2_QUALITY_MODE_BALANCED;
		break;
	case NVSDK_NGX_PerfQuality_Value_MaxQuality:
		fsrQualityMode = FFX_FSR2_QUALITY_MODE_QUALITY;
		return;
	case NVSDK_NGX_PerfQuality_Value_UltraPerformance:
		fsrQualityMode = FFX_FSR2_QUALITY_MODE_ULTRA_PERFORMANCE;
		return;
	case NVSDK_NGX_PerfQuality_Value_UltraQuality:
		//Not defined by AMD
		OutHeight = Height;
		OutWidth = Width;
		return;
	}

	ffxFsr2GetRenderResolutionFromQualityMode(&OutWidth, &OutHeight, Width, Height, fsrQualityMode);
}
