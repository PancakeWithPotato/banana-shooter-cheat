#pragma once

#include "../../dependencies/kiero/kiero.hpp"
#include "../../dependencies/imgui/imgui.h"
#include "../../dependencies/imgui/imgui_impl_dx11.h"
#include "../../dependencies/imgui/imgui_impl_win32.h"

struct Menu {
	void Render();
	bool open = true; //cmon, you gotta set this true, how else would they notice they injected (let's ignore the console for a second)
}; 

inline Menu g_Menu;