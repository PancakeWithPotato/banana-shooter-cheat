#include "hooks.hpp"

bool Hooks::AddHook(std::string hookName, unsigned long long pTarget, void* detour, void* original)  {
	if (MH_CreateHook(reinterpret_cast<LPVOID>(pTarget), reinterpret_cast<LPVOID>(detour), reinterpret_cast<LPVOID*>(original)) != MH_OK) {
		g_Debug.logState(error, "Could not hook: " + hookName);
		return false;
	}

	if (original == nullptr) {
		g_Debug.logState(warning, "Original null (forgot to ref?): " + hookName);
		return false;
	}

	g_Debug.logState(success, "Hooked: " + hookName);
	return true;
}

bool Hooks::Setup()  {
	if (MH_Initialize() != MH_OK) {
		g_Debug.logState(error, "Could not initialize MinHook");
		return false;
	}

	if (!AddHook("RecoilFire", (Offsets::pAssembly + Offsets::Recoil::RecoilFir), &hRecoilFir, &oRecoil))
		return false;

	if (!AddHook("DoAttack", (Offsets::pAssembly + Offsets::Firearms::DoAttack), &hDoAttack, &oDoAttack))
		return false;

	if (!AddHook("UpdatePlayer", (Offsets::pAssembly + Offsets::Player::Update), &hUpdatePlayer, &oUpdatePlayer))
		return false;

	if (!AddHook("ReloadGun", (Offsets::pAssembly + Offsets::Firearms::Reload), &hReloadGun, &oReloadGun))
		return false;

	if (MH_EnableHook(MH_ALL_HOOKS) != MH_OK) {
		g_Debug.logState(error, "Could not enable hooks");
		return false;
	}

	return true;
}

void Hooks::Destroy()  {
	MH_DisableHook(MH_ALL_HOOKS);
	MH_RemoveHook(MH_ALL_HOOKS);
	MH_Uninitialize();
}

void __stdcall Hooks::hRecoilFir(Firearms_o* thisptr, float x, float y, float z) {
	return g_Hooks->oRecoil(thisptr, 0.f, 0.f, 0.f);
}

void __stdcall Hooks::hDoAttack(Firearms_o* thisptr) {
	thisptr->fields.bulletCount = 5;
	thisptr->fields.createBullet = true;

	Vector3 aimPos = g_Sdk.getTransformPosition(g_Hack->localPlayer->fields.aimTarget);
	g_Funcs->pCreateExplosiveBullet(thisptr, aimPos);

	return g_Hooks->oDoAttack(thisptr);
}

void __stdcall Hooks::hReloadGun(Firearms_o* thisptr, float time, int spin) {
	thisptr->fields.reloadTime = 0.0f;

	return g_Hooks->oReloadGun(thisptr, 0.f, 0);
}

void __stdcall Hooks::hUpdatePlayer(Player* player) {
	if (player->fields._IsLocal_k__BackingField)
		g_Hack->localPlayer = player;

	return g_Hooks->oUpdatePlayer(player);
}
