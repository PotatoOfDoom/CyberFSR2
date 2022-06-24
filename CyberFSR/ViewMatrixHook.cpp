#include "pch.h"
#include "ViewMatrixHook.h"

ViewMatrixHook::ViewMatrixHook()
{
	//TODO check for different executable versions

	/*
	Protip for future self to get the offsets, search for the vertical FOV to get the structure, then look for references to that structure and afterwards look for static references
	*/

	auto mod = (uint64_t)GetModuleHandleW(L"Cyberpunk2077.exe");
	auto ptr1 = *((uintptr_t*)(mod + 0x4B6F888));
	camParams = ((CameraParams*)(ptr1 + 0x60));
}

float ViewMatrixHook::GetFov()
{
	return camParams->FoV;
}

float ViewMatrixHook::GetFarPlane()
{
	return camParams->FarPlane;
}

float ViewMatrixHook::GetNearPlane()
{
	return camParams->NearPlane;
}
