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
		case ViewMethod::DL2:
			return std::make_unique<ViewMatrixHook::DL2>();

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

#pragma region DL2

ViewMatrixHook::DL2::DL2()
{
	// the real fov value is found in "gamedll_ph_x64_rwdi.dll" but a some pointers from "engine_x64_rwdi.dll" has access to it 
	// in DL2 the CameraDefaultFOV is 57
	// the game keeps 57 fov as default and there is another fov value called extraFOV which users can change as part of game settings
	// the real fov is CameraDefaultFOV + extraFOV
	// the address below is the real FOV address but it is not accessible until the load into the game world so we find the CameraDefaultFOV and extraFOV address and calculate our fov from these
	// auto loc = (*(uintptr_t*)(*(uintptr_t*)(*(uintptr_t*)(*(uintptr_t*)(*(uintptr_t*)(scanner::GetOffsetFromInstruction(L"engine_x64_rwdi.dll", "48 8D 2D ? ? ? ? 8B 0D", 3) + 0x18) + 0x28) + 0x20) + 0x18) + 0x150) + 0x98);
	uintptr_t ptr = (scanner::GetOffsetFromInstruction(L"gamedll_ph_x64_rwdi.dll","EB ? E8 ? ? ? ? 48 8D 78 ? E8", 3) + 0xDF);
	ptr += (*(int32_t*)ptr + sizeof(int32_t)) + 0xCC8;  // ptr + 0xCC8 this is pointer to whatever is saved in CameraDefaultFOV from player_variables.scr
	camParams = (CameraParams*)ptr;

	auto ptr2 = (*(uintptr_t*)(*(uintptr_t*)scanner::GetOffsetFromInstruction(L"engine_x64_rwdi.dll", "48 8B 05 ? ? ? ? 8B DA", 3)+0xE0)+0x15C); 
	extracamParams = (ExtraCameraParams*)ptr2; //extraFOV
}    

float ViewMatrixHook::DL2::GetFov()
{
	auto param1 = camParams->Fov;
	auto param2 = extracamParams->Fov;

	return (param1 + param2);
}

float ViewMatrixHook::DL2::GetFarPlane()
{
	return camParams->FarPlane;
}

float ViewMatrixHook::DL2::GetNearPlane()
{
	return camParams->NearPlane;
}

#pragma endregion
