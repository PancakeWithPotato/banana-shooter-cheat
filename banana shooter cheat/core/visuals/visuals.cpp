#include "visuals.hpp"
#include "../../utilities/spotify/spotify.hpp"

void Visuals::init() {
	if (bInit)
		return;

	draw = ImGui::GetBackgroundDrawList();
	v2ScreenSize = ImGui::GetIO().DisplaySize;

	bInit = true;
}

void Visuals::renderSpotifyStatus() {
	float fTextX = ImGui::CalcTextSize(g_Spotify.strComplete.c_str()).x;
	ImVec2 v2RenderPos = { (v2ScreenSize.x - 30.f) - fTextX, 50.f };
	draw->AddText(v2RenderPos, ImColor(255, 255, 255, 255), g_Spotify.strComplete.c_str());
}
