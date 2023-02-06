#pragma once
#include "../../utilities/includes.hpp"

class Visuals 
{
private:
	bool bInit = false;
public:
	ImDrawList* draw;
	ImVec2 v2ScreenSize;

	void init();
	void renderSpotifyStatus();
	void renderEnemyBoxes(const ImVec2& pos, const ImVec2& pos2, const ImVec4& color);

	//some stuff for lua calling
	void renderText(const char* text, const ImVec2& pos, const ImVec4& color);
}; inline Visuals g_Visuals;