#pragma once

#include "../../dependencies/kiero/kiero.hpp"
#include "../../dependencies/imgui/imgui.h"
#include "../../dependencies/imgui/imgui_impl_dx11.h"
#include "../../dependencies/imgui/imgui_impl_win32.h"

struct g_Menu {
	void Render();
	bool open = false;
}; 

inline g_Menu g_Menu;