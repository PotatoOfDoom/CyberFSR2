#pragma once
#include "Config.h"

struct CameraParams
{
	unsigned char unknown0[0x20];
	float FoV;
	unsigned char unknown1[0x1C];
	float NearPlane;
	float FarPlane;
};

class ViewMatrixHook
{
	const Config& config;
	CameraParams* camParams = nullptr;

public:
	ViewMatrixHook(const Config& config);

	float GetFov();
	float GetFarPlane();
	float GetNearPlane();
};

