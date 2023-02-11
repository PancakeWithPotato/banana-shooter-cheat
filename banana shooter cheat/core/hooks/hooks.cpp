#include "hooks.hpp"

#include "../menu/menu.hpp"
#include "../config/config.hpp"

#include "../../dependencies/minhook/MinHook.h"
#include "../combat/combat.hpp"

bool Hooks::addHook(std::string hookName, unsigned long long pTarget, void* detour, void* original)  {
	iHooks[0]++;
	if (MH_CreateHook(reinterpret_cast<LPVOID>(pTarget), reinterpret_cast<LPVOID>(detour), reinterpret_cast<LPVOID*>(original)) != MH_OK) {
		g_Debug.logState(ERROR, "Could not hook %d: %s", iHooks[0], hookName.c_str());
		return false;
	}

	g_Debug.logState(SUCCESS, "Hooked %d: %s", iHooks[0], hookName.c_str());
	iHooks[1]++;
	return true;
}

bool Hooks::setup()  {
	if (MH_Initialize() != MH_OK) {
		g_Debug.logState(ERROR, "Could not initialize MinHook");
		return false;
	}

	addHook("RecoilFire", (Offsets::pAssembly + Offsets::Recoil::RecoilFir), &hRecoilFir, &oRecoil);
	addHook("DoAttack", (Offsets::pAssembly + Offsets::Firearms::DoAttack), &hDoAttack, &oDoAttack);
	addHook("UpdatePlayer", (Offsets::pAssembly + Offsets::Player::Update), &hUpdatePlayer, &oUpdatePlayer);
	addHook("ReloadGun", (Offsets::pAssembly + Offsets::Firearms::Reload), &hReloadGun, &oReloadGun);
	addHook("FirearmsUpdate", (Offsets::pAssembly + Offsets::Firearms::Update), &hFirearmsUpdate, &oFirearmsUpdate);
	addHook("AntiCheatUpdate", (Offsets::pAssembly + Offsets::AntiCheat::Update), &hUpdateAntiCheat, nullptr); // we dont call the original
	addHook("BulletInitialization", (Offsets::pAssembly + Offsets::Bullet::BulletInitialization), &hBulletInitialization, &oBulletInitialization);
	addHook("ChatUpdate", (Offsets::pAssembly + Offsets::Chat::Update), &hChatUpdate, &oChatUpdate);

	g_Debug.logState((iHooks[1] / iHooks[0] > 0.50f ? SUCCESS : ERROR), "Managed to hook %d functions out of %d", iHooks[1], iHooks[0]);

	if (kiero::init(kiero::RenderType::D3D11) != 0) {
		g_Debug.logState(ERROR, "Could not initialize Kiero");
		return false;
	}

	if (kiero::bind(8, (void**)&oPresent, &hPresent) != 0) {
		g_Debug.logState(ERROR, "Could not hook Present");
		return false;
	}

	if (MH_EnableHook(MH_ALL_HOOKS) != MH_OK) {
		g_Debug.logState(ERROR, "Could not enable hooks");
		return false;
	}

	return true;
}

void Hooks::destroy() {
	MH_DisableHook(MH_ALL_HOOKS);
	MH_RemoveHook(MH_ALL_HOOKS);
	MH_Uninitialize();
}

void __stdcall Hooks::hChatUpdate(Chat_o* self)  {
	UnityEngine_Color_o color = { 255,0,0,255};
	if (g_Funcs->pAddMessage) {
		if (GetAsyncKeyState(VK_F1) & 1)
			g_Funcs->pAddMessage(self, "Get this cheat at https://github.com/PancakeWithPotato/banana-shooter-cheat\n", color);
	}
	return g_Hooks->oChatUpdate(self);
}

void __stdcall Hooks::hFirearmsUpdate(Firearms_o* thisptr) {
	g_Sdk.localCamera = thisptr->fields.PlayerCam;

	thisptr->fields.bobSpeed = g_Config::get<float>("visuals,bob_speed,f");

	return g_Hooks->oFirearmsUpdate(thisptr);
}

void __stdcall Hooks::hRecoilFir(void* thisptr, float x, float y, float z) {
	if (g_Config::get<bool>("combat,norecoil,b"))
		return g_Hooks->oRecoil(thisptr, 0.f, 0.f, 0.f);

	return g_Hooks->oRecoil(thisptr, x, y, z);
}

void __stdcall Hooks::hDoAttack(Firearms_o* thisptr)  {
	static Player* player;
	static Vector3 aimPos;
	aimPos = g_Sdk.getTransformPosition(g_Hack->localPlayer->fields.aimTarget);

	if (g_Config::get<int>("combat,bullet_count,i"))
		g_Combat.bulletMultiplier(thisptr, g_Config::get<int>("combat,bullet_count,i"));
	thisptr->fields.damage = INT_MAX;
	player = g_Combat.closestPlayer(g_Hack->players, true);
	for (auto& i : g_Lua.luas)
	{
		if (i.state || i.attackUpdateCallbacks.empty())
			continue;
		for (auto& k : i.attackUpdateCallbacks)
		{
			lua_getglobal(i.state, k.second);
			auto stuff = lua_pcall(i.state, 0, 0, 0);
			if (stuff != LUA_OK) {
				std::string error = lua_tostring(i.state, -1);
				std::cout << error << std::endl;
			}
		}
	}

	if (g_Config::get<bool>("combat,aimbot_enabled,b")) 
		g_Combat.aimbot(thisptr, player, g_Config::get<bool>("combat,explosive_bullets,b"), g_Config::get<int>("combat,aimbot_target,i"));

	if (g_Config::get<bool>("combat,explosive_bullets,b"))
		g_Funcs->pCreateExplosiveBullet(thisptr, aimPos);

	return g_Hooks->oDoAttack(thisptr);
}

void __stdcall Hooks::hReloadGun(Firearms_o* thisptr, float time, int spin) {
	if (!g_Config::get<bool>("combat,noreload,b"))
		return g_Hooks->oReloadGun(thisptr, time, spin);
	
	thisptr->fields.reloadTime = 0.0f;
	return g_Hooks->oReloadGun(thisptr, 0.f, 100);
}

void __stdcall Hooks::hUpdatePlayer(Player* player) {
	g_Sdk.networkManager = g_Sdk.getNetworkManager();

	if (player->fields._IsLocal_k__BackingField)
		g_Hack->localPlayer = player;
	else {
		if (g_Hack->players.find(player->fields._SteamId_k__BackingField) == g_Hack->players.end())
			g_Hack->players.insert({ player->fields._SteamId_k__BackingField, player });
	}

	for (auto& i : g_Lua.luas)
	{
		if (i.state || i.renderCallbacks.empty())
			continue;
		for (auto& k : i.renderCallbacks)
		{
			lua_getglobal(i.state, k.second);
			auto stuff = lua_pcall(i.state, 0, 0, 0);
			if (stuff != LUA_OK) {
				std::string error = lua_tostring(i.state, -1);
				std::cout << error << std::endl;
			}
		}
	}

	return g_Hooks->oUpdatePlayer(player);
}

void __stdcall Hooks::hUpdateAntiCheat(Manager_AntiCheatDectect_o* ptr) {
	ptr->klass->static_fields->banned = false;
	return;
}

void __stdcall Hooks::hBulletInitialization(Bullet_o* bullet, Vector3 dir, float speed, int damage, void* layermask, bool local, bool useGravity) {
	if (local) {
		return g_Hooks->oBulletInitialization(bullet, dir, 9999.f, 9999.f, layermask, local, false);
	}

	return g_Hooks->oBulletInitialization(bullet, dir, speed, damage, layermask, local, useGravity);
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


			ImFontConfig RoBoToFoNtCoNfIG;
			RoBoToFoNtCoNfIG.RasterizerFlags = ImGuiFreeType::ForceAutoHint;
			g_Menu.smallest7 = io.Fonts->AddFontFromMemoryTTF(pixel7::font, pixel7::size, 14, &RoBoToFoNtCoNfIG);
			g_Menu.helveticaNormal = io.Fonts->AddFontFromMemoryTTF(Helvetica::fontNormal, Helvetica::normalSize, 15, &RoBoToFoNtCoNfIG);
			g_Menu.robotoFont = io.Fonts->AddFontFromMemoryTTF(Roboto::font, Roboto::size, 15, &RoBoToFoNtCoNfIG, io.Fonts->GetGlyphRangesDefault());

			g_Menu.monsterratFont = io.Fonts->AddFontFromMemoryTTF(Monsterrat::font, Monsterrat::size, 17);

			g_Menu.verdanaFont = io.Fonts->AddFontFromMemoryTTF(Verdana::font, Verdana::size, 12);
			g_Menu.helveticaLight = io.Fonts->AddFontFromMemoryTTF(Helvetica::fontLight, Helvetica::lightSize, 15, &RoBoToFoNtCoNfIG, io.Fonts->GetGlyphRangesDefault());
			

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

	g_Menu.render();

	if (g_Config::get<bool>("misc,spotify,b"))
		g_Visuals.renderSpotifyStatus();
	if (g_Config::get<bool>("visuals,enemy_box,b"))
		g_Visuals.renderEnemyBoxes({ 100,100 }, { 200,200 }, g_Config::get<ImVec4>("visuals,enemy_box_color,c"));

	g_Visuals.renderText("this is from c++", { 500,500 }, { 0,0,0,1 });
	for (auto& i : g_Lua.luas)
	{
		if (!i.state || i.renderCallbacks.empty())
			continue;
		for (auto& k : i.renderCallbacks)
		{
			lua_getglobal(i.state, k.second);
			auto stuff = lua_pcall(i.state, 0, 0, 0);
			if (stuff != LUA_OK) {
				std::string error = lua_tostring(i.state, -1);
				std::cout << error << std::endl;
			}
		}
	}

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