#include "hooks.hpp"

#include "../menu/menu.hpp"
#include "../config/config.hpp"

bool Hooks::AddHook(std::string hookName, unsigned long long pTarget, void* detour, void* original)  {
	this->iHooks[0]++;
	if (MH_CreateHook(reinterpret_cast<LPVOID>(pTarget), reinterpret_cast<LPVOID>(detour), reinterpret_cast<LPVOID*>(original)) != MH_OK) {
		g_Debug.logState(error, "Could not hook %d: %s", this->iHooks[0], hookName.c_str());
		return false;
	}

	g_Debug.logState(success, "Hooked %d: %s", this->iHooks[0], hookName.c_str());
	this->iHooks[1]++;
	return true;
}

bool Hooks::Setup()  {
	if (MH_Initialize() != MH_OK) {
		g_Debug.logState(error, "Could not initialize MinHook");
		return false;
	}

	AddHook("RecoilFire", (Offsets::pAssembly + Offsets::Recoil::RecoilFir), &hRecoilFir, &oRecoil);
	AddHook("DoAttack", (Offsets::pAssembly + Offsets::Firearms::DoAttack), &hDoAttack, &oDoAttack);
	AddHook("UpdatePlayer", (Offsets::pAssembly + Offsets::Player::Update), &hUpdatePlayer, &oUpdatePlayer);
	AddHook("ReloadGun", (Offsets::pAssembly + Offsets::Firearms::Reload), &hReloadGun, &oReloadGun);
	AddHook("FirearmsUpdate", (Offsets::pAssembly + Offsets::Firearms::Update), &hFirearmsUpdate, &oFirearmsUpdate);
	AddHook("AntiCheatUpdate", (Offsets::pAssembly + Offsets::AntiCheat::Update), &hUpdateAntiCheat, nullptr); // we dont need the original

	std::cout << (this->iHooks[1] / this->iHooks[0] > 0.50f ? SUCCES : ERR) << std::format("Managed to hook {} functions out of {} \n", this->iHooks[1], this->iHooks[0]);

	if (kiero::init(kiero::RenderType::D3D11) != 0) {
		g_Debug.logState(error, "Could not initialize Kiero");
		return false;
	}

	if (kiero::bind(8, (void**)&oPresent, &hPresent) != 0) {
		g_Debug.logState(error, "Could not hook Present");
		return false;
	}

	if (MH_EnableHook(MH_ALL_HOOKS) != MH_OK) {
		g_Debug.logState(error, "Could not enable hooks");
		return false;
	}

	return true;
}

void Hooks::Destroy() {
	MH_DisableHook(MH_ALL_HOOKS);
	MH_RemoveHook(MH_ALL_HOOKS);
	MH_Uninitialize();
}

void __stdcall Hooks::hFirearmsUpdate(Firearms_o* thisptr) {
	g_Sdk.localCamera = thisptr->fields.PlayerCam;

	return g_Hooks->oFirearmsUpdate(thisptr);
}

void __stdcall Hooks::hRecoilFir(void* thisptr, float x, float y, float z) {
	if (g_Config::Combat::NoRecoil)
		return g_Hooks->oRecoil(thisptr, 0.f, 0.f, 0.f);

	return g_Hooks->oRecoil(thisptr, x, y, z);
}

void __stdcall Hooks::hDoAttack(Firearms_o* thisptr) {
	if (g_Config::Combat::Aimbot) {
		if (!g_Hack->closestPlayer)
			return g_Hooks->oDoAttack(thisptr);

		Vector3 aimPos = g_Sdk.getTransformPosition(g_Hack->closestPlayer->fields.head);

		if (g_Config::Combat::ExplosiveBullets)
			g_Funcs->pCreateExplosiveBullet(thisptr, aimPos);
		else
			g_Funcs->pCreateBullet(thisptr, aimPos);
	}
	else {
		Vector3 aimPos = g_Sdk.getTransformPosition(g_Hack->localPlayer->fields.aimTarget);

		if (g_Config::Combat::ExplosiveBullets)
			g_Funcs->pCreateBullet(thisptr, aimPos);
		else
			g_Funcs->pCreateExplosiveBullet(thisptr, aimPos);
	}

	thisptr->fields.bulletCount = g_Config::Combat::BulletsCount;

	return g_Hooks->oDoAttack(thisptr);
}

void __stdcall Hooks::hReloadGun(Firearms_o* thisptr, float time, int spin) {
	if (!g_Config::Combat::NoReload)
		return g_Hooks->oReloadGun(thisptr, time, spin);
	
	thisptr->fields.reloadTime = 0.0f;
	return g_Hooks->oReloadGun(thisptr, 0.f, 0);
}

void __stdcall Hooks::hUpdatePlayer(Player* player) {
	if (player->fields._IsLocal_k__BackingField)
		g_Hack->localPlayer = player;
	else {
		if (!std::count(g_Hack->players.begin(), g_Hack->players.begin(), player))
			g_Hack->players.emplace_back(player);
	}

	float bestDistance = FLT_MAX;
	
	for (int i = 0; i < g_Hack->players.size(); i++) {
		if (!g_Hack->players[i]) {
			g_Hack->players.erase(g_Hack->players.begin() + i);
			continue;
		}

		if (g_Hack->localPlayer != nullptr && g_Hack->localPlayer->fields.health > 0 && player->fields.health > 0) {
			Vector3 localPos = g_Sdk.getTransformPosition(g_Hack->localPlayer->fields.head);
			Vector3 enemyPos = g_Sdk.getTransformPosition(player->fields.head);

			float distance = g_Sdk.getDistance(localPos, enemyPos);
			if (distance < bestDistance) {
				g_Hack->closestPlayer = player;
				bestDistance = distance;
			}
		}
	}

	return g_Hooks->oUpdatePlayer(player);
}

void __stdcall Hooks::hUpdateAntiCheat(Manager_AntiCheatDectect_o* ptr) {
	ptr->klass->static_fields->banned = false;
	return;
}

extern LRESULT ImGui_ImplWin32_WndProcHandler(HWND hWnd, UINT msg, WPARAM wParam, LPARAM lParam);
LRESULT WINAPI Hooks::WndProc(const HWND hWnd, UINT uMsg, WPARAM wParam, LPARAM lParam) {
	if (g_Menu.open && ImGui_ImplWin32_WndProcHandler(hWnd, uMsg, wParam, lParam))
		return 0L;
	
	return CallWindowProcA(g_Hooks->oWndProc, hWnd, uMsg, wParam, lParam);
}

HRESULT Hooks::hPresent(IDXGISwapChain* pSwapChain, UINT SyncInterval, UINT Flags)
{
	static bool init = false;

	if (!init)
	{
		if (SUCCEEDED(pSwapChain->GetDevice(__uuidof(ID3D11Device), (void**)&g_Hooks->pDevice)))
		{
			g_Hooks->pDevice->GetImmediateContext(&g_Hooks->pContext);
			DXGI_SWAP_CHAIN_DESC sd;
			pSwapChain->GetDesc(&sd);
			g_Hooks->window = sd.OutputWindow;
			ID3D11Texture2D* pBackBuffer;
			pSwapChain->GetBuffer(0, __uuidof(ID3D11Texture2D), (LPVOID*)&pBackBuffer);
			g_Hooks->pDevice->CreateRenderTargetView(pBackBuffer, NULL, &g_Hooks->mainRenderTargetView);
			pBackBuffer->Release();
			g_Hooks->oWndProc = (WNDPROC)SetWindowLongPtrW(g_Hooks->window, GWLP_WNDPROC, (LONG_PTR)WndProc);

			ImGui::CreateContext();
			ImGuiIO& io = ImGui::GetIO();
			io.ConfigFlags = ImGuiConfigFlags_NoMouseCursorChange;
			ImGui_ImplWin32_Init(g_Hooks->window);
			ImGui_ImplDX11_Init(g_Hooks->pDevice, g_Hooks->pContext);

			init = true;
		}
		else
			return g_Hooks->oPresent(pSwapChain, SyncInterval, Flags);
	}

	ImGui_ImplDX11_NewFrame();
	ImGui_ImplWin32_NewFrame();
	ImGui::NewFrame();

	g_Menu.Render();

	ImGui::Render();

	g_Hooks->pContext->OMSetRenderTargets(1, &g_Hooks->mainRenderTargetView, NULL);
	ImGui_ImplDX11_RenderDrawData(ImGui::GetDrawData());

	if (g_Hack->shouldUnload) {
		kiero::shutdown();
		g_Hooks->pDevice->Release();
		g_Hooks->pContext->Release();
		pSwapChain->Release();

		if (g_Hooks->oWndProc) {
			SetWindowLongPtrW(g_Hooks->window, GWLP_WNDPROC, (LONG_PTR)g_Hooks->oWndProc);
			g_Hooks->oWndProc = NULL;
		}

		g_Hooks->oPresent(pSwapChain, SyncInterval, Flags);
		return 0;
	}

	return g_Hooks->oPresent(pSwapChain, SyncInterval, Flags);
}