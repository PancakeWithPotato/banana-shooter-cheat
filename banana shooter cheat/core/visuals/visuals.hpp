#pragma once
#include "../../utilities/includes.hpp"
#include "../../dependencies/easing-functions/easing.h"
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


struct AnimInfo 
{
	float duration;
	bool state;

	float from;
	float dest;
};
namespace Animations
{
	//the animations
	inline easingFunction easeIn;

	inline easingFunction easeInQuint;
	inline easingFunction easeOutQuint;
	inline easingFunction easeInOutQuart;
	inline easingFunction easeInCirc;

	static void Setup();

	inline double time = 0;

	float TweenHelper(uint64_t id, float duration, bool state, float dt); //thx boblox :D
	float Tween(uint64_t id, const AnimInfo& info);

	inline std::unordered_map<uint64_t, float> values;
}



