#pragma once
#include <cstdint>
#include "ext/mh/MinHook.h"
#include "offsets.h"
#include "hack.h"
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

private:
	//typedefs lolz
	typedef void(__cdecl* RecoilFirFN)(void* thisptr, float x, float y, float z);
public:
	RecoilFirFN oRecoil = nullptr;
}; inline Hooks* g_Hooks = new Hooks();