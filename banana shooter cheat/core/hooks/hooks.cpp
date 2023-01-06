#include "hooks.hpp"

#include "../menu/menu.hpp"
#include "../config/config.hpp"

bool Hooks::AddHook(std::string hookName, unsigned long long pTarget, void* detour, void* original)  {
	this->iHooks[0]++;
	if (MH_CreateHook(reinterpret_cast<LPVOID>(pTarget), reinterpret_cast<LPVOID>(detour), reinterpret_cast<LPVOID*>(original)) != MH_OK) {
		g_Debug.logState(error, "Could not hook: " + hookName);
		return false;
	}

	if (original == nullptr) {
		g_Debug.logState(warning, "Original null (forgot to ref?): " + hookName);
		return false;
	}

	g_Debug.logState(success, "Hooked: " + hookName);
	this->iHooks[1]++;
	return true;
}

bool Hooks::Setup()  {
	if (MH_Initialize() != MH_OK) {
		g_Debug.logState(error, "Could not initialize MinHook");
		return false;
	}

	if (!AddHook("RecoilFire", (Offsets::pAssembly + Offsets::Recoil::RecoilFir), &hRecoilFir, &oRecoil))
		g_Debug.logState(error, "Failed to hook #", this->iHooks[0]);

	if (!AddHook("DoAttack", (Offsets::pAssembly + Offsets::Firearms::DoAttack), &hDoAttack, &oDoAttack))
		g_Debug.logState(error, "Failed to hook #", this->iHooks[0]);

	if (!AddHook("UpdatePlayer", (Offsets::pAssembly + Offsets::Player::Update), &hUpdatePlayer, &oUpdatePlayer))
		g_Debug.logState(error, "Failed to hook #", this->iHooks[0]);

	if (!AddHook("ReloadGun", (Offsets::pAssembly + Offsets::Firearms::Reload), &hReloadGun, &oReloadGun))
		g_Debug.logState(error, "Failed to hook #", this->iHooks[0]);

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

void __stdcall Hooks::hRecoilFir(void* thisptr, float x, float y, float z) {

	g_Debug.logState(::warning, "We are in recoilfir");
	if (g_Config::NoRecoil)
		return g_Hooks->oRecoil(thisptr, 0, 0, 0);
	return g_Hooks->oRecoil(thisptr, x, y, z);
}

void __stdcall Hooks::hDoAttack(Firearms_o* thisptr) {
	if (!g_Config::ExplosiveBullets)
		return g_Hooks->oDoAttack(thisptr);

	Vector3 aimPos = g_Sdk.getTransformPosition(g_Hack->localPlayer->fields.aimTarget);
	g_Funcs->pCreateExplosiveBullet(thisptr, aimPos);

	return g_Hooks->oDoAttack(thisptr);
}

void __stdcall Hooks::hReloadGun(Firearms_o* thisptr, float time, int spin) {
	if (!g_Config::NoReload)
		return g_Hooks->oReloadGun(thisptr, time, spin);
	
	thisptr->fields.reloadTime = 0.0f;
	return g_Hooks->oReloadGun(thisptr, 0.f, 0);
}

void __stdcall Hooks::hUpdatePlayer(Player* player) {
	if (player->fields._IsLocal_k__BackingField)
		g_Hack->localPlayer = player;

	return g_Hooks->oUpdatePlayer(player);
}



extern LRESULT ImGui_ImplWin32_WndProcHandler(HWND hWnd, UINT msg, WPARAM wParam, LPARAM lParam);
LRESULT WINAPI Hooks::WndProc(const HWND hWnd, UINT uMsg, WPARAM wParam, LPARAM lParam) {
	if (g_Menu.open)
		if (!ImGui_ImplWin32_WndProcHandler(hWnd, uMsg, wParam, lParam))
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