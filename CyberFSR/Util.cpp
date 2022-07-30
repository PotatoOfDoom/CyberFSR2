#include "pch.h"
#include "Config.h"
#include "Util.h"

namespace fs = std::filesystem;

extern HMODULE dllModule;

fs::path Util::DllPath()
{
	static fs::path dll;
	if (dll.empty())
	{
		wchar_t dllPath[MAX_PATH];
		GetModuleFileNameW(dllModule, dllPath, MAX_PATH);
		dll = fs::path(dllPath);
	}
	return dll;
}

fs::path Util::ExePath()
{
	static fs::path exe;
	if (exe.empty())
	{
		wchar_t exePath[MAX_PATH];
		GetModuleFileNameW(nullptr, exePath, MAX_PATH);
		exe = fs::path(exePath);
	}
	return exe;
}

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

float Util::ConvertSharpness(float sharpness, std::optional<SharpnessRangeModifier> range)
{
	if (range == SharpnessRangeModifier::Extended)
	{
		// normalize sharpness value to [0, 1] range
		// originally in range [-0.99, 1]
		if (sharpness >= 1.0f)
		{
			return 1.0f;
		}
		else if (sharpness <= -1.0f)
		{
			return 0;
		}
		else
		{
			return (sharpness + 0.99f) / 2.0f;
		}
	}
	else
	{
		return sharpness;
	}
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
		{"#\x01", NvParameter::SuperSampling_Available},

		{"Width", NvParameter::Width},
		{"Height", NvParameter::Height},
		{"PerfQualityValue", NvParameter::PerfQualityValue},
		{"RTXValue", NvParameter::RTXValue},
		{"NVSDK_NGX_Parameter_FreeMemOnReleaseFeature", NvParameter::FreeMemOnReleaseFeature},

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

		{"SizeInBytes", NvParameter::SizeInBytes},
		{"Snippet.OptLevel", NvParameter::OptLevel},
		{"#\x44", NvParameter::OptLevel},
		{"Snippet.IsDevBranch", NvParameter::IsDevSnippetBranch},
		{"#\x45", NvParameter::IsDevSnippetBranch}
	};

	return NvParamTranslation[std::string(name)];
}
