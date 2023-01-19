#pragma once
#include "../../sdk/il2cpp.h"
#include "../hooks/hooks.hpp"
#include "../../sdk/sdk.hpp"

class combat
{
public:
	Player* ClosestPlayer(std::unordered_map<unsigned long long, Multiplayer_Client_ClientPlayer_o*>& );

	void Aimbot(void* this2, Player* player, const bool& bExplosive, const int& iHittarget);

	void ExplosiveBullet();

	void BulletMultiplier(const int& iBulletcount);


}; inline combat g_Combat;

