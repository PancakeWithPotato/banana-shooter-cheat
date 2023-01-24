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
}; inline Visuals g_Visuals;