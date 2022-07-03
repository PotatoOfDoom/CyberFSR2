#pragma once
#include "pch.h"

enum class SharpnessRangeModifier {
	Normal,
	Negative
};

class Config
{
public: 
	// Init
	std::optional<bool> DepthInverted;
	std::optional<bool> AutoExposure;
	std::optional<bool> Hdr;
	std::optional<bool> JitterMotion;
	std::optional<bool> LowRes;

	// Dispatch
	std::optional<bool> EnableSharpening;
	std::optional<float> Sharpness;
	std::optional<SharpnessRangeModifier> SharpnessRange;

	// Dispatch.View
	std::optional<float> VerticalFOV;
	std::optional<float> NearPlane;
	std::optional<float> FarPlane;

	static const Config& instance()
	{
		static Config INSTANCE(L"nvngx.ini");
		return INSTANCE;
	}

private:
	Config(std::wstring fileName);

	std::wstring absoluteFileName;

	std::wstring readValue(std::wstring section, std::wstring key);

	std::optional<std::wstring> readString(std::wstring section, std::wstring key);
	std::optional<bool> readBool(std::wstring section, std::wstring key);
	std::optional<float> readFloat(std::wstring section, std::wstring key);
	std::optional<SharpnessRangeModifier> readSharpnessRange(std::wstring section, std::wstring key);
};
