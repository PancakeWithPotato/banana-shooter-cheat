#pragma once

#include "../../dependencies/kiero/kiero.hpp"
#include "../../dependencies/imgui/imgui.h"
#include "../../dependencies/imgui/imgui_impl_dx11.h"
#include "../../dependencies/imgui/imgui_impl_win32.h"
#include <string>
 
struct Menu {
private:
	void FormatDate(std::string& date, const std::string& rawdate);
	enum TABS {
		TAB_CONFIG,
		TAB_COMBAT,
		TAB_VISUALS
	};
	TABS TabCount = TAB_CONFIG;
	bool bInit = false;
	//tabs renderables
	void RenderConfig();
	void RenderCombat();
	void RenderVisuals();
public:
	void Render();
	bool open = true; //menu open by default
}; 

inline Menu g_Menu;