#pragma once
#include "sdk/il2cpp.h"
#include "hack.h"
#include "offsets.h"
class Functions {

private:
	typedef void(__cdecl* CreateExplosiveBulletFN)(void*, UnityEngine_Vector3_o);

	typedef UnityEngine_Camera_o*(__cdecl* CameraGetCurrentFN)();
	typedef void(__cdecl* CameraSetAspect)(void*, float);
public:
	CreateExplosiveBulletFN pCreateExplosiveBullet = nullptr;

	CameraGetCurrentFN pGetCurrentCam = nullptr;
	CameraSetAspect pSetCameraAspect = nullptr;
public:

	void Setup() 
	{
		pCreateExplosiveBullet = reinterpret_cast<CreateExplosiveBulletFN>(g_Hack->pAssembly + Offsets::Firearms::CreateExplosiveBullet);
		pGetCurrentCam = reinterpret_cast<CameraGetCurrentFN>(g_Hack->pAssembly + Offsets::Cam::GetCurrent);
		pSetCameraAspect = reinterpret_cast<CameraSetAspect>(g_Hack->pAssembly + Offsets::Cam::SetAspect);

	}

	//thx stephan (even tho you did not make it lol)
	UnityEngine_Vector3_o getTransformPosition(UnityEngine_Transform_o* transform) {
		if (!transform || sizeof(*transform) != sizeof(UnityEngine_Transform_o))
			return UnityEngine_Vector3_o{};

		if (!transform->fields.m_CachedPtr)
			return UnityEngine_Vector3_o{};

		return reinterpret_cast<UnityEngine_Vector3_o(__cdecl*)(UnityEngine_Transform_o*)>(g_Hack->pAssembly + Offsets::Transform::GetPos)(transform);
	}


}; inline Functions* g_Funcs = new Functions();