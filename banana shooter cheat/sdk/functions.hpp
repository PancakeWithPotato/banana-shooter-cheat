#pragma once
#include "sdk.hpp"
#include "../core/hack.hpp"
#include <string>

class Functions {
private:
	typedef void(__cdecl* CreateExplosiveBulletFN)(void*, Vector3);
	typedef CreateExplosiveBulletFN CreateBulletFN;

	typedef Camera*(__cdecl* CameraGetCurrentFN)();
	typedef void(__cdecl* CameraSetAspect)(void*, float);

	typedef void(__cdecl* AddMessageFN)(void*, std::string, UnityEngine_Color_o);
	typedef std::string(__cdecl* SwearCheckFN)(void*, std::string);
public:
	CreateExplosiveBulletFN pCreateExplosiveBullet = nullptr;
	CreateBulletFN pCreateBullet = nullptr;

	CameraGetCurrentFN pGetCurrentCam = nullptr;
	CameraSetAspect pSetCameraAspect = nullptr;

	AddMessageFN pAddMessage;
public:
	void Setup() 
	{
		pCreateExplosiveBullet = reinterpret_cast<CreateExplosiveBulletFN>(Offsets::pAssembly + Offsets::Firearms::CreateExplosiveBullet);
		pCreateBullet = reinterpret_cast<CreateBulletFN>(Offsets::pAssembly + Offsets::Firearms::CreateBullet);

		pGetCurrentCam = reinterpret_cast<CameraGetCurrentFN>(Offsets::pAssembly + Offsets::Cam::GetCurrent);
		pSetCameraAspect = reinterpret_cast<CameraSetAspect>(Offsets::pAssembly + Offsets::Cam::SetAspect);

		pAddMessage = reinterpret_cast<AddMessageFN>(Offsets::pAssembly + Offsets::Chat::AddMessage);
	}
}; inline Functions* g_Funcs = new Functions();