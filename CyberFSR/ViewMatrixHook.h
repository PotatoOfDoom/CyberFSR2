#pragma once
#include "Config.h"

class ViewMatrixHook
{
public:
	virtual float GetFov() = 0;
	virtual float GetFarPlane() = 0;
	virtual float GetNearPlane() = 0;

	class Configured;
	class Cyberpunk2077;
	class RDR2;
	class DL2;

	static std::unique_ptr<ViewMatrixHook> Create(const Config& config);
};

class ViewMatrixHook::Configured : public ViewMatrixHook
{
public:
	Configured(float fov, float nearPlane, float farPlane);

	float GetFov();
	float GetFarPlane();
	float GetNearPlane();

private:
	float Fov;
	float FarPlane;
	float NearPlane;
};

class ViewMatrixHook::Cyberpunk2077 : public ViewMatrixHook
{
public:
	Cyberpunk2077();

	float GetFov();
	float GetFarPlane();
	float GetNearPlane();

private:
	struct CameraParams
	{
		unsigned char unknown0[0x20];
		float Fov;
		unsigned char unknown1[0x1C];
		float NearPlane;
		float FarPlane;
	};

	CameraParams* camParams = nullptr;
};

class ViewMatrixHook::RDR2 : public ViewMatrixHook
{
public:
	RDR2();

	float GetFov();
	float GetFarPlane();
	float GetNearPlane();

private:
	struct CameraParams
	{
		float Fov;
		float NearPlane;
		float FarPlane;
	};

	CameraParams* camParams = nullptr;
};

class ViewMatrixHook::DL2 : public ViewMatrixHook
{
public:
	DL2();

	float GetFov();
	float GetFarPlane();
	float GetNearPlane();

private:
	struct ExtraCameraParams
	{
		float Fov;
	};

	ExtraCameraParams* extracamParams = nullptr;

	struct CameraParams
	{
		float Fov;
		float NearPlane;
		float FarPlane;
	};

	CameraParams* camParams = nullptr;
};