#include "combat.h"

Player* combat::ClosestPlayer(std::unordered_map<unsigned long long, Multiplayer_Client_ClientPlayer_o*>& players)
{
	float bestDistance = FLT_MAX;

	for (auto& [steamID, player] : players)
	{
		if (!player) {
			players.erase(steamID);
			continue;
		}

		if (g_Hack->localPlayer != nullptr && g_Hack->localPlayer->fields.health > 0 && player->fields.health > 0) 
		{ 
			//if local and enemy alive, get their pos
			Vector3 local = g_Sdk.getTransformPosition(g_Hack->localPlayer->fields.head);
			Vector3 enemy = g_Sdk.getTransformPosition(player->fields.head);

			//get distance between them
			float distance = g_Sdk.getDistance(local, enemy);

			//if distance is better than bestdistance, change bestdistance and return player*
			if (distance < bestDistance) {
				bestDistance = distance;
				return player;
			}
			
		}
	}

	return nullptr;

	//for (auto& [steamID, player] : g_Hack->players) {
	//	if (!player) {
	//		g_Hack->players.erase(steamID);
	//		continue;
	//	}

	//	if (g_Hack->localPlayer != nullptr && g_Hack->localPlayer->fields.health > 0 && player->fields.health > 0) {
	//		Vector3 localPos = g_Sdk.getTransformPosition(g_Hack->localPlayer->fields.head);
	//		Vector3 enemyPos = g_Sdk.getTransformPosition(player->fields.head);

	//		float distance = g_Sdk.getDistance(localPos, enemyPos);
	//		if (distance < bestDistance) {
	//			g_Hack->closestPlayer = player;
	//			bestDistance = distance;
	//		}
	//	}
	//}
}

void combat::Aimbot(void* this2, Player* player, const bool& bExplosive, const int& iHittarget)
{
	if (player == nullptr)
		return;

	Vector3 aimPos = player->fields.desiredPos;

	switch (iHittarget)
	{
	case 0:
		//set aimpos to head
		aimPos = g_Sdk.getTransformPosition(player->fields.head);
		if (bExplosive)
			g_Funcs->pCreateExplosiveBullet(this2, aimPos);
		else
			g_Funcs->pCreateBullet(this2, aimPos);
		break;
	case 1:
		if (bExplosive)
			g_Funcs->pCreateExplosiveBullet(this2, aimPos);
		else
			g_Funcs->pCreateBullet(this2, aimPos);
		break;
	}
}
