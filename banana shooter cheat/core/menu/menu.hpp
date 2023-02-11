#pragma once

#include "../../dependencies/kiero/kiero.hpp"
#include "../../dependencies/imgui/imgui.h"
#include "../../dependencies/imgui/imgui_impl_dx11.h"
#include "../../dependencies/imgui/imgui_impl_win32.h"
#include "../../dependencies/imgui/imgui_stdlib.h"
#include <string>
#include <D3DX11tex.h>
#include "meowware_cover.hpp"
struct Menu {
private:

	void formatDate(std::string& date, const std::string& rawdate);
	enum TABS {
		TAB_MISC,
		TAB_COMBAT,
		TAB_VISUALS
	};
	TABS TabCount = TAB_COMBAT;
	bool bInit = false;
	void renderMisc();
	void renderCombat();
	void renderVisuals();
public:
	ImFont* robotoFont = nullptr;
	ImFont* monsterratFont = nullptr;
	ImFont* verdanaFont = nullptr;

	ImFont* helveticaNormal = nullptr;
	ImFont* helveticaLight = nullptr;

	ImFont* smallest7 = nullptr;
	void render();
	void renderLua();
	bool open = true;
	ID3D11ShaderResourceView* image = nullptr;
}; 

inline Menu g_Menu;