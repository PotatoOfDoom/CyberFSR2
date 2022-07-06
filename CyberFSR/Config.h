#pragma once
#include "pch.h"

enum class SharpnessRangeModifier {
	Normal,
	Extended,
};

enum class ViewMethod {
	Config,
	Cyberpunk2077,
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

	// View
	std::optional<ViewMethod> Method;
	std::optional<float> VerticalFOV;
	std::optional<float> NearPlane;
	std::optional<float> FarPlane;

private:
	std::wstring absoluteFileName;

	std::wstring readValue(std::wstring section, std::wstring key);

	std::optional<std::wstring> readString(std::wstring section, std::wstring key);
	std::optional<bool> readBool(std::wstring section, std::wstring key);
	std::optional<float> readFloat(std::wstring section, std::wstring key);
	std::optional<SharpnessRangeModifier> readSharpnessRange(std::wstring section, std::wstring key);
	std::optional<ViewMethod> readViewMethod(std::wstring section, std::wstring key);
};
