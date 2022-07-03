#include "pch.h"
#include "ViewMatrixHook.h"
#include "Config.h"

ViewMatrixHook::ViewMatrixHook()
{
	//TODO check for different executable versions

	/*
	Protip for future self to get the offsets, search for the vertical FOV to get the structure, then look for references to that structure and afterwards look for static references
	*/

	auto mod = (uint64_t)GetModuleHandleW(L"Cyberpunk2077.exe");
	if (mod != 0)
	{
		auto ptr1 = *((uintptr_t*)(mod + 0x4B6F888));
		camParams = ((CameraParams*)(ptr1 + 0x60));
	}
}

float ViewMatrixHook::GetFov()
{
	return Config::instance().VerticalFOV.value_or(camParams ? camParams->FoV : 60.0f);
}

float ViewMatrixHook::GetFarPlane()
{
	float infinity = std::numeric_limits<float>::infinity();
	return Config::instance().FarPlane.value_or(camParams ? camParams->FarPlane : infinity);
}

float ViewMatrixHook::GetNearPlane()
{
	return Config::instance().NearPlane.value_or(camParams ? camParams->NearPlane : 0.0f);
}
