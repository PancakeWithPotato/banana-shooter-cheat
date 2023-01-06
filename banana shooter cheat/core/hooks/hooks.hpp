#pragma once
#include "../../utilities/includes.hpp"
#include "../../dependencies/minhook/MinHook.h"

class Hooks 
{
private:
	bool AddHook(std::string hookName, unsigned long long pTarget, void* detour, void* original);
public:
	bool Setup();
	void Destroy();

	static void __stdcall hRecoilFir(Firearms_o* thisptr, float x, float y, float z);
	static void __stdcall hDoAttack(Firearms_o* thisptr);
	static void __stdcall hReloadGun(Firearms_o* thisptr, float time, int spin);

	static void __stdcall hUpdatePlayer(Player* player);

private:
	typedef void(__thiscall* RecoilFirFN)(Firearms_o*, float, float, float);
	typedef void(__thiscall* ReloadGunFN)(Firearms_o*, float, int);
	typedef void(__thiscall* DoAttackFN)(Firearms_o*);

	typedef void(__thiscall* UpdatePlayerFN)(Player*);
public:
	RecoilFirFN oRecoil = nullptr;
	DoAttackFN oDoAttack = nullptr;
	ReloadGunFN oReloadGun = nullptr;

	UpdatePlayerFN oUpdatePlayer = nullptr;
}; inline Hooks* g_Hooks = new Hooks();