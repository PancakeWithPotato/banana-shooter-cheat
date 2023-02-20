#include "visuals.hpp"
#include "../../utilities/spotify/spotify.hpp"
#include "../menu/menu.hpp"

void Visuals::init() {
	if (bInit)
		return;

	draw = ImGui::GetBackgroundDrawList();
	v2ScreenSize = ImGui::GetIO().DisplaySize;
	Animations::Setup();
	bInit = true;
}

void Animations::Setup()
{
	easeIn = getEasingFunction(EaseInSine);
	easeInQuint = getEasingFunction(EaseInQuint);
	easeOutQuint = getEasingFunction(EaseOutQuint);
	easeInOutQuart = getEasingFunction(EaseInOutQuart);
	easeInCirc = getEasingFunction(EaseInCirc);
}

void Visuals::renderSpotifyStatus() {
	float fTextX = ImGui::CalcTextSize(g_Spotify.strComplete.c_str()).x;
	ImVec2 v2RenderPos = { (v2ScreenSize.x - 30.f) - fTextX, 50.f };
	draw->AddText(v2RenderPos, ImColor(255, 255, 255, 255), g_Spotify.strComplete.c_str());
}
void Visuals::renderEnemyBoxes(const ImVec2& pos, const ImVec2& pos2, const ImVec4& color)
{
	ImGui::PushFont(g_Menu.monsterratFont);
	draw->AddText({ 500,500 }, ImColor{ 0,0,0,255 }, "This is monsterrat");
	ImGui::PopFont();
	ImGui::PushFont(g_Menu.robotoFont);
	draw->AddText({ 500,525 }, ImColor{ 0,0,0,255 }, "This is roboto");
	ImGui::PopFont();
	ImGui::PushFont(g_Menu.verdanaFont);
	draw->AddText({ 500,550 }, ImColor{ 0,0,0,255 }, "This is verdana12");
	ImGui::PopFont();
	ImGui::PushFont(g_Menu.helveticaLight);
	draw->AddText({ 500,570 }, ImColor{ 0,0,0,255 }, "This is helveticalight");
	ImGui::PopFont();
	ImGui::PushFont(g_Menu.helveticaNormal);
	draw->AddText({ 500,600 }, ImColor{ 0,0,0,255 }, "This is helvetica");
	ImGui::PopFont();
	draw->AddRect(pos, pos2, g_Config::convertColor(color));
}

void Visuals::renderText(const char* text, const ImVec2& pos, const ImVec4& color)
{
	draw->AddText(pos, g_Config::convertColor(color), text);
}

void Visuals::renderBox(const ImVec2& min, const ImVec2& max, const ImVec4& color)
{
	draw->AddRect(min, max, g_Config::convertColor(color));
}


float Animations::Tween(uint64_t id, const AnimInfo& info)
{
	float time = TweenHelper(id, info.duration, info.state, Animations::time);
	return (float)Animations::easeOutQuint(time);
}

float Animations::TweenHelper(uint64_t id, float duration, bool state, float dt)
{
	auto value = Animations::values.find(id);

	if (value == values.end())
	{
		values.insert({ id, 0.0f });
		value = values.find(id);
	}

	if (state)
	{
		if (value->second < 1.0f) 
		{
			value->second += (1.0f / duration) * dt; 
		}
	}
	else
	{
		if (value->second > 0.0f) 
		{
			value->second -= (1.0f / duration) * dt;
		}
	}

  	value->second = std::clamp(value->second, 0.0f, 1.0f);

	return value->second;
}
