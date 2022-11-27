#include "pch.h"
#include "ViewMatrixHook.h"
#include "scanner.h"
std::unique_ptr<ViewMatrixHook> ViewMatrixHook::Create(const Config& config)
{
	switch (config.Method.value_or(ViewMethod::Config))
	{
		case ViewMethod::Cyberpunk2077:
			return std::make_unique<ViewMatrixHook::Cyberpunk2077>();
			
		case ViewMethod::RDR2:
			return std::make_unique<ViewMatrixHook::RDR2>();

		case ViewMethod::Config:
		default:
			return std::make_unique<ViewMatrixHook::Configured>(
				config.VerticalFOV.value_or(60.0f),
				config.FarPlane.value_or(std::numeric_limits<float>::infinity()),
				config.NearPlane.value_or(0.0f)
			);
	}
}

#pragma region Configured

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

#pragma endregion

#pragma region Cyberpunk2077

ViewMatrixHook::Cyberpunk2077::Cyberpunk2077()
{
	//TODO check for different executable versions

	/*
	Protip for future self to get the offsets, search for the vertical FOV to get the structure, then look for references to that structure and afterwards look for static references
	*/

	auto loc = *(uintptr_t*)scanner::GetOffsetFromInstruction(L"Cyberpunk2077.exe", "F3 0F 7F 0D ? ? ? ? E8", 4);
	camParams = ((CameraParams*)(loc + 0x60));
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

#pragma endregion

#pragma region RDR2

ViewMatrixHook::RDR2::RDR2()
{
	auto loc = scanner::GetOffsetFromInstruction(L"RDR2.exe", "4C 8D 2D ? ? ? ? 48 85 DB", 3);
	camParams = ((CameraParams*)(loc + 0x60));
}

float ViewMatrixHook::RDR2::GetFov()
{
	return camParams->Fov;
}

float ViewMatrixHook::RDR2::GetFarPlane()
{
	return camParams->FarPlane;
}

float ViewMatrixHook::RDR2::GetNearPlane()
{
	return camParams->NearPlane;
}

#pragma endregion
