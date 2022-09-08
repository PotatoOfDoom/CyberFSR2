#include "pch.h"
#include "Config.h"
#include "Util.h"

Config::Config(std::string fileName)
{
	absoluteFileName = Util::DllPath().parent_path() / fileName;

	Reload();
}

void Config::Reload()
{
	if (ini.LoadFile(absoluteFileName.c_str()) == SI_OK)
	{
		// Depth
		DepthInverted = readBool("Depth", "DepthInverted");

		// Color
		AutoExposure = readBool("Color", "AutoExposure");
		HDR = readBool("Color", "HDR");

		// MotionVectors
		JitterCancellation = readBool("MotionVectors", "JitterCancellation");
		DisplayResolution = readBool("MotionVectors", "DisplayResolution");

		// Sharpening
		EnableSharpening = readBool("Sharpening", "EnableSharpening");
		Sharpness = readFloat("Sharpening", "Sharpness");
		SharpnessRange = readSharpnessRange("Sharpening", "SharpnessRange");

		//Upscale Ratio Override
		UpscaleRatioOverrideEnabled = readBool("UpscaleRatio", "UpscaleRatioOverrideEnabled");
		UpscaleRatioOverrideValue = readFloat("UpscaleRatio", "UpscaleRatioOverrideValue");

		// View
		Method = readViewMethod("View", "Method");
		VerticalFOV = readFloat("View", "VerticalFOV");
		NearPlane = readFloat("View", "NearPlane");
		FarPlane = readFloat("View", "FarPlane");
		InfiniteFarPlane = readBool("View", "InfiniteFarPlane");

		DisableReactiveMask = readBool("Hotfix", "DisableReactiveMask");
	}

	auto exeName = Util::ExePath().filename();

	if (exeName == "Cyberpunk2077.exe")
	{
		Method = Method.value_or(ViewMethod::Cyberpunk2077);
	}
	else if (exeName == "DyingLightGame_x64_rwdi.exe")
	{
		SharpnessRange = SharpnessRange.value_or(SharpnessRangeModifier::Extended);
	}
	else if (exeName == "RDR2.exe")
	{
		Method = Method.value_or(ViewMethod::RDR2);
	}
}

std::optional<std::string> Config::readString(std::string section, std::string key, bool lowercase)
{
	std::string value = ini.GetValue(section.c_str(), key.c_str(), "auto");

	std::string lower = value;
	std::transform(
		lower.begin(), lower.end(),
		lower.begin(),
		[](unsigned char c)
		{
			return std::tolower(c);
		}
	);

	if (lower == "auto")
	{
		return std::nullopt;
	}
	return lowercase ? lower : value;
}

std::optional<float> Config::readFloat(std::string section, std::string key)
{
	auto value = readString(section, key);
	try
	{
		return std::stof(value.value());
	}
	catch (const std::bad_optional_access&) // missing or auto value
	{
		return std::nullopt;
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

std::optional<bool> Config::readBool(std::string section, std::string key)
{
	auto value = readString(section, key, true);
	if (value == "true")
	{
		return true;
	}
	else if (value == "false")
	{
		return false;
	}

	return std::nullopt;
}

std::optional<SharpnessRangeModifier> Config::readSharpnessRange(std::string section, std::string key)
{
	auto value = readString(section, key, true);
	if (value == "normal")
	{
		return SharpnessRangeModifier::Normal;
	}
	else if (value == "extended")
	{
		return SharpnessRangeModifier::Extended;
	}

	return std::nullopt;
}

std::optional<ViewMethod> Config::readViewMethod(std::string section, std::string key)
{
	auto value = readString(section, key, true);
	if (value == "config")
	{
		return ViewMethod::Config;
	}
	else if (value == "cyberpunk2077")
	{
		return ViewMethod::Cyberpunk2077;
	}
	else if (value == "rdr2")
	{
		return ViewMethod::RDR2;
	}

	return std::nullopt;
}
