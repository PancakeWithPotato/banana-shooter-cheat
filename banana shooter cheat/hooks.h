#pragma once
#include <cstdint>
#include "ext/mh/MinHook.h"
#include "hack.h"
#include "functions.h"
class Hooks 
{
private:
	struct HookReturn { //return type for AddHook (returns the status and the original as well)
		MH_STATUS status;
		void* oAddr;
	};
	HookReturn AddHook(void* pTarget, void* detour);

	int m_iCount = 0; //amount of hooks, used to keep track of hook #
public:
	bool Setup(); //setup hooks
	void Destroy();
	static void __cdecl hRecoilFir(void* thisptr, float x, float y, float z);
	static void __cdecl hCreateBullet(void* thisptr, UnityEngine_Vector3_o);
	static void __cdecl hDoAttack(Firearms_o* thisptr);

private:
	//typedefs lolz
	typedef void(__cdecl* RecoilFirFN)(void* thisptr, float x, float y, float z);

	typedef void(__cdecl* CreateBulletFN)(void*, UnityEngine_Vector3_o);
	typedef void(__cdecl* DoAttackFN)(Firearms_o*);
public:
	RecoilFirFN oRecoil = nullptr;

	CreateBulletFN oCreateBullet = nullptr;
	DoAttackFN oDoAttack = nullptr;
}; inline Hooks* g_Hooks = new Hooks();