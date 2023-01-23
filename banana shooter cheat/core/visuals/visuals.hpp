#pragma once
#include "../../utilities/Song/Song.hpp"
#include "../../utilities/includes.hpp"

class Visuals 
{
private:
	bool bInit = false;
public:
	ImDrawList* draw; // = ImGui::GetBackgroundDrawList();
	ImVec2 v2ScreenSize; //= ImGui::GetIO().DisplaySize;

	void Init();
	void RenderSpotifyStatus();
}; inline Visuals g_Visuals;