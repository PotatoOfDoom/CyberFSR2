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
};

class Config
{
public:
	Config(std::string fileName);

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

	// View
	std::optional<ViewMethod> Method;
	std::optional<float> VerticalFOV;
	std::optional<float> NearPlane;
	std::optional<float> FarPlane;

private:
	CSimpleIniA ini;

	std::optional<std::string> readString(std::string section, std::string key, bool lowercase = false);
	std::optional<float> readFloat(std::string section, std::string key);
	std::optional<bool> readBool(std::string section, std::string key);
	std::optional<SharpnessRangeModifier> readSharpnessRange(std::string section, std::string key);
	std::optional<ViewMethod> readViewMethod(std::string section, std::string key);
};
