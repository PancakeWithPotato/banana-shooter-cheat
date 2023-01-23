#include "visuals.hpp"

void Visuals::Init()
{
	if (bInit)
		return;

	this->draw = ImGui::GetBackgroundDrawList();
	this->v2ScreenSize = ImGui::GetIO().DisplaySize;
}

void Visuals::RenderSpotifyStatus()
{
	float fTextX = ImGui::CalcTextSize(g_Spotify.strComplete.c_str()).x;
	ImVec2 v2RenderPos = { (this->v2ScreenSize.x * 0.9877777f) - fTextX ,50.f };
	draw->AddText(v2RenderPos, ImColor(255, 255, 255, 255), g_Spotify.strComplete.c_str());
}
