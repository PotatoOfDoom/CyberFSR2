#pragma once
#include "Config.h"

namespace Util
{
	std::filesystem::path ExePath();

	std::filesystem::path DllPath();

	double MillisecondsNow();

	float ConvertSharpness(float sharpness, std::optional<SharpnessRangeModifier> range);

	enum class NvParameter
	{
		Invalid,

		//SuperSampling
		SuperSampling_ScaleFactor,
		SuperSampling_Available,
		SuperSampling_MinDriverVersionMajor,
		SuperSampling_MinDriverVersionMinor,
		SuperSampling_FeatureInitResult,
		SuperSampling_NeedsUpdatedDriver,
		//User settings stuff
		Width,
		Height,
		PerfQualityValue,
		RTXValue,
		//Resolution stuff
		OutWidth,
		OutHeight,

		DLSS_Render_Subrect_Dimensions_Width,
		DLSS_Render_Subrect_Dimensions_Height,
		DLSS_Get_Dynamic_Max_Render_Width,
		DLSS_Get_Dynamic_Max_Render_Height,
		DLSS_Get_Dynamic_Min_Render_Width,
		DLSS_Get_Dynamic_Min_Render_Height,
		Sharpness,
		//Callbacks
		DLSSGetStatsCallback,
		DLSSOptimalSettingsCallback,

		//Render stuff
		CreationNodeMask,
		VisibilityNodeMask,
		DLSS_Feature_Create_Flags,
		DLSS_Enable_Output_Subrects,

		//D3D12 Buffers
		Color,
		MotionVectors,
		Depth,
		Output,
		TransparencyMask,
		ExposureTexture,
		DLSS_Input_Bias_Current_Color_Mask,
		Pre_Exposure,
		Exposure_Scale,

		Reset,
		MV_Scale_X,
		MV_Scale_Y,
		Jitter_Offset_X,
		Jitter_Offset_Y,
	};

	NvParameter NvParameterToEnum(const char* name);
};

inline void ThrowIfFailed(HRESULT hr)
{
	if (FAILED(hr))
	{
		// Set a breakpoint on this line to catch DirectX API errors
		throw std::exception();
	}
}