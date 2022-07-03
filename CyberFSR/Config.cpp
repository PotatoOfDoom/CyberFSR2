#include "pch.h"
#include "Config.h"

extern HMODULE hMod;

Config::Config(std::wstring fileName)
{
	std::wstring dllName = L"nvngx.dll";
	wchar_t dllPath[MAX_PATH];
	GetModuleFileNameW(hMod, dllPath, MAX_PATH);

	absoluteFileName = std::wstring(dllPath);
	absoluteFileName.replace(absoluteFileName.find(dllName), dllName.size(), fileName);

	// Depth
	DepthInverted = readBool(L"Depth", L"DepthInverted");

	// Color
	AutoExposure = readBool(L"Color", L"AutoExposure");
	HDR = readBool(L"Color", L"HDR");

	// MotionVectors
	JitterCancellation = readBool(L"MotionVectors", L"JitterCancellation");
	DisplayResolution = readBool(L"MotionVectors", L"DisplayResolution");

	// Sharpening
	EnableSharpening = readBool(L"Sharpening", L"EnableSharpening");
	Sharpness = readFloat(L"Sharpening", L"Sharpness");
	SharpnessRange = readSharpnessRange(L"Sharpening", L"SharpnessRange");

	// View
	VerticalFOV = readFloat(L"View", L"VerticalFOV");
	NearPlane = readFloat(L"View", L"NearPlane");
	FarPlane = readFloat(L"View", L"FarPlane");
}

std::wstring Config::readValue(std::wstring section, std::wstring key)
{
	wchar_t value[100];
	GetPrivateProfileStringW(
		section.c_str(), // lpAppName
		key.c_str(), // lpKeyName
		L"auto", // lpDefault
		value, // lpReturnedString
		100, // nSize
		absoluteFileName.c_str() // lpFileName
	);
	return std::wstring(value);
}

std::optional<std::wstring> Config::readString(std::wstring section, std::wstring key)
{
	auto value = readValue(section, key);
	if (value == L"auto")
	{
		return std::nullopt;
	}
	return value;
}

std::optional<bool> Config::readBool(std::wstring section, std::wstring key)
{
	auto value = readValue(section, key);
	std::transform(
		value.begin(), value.end(),
		value.begin(),
		std::towlower
	);

	if (value == L"true")
	{
		return true;
	}
	else if (value == L"false")
	{
		return false;
	}
	else
	{
		return std::nullopt;
	}
}

std::optional<float> Config::readFloat(std::wstring section, std::wstring key)
{
	auto value = readValue(section, key);
	if (value == L"auto")
	{
		return std::nullopt;
	}

	try
	{
		return std::stof(value);
	}
	catch (const std::invalid_argument&) // invalid float string for std::stof
	{
		return std::nullopt;
	}
	catch (const std::out_of_range&) // out of range for 32 bit float
	{
		return std::nullopt;
	}
}

std::optional<SharpnessRangeModifier> Config::readSharpnessRange(std::wstring section, std::wstring key)
{
	auto value = readValue(section, key);
	std::transform(
		value.begin(), value.end(),
		value.begin(),
		std::towlower
	);

	if (value == L"normal")
	{
		return SharpnessRangeModifier::Normal;
	}
	else if (value == L"negative")
	{
		return SharpnessRangeModifier::Negative;
	}
	else
	{
		return std::nullopt;
	}
}
