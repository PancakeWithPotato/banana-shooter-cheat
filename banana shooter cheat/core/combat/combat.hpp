#pragma once
#include "../../sdk/il2cpp.h"
#include "../hooks/hooks.hpp"
#include "../../sdk/sdk.hpp"

class Combat
{
public:
	Player* closestPlayer(std::unordered_map<unsigned long long, Player*>& );

	void aimbot(Firearms_o* this2, Player* player, const bool& bExplosive, const int& iHittarget);

	void bulletMultiplier(Firearms_o* self, const int& iBulletcount);
}; inline Combat g_Combat;

