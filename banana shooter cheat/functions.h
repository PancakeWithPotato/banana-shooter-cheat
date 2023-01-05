#pragma once
#include "sdk/il2cpp.h"
#include "hack.h"
#include "offsets.h"
class Functions {

private:
	typedef void(__cdecl* CreateExplosiveBulletFN)(void*, UnityEngine_Vector3_o);

public:
	CreateExplosiveBulletFN pCreateExplosiveBullet = nullptr;
public:

	void Setup() 
	{
		pCreateExplosiveBullet = reinterpret_cast<CreateExplosiveBulletFN>(g_Hack->pAssembly + Offsets::Firearms::CreateExplosiveBullet);
	}


}; inline Functions* g_Funcs = new Functions();