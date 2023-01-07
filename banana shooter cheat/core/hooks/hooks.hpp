#pragma once
#include "../../utilities/includes.hpp"
#include "../../dependencies/minhook/MinHook.h"

class Hooks 
{
private:
	bool AddHook(std::string hookName, unsigned long long pTarget, void* detour, void* original);
	std::array<int, 2> iHooks = { 0,0 };
public:
	bool Setup();
	void Destroy();

	static void __stdcall hFirearmsUpdate(Firearms_o* thisptr);

	static void __stdcall hRecoilFir(void* thisptr, float x, float y, float z);

	static void __stdcall hDoAttack(Firearms_o* thisptr);
	static void __stdcall hReloadGun(Firearms_o* thisptr, float time, int spin);

	static void __stdcall hUpdatePlayer(Player* player);

	static void __stdcall hUpdateAntiCheat(Manager_AntiCheatDectect_o* ptr);

	HWND window = NULL;
	WNDPROC oWndProc;
	ID3D11Device* pDevice = NULL;
	ID3D11DeviceContext* pContext = NULL;
	ID3D11RenderTargetView* mainRenderTargetView;

	static LRESULT __stdcall WndProc(const HWND hWnd, UINT uMsg, WPARAM wParam, LPARAM lParam);
	static HRESULT __stdcall hPresent(IDXGISwapChain* pSwapChain, UINT SyncInterval, UINT Flags);
private:

	typedef void(__thiscall* RecoilFirFN)(void*, float, float, float);

	typedef void(__thiscall* ReloadGunFN)(Firearms_o*, float, int);
	typedef void(__thiscall* DoAttackFN)(Firearms_o*);
	typedef void(__thiscall* FirearmsUpdateFN)(Firearms_o*);

	typedef unsigned long(__stdcall* PresentFN) (IDXGISwapChain* pSwapChain, UINT SyncInterval, UINT Flags);
	typedef LRESULT(CALLBACK* WNDPROC)(HWND, UINT, WPARAM, LPARAM);

	typedef void(__thiscall* UpdatePlayerFN)(Player*);

	typedef void(__thiscall* UpdateAntiCheatFN)(Manager_AntiCheatDectect_o*);
public:
	RecoilFirFN oRecoil = nullptr;
	DoAttackFN oDoAttack = nullptr;
	ReloadGunFN oReloadGun = nullptr;
	FirearmsUpdateFN oFirearmsUpdate = nullptr;

	UpdatePlayerFN oUpdatePlayer = nullptr;

	UpdateAntiCheatFN oUpdateAntiCheat = nullptr;

	PresentFN oPresent = nullptr;
}; inline Hooks* g_Hooks = new Hooks();