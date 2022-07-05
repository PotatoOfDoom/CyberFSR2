#include "pch.h"
#include "Util.h"

double Util::MillisecondsNow()
{
	static LARGE_INTEGER s_frequency;
	static BOOL s_use_qpc = QueryPerformanceFrequency(&s_frequency);
	double milliseconds = 0;

	if (s_use_qpc)
	{
		LARGE_INTEGER now;
		QueryPerformanceCounter(&now);
		milliseconds = double(1000.0 * now.QuadPart) / s_frequency.QuadPart;
	}
	else
	{
		milliseconds = double(GetTickCount());
	}

	return milliseconds;
}

Util::NvParameter Util::NvParameterToEnum(const char* name)
{
	static std::unordered_map<std::string, NvParameter> NvParamTranslation = {
	{"SuperSampling.ScaleFactor", NvParameter::SuperSampling_ScaleFactor},
	{"SuperSampling.Available", NvParameter::SuperSampling_Available},
	{"SuperSampling.MinDriverVersionMajor", NvParameter::SuperSampling_MinDriverVersionMajor},
	{"SuperSampling.MinDriverVersionMinor", NvParameter::SuperSampling_MinDriverVersionMinor},
	{"SuperSampling.FeatureInitResult", NvParameter::SuperSampling_FeatureInitResult},
	{"SuperSampling.NeedsUpdatedDriver", NvParameter::SuperSampling_NeedsUpdatedDriver},

	{"Width", NvParameter::Width},
	{"Height", NvParameter::Height},
	{"PerfQualityValue", NvParameter::PerfQualityValue},
	{"RTXValue", NvParameter::RTXValue},

	{"OutWidth", NvParameter::OutWidth},
	{"OutHeight", NvParameter::OutHeight},

	{"DLSS.Render.Subrect.Dimensions.Width", NvParameter::DLSS_Render_Subrect_Dimensions_Width},
	{"DLSS.Render.Subrect.Dimensions.Height", NvParameter::DLSS_Render_Subrect_Dimensions_Height},
	{"DLSS.Get.Dynamic.Max.Render.Width", NvParameter::DLSS_Get_Dynamic_Max_Render_Width},
	{"DLSS.Get.Dynamic.Max.Render.Height", NvParameter::DLSS_Get_Dynamic_Max_Render_Height},
	{"DLSS.Get.Dynamic.Min.Render.Width", NvParameter::DLSS_Get_Dynamic_Min_Render_Width},
	{"DLSS.Get.Dynamic.Min.Render.Height", NvParameter::DLSS_Get_Dynamic_Min_Render_Height},
	{"Sharpness", NvParameter::Sharpness},

	{"DLSSOptimalSettingsCallback", NvParameter::DLSSOptimalSettingsCallback},
	{"DLSSGetStatsCallback", NvParameter::DLSSGetStatsCallback},

	{"CreationNodeMask", NvParameter::CreationNodeMask},
	{"VisibilityNodeMask", NvParameter::VisibilityNodeMask},
	{"DLSS.Feature.Create.Flags", NvParameter::DLSS_Feature_Create_Flags},
	{"DLSS.Enable.Output.Subrects", NvParameter::DLSS_Enable_Output_Subrects},

	{"Color", NvParameter::Color},
	{"MotionVectors", NvParameter::MotionVectors},
	{"Depth", NvParameter::Depth},
	{"Output", NvParameter::Output},
	{"TransparencyMask", NvParameter::TransparencyMask},
	{"ExposureTexture", NvParameter::ExposureTexture},
	{"DLSS.Input.Bias.Current.Color.Mask", NvParameter::DLSS_Input_Bias_Current_Color_Mask},

	{"DLSS.Pre.Exposure", NvParameter::Pre_Exposure},
	{"DLSS.Exposure.Scale", NvParameter::Exposure_Scale},

	{"Reset", NvParameter::Reset},
	{"MV.Scale.X", NvParameter::MV_Scale_X},
	{"MV.Scale.Y", NvParameter::MV_Scale_Y},
	{"Jitter.Offset.X", NvParameter::Jitter_Offset_X},
	{"Jitter.Offset.Y", NvParameter::Jitter_Offset_Y},
	};

	return NvParamTranslation[std::string(name)];
}
