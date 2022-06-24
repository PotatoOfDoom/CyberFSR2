#pragma once

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
	CameraParams* camParams = nullptr;

public:
	ViewMatrixHook();

	float GetFov();
	float GetFarPlane();
	float GetNearPlane();
};

