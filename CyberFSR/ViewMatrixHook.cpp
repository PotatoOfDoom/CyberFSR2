#include "pch.h"
#include "ViewMatrixHook.h"

std::unique_ptr<ViewMatrixHook> ViewMatrixHook::Create(const Config& config)
{
	switch (config.Method.value_or(ViewMethod::Config))
	{
		case ViewMethod::Cyberpunk2077:
			return std::make_unique<ViewMatrixHook::Cyberpunk2077>();

		case ViewMethod::Config:
		default:
			return std::make_unique<ViewMatrixHook::Configured>(
				config.VerticalFOV.value_or(60.0f),
				config.FarPlane.value_or(std::numeric_limits<float>::infinity()),
				config.NearPlane.value_or(0.0f)
			);
	}
}

ViewMatrixHook::Configured::Configured(float fov, float nearPlane, float farPlane)
	: Fov(fov)
	, NearPlane(nearPlane)
	, FarPlane(farPlane)
{
}

float ViewMatrixHook::Configured::GetFov()
{
	return Fov;
}

float ViewMatrixHook::Configured::GetFarPlane()
{
	return FarPlane;
}

float ViewMatrixHook::Configured::GetNearPlane()
{
	return NearPlane;
}

ViewMatrixHook::Cyberpunk2077::Cyberpunk2077()
{
	//TODO check for different executable versions

	/*
	Protip for future self to get the offsets, search for the vertical FOV to get the structure, then look for references to that structure and afterwards look for static references
	*/

	auto mod = (uint64_t)GetModuleHandleW(L"Cyberpunk2077.exe");
	auto ptr1 = *((uintptr_t*)(mod + 0x4B6F888));
	camParams = ((CameraParams*)(ptr1 + 0x60));
}

float ViewMatrixHook::Cyberpunk2077::GetFov()
{
	return camParams->Fov;
}

float ViewMatrixHook::Cyberpunk2077::GetFarPlane()
{
	return camParams->FarPlane;
}

float ViewMatrixHook::Cyberpunk2077::GetNearPlane()
{
	return camParams->NearPlane;
}
