#pragma once
#include "pch.h"

enum class SharpnessRangeModifier
{
	Normal,
	Extended,
};

enum class ViewMethod
{
	Config,
	Cyberpunk2077,
	RDR2,
	DL2,
};

class Config
{
public:
	Config(std::wstring fileName);

	// Depth
	std::optional<bool> DepthInverted;

	// Color
	std::optional<bool> AutoExposure;
	std::optional<bool> HDR;

	// Motion
	std::optional<bool> JitterCancellation;
	std::optional<bool> DisplayResolution;

	// Sharpening
	std::optional<bool> EnableSharpening;
	std::optional<float> Sharpness;
	std::optional<SharpnessRangeModifier> SharpnessRange;

	// Upscale Ratio Override
	std::optional<bool> UpscaleRatioOverrideEnabled;
	std::optional<float> UpscaleRatioOverrideValue;
	
	// Quality Overrides
	std::optional<bool> QualityRatioOverrideEnabled;
	std::optional<float> QualityRatio_UltraQuality;
	std::optional<float> QualityRatio_Quality;
	std::optional<float> QualityRatio_Balanced;
	std::optional<float> QualityRatio_Performance;
	std::optional<float> QualityRatio_UltraPerformance;

	// View
	std::optional<ViewMethod> Method;
	std::optional<float> VerticalFOV;
	std::optional<float> NearPlane;
	std::optional<float> FarPlane;
	std::optional<bool> InfiniteFarPlane;

	//Hotfix for Steam Deck
	std::optional<bool> DisableReactiveMask;

	void Reload();

private:
	CSimpleIniA ini;

	std::filesystem::path absoluteFileName;

	std::optional<std::string> readString(std::string section, std::string key, bool lowercase = false);
	std::optional<float> readFloat(std::string section, std::string key);
	std::optional<bool> readBool(std::string section, std::string key);
	std::optional<SharpnessRangeModifier> readSharpnessRange(std::string section, std::string key);
	std::optional<ViewMethod> readViewMethod(std::string section, std::string key);
};
