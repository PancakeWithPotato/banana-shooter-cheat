#include "combat.hpp"

Player* Combat::closestPlayer(std::unordered_map<unsigned long long, Player*>& playerList, bool skipIfTeammate) {
	float bestDistance = FLT_MAX;
	Player* bestPlayer = nullptr;

	for (auto& [steamID, player] : playerList)
	{
		if (!player) {
			playerList.erase(steamID);
			continue;
		}

		if (g_Hack->localPlayer != nullptr && g_Hack->localPlayer->isAlive() && (player->isEnemyWith(g_Hack->localPlayer) && skipIfTeammate && player->isValid())) {
			/*Vector3 local = g_Sdk.getTransformPosition(g_Hack->localPlayer->fields.head);
			Vector3 enemy = g_Sdk.getTransformPosition(player->fields.head);*/

			Vector3 local = g_Hack->localPlayer->fields.desiredPos;
			Vector3 enemy = player->fields.desiredPos;

			float distance = g_Sdk.getDistance(local, enemy);

			if (distance < bestDistance) {
				bestDistance = distance;
				bestPlayer = player;
			}
		}
	}

	return bestPlayer;
}

void Combat::aimbot(Firearms_o* self, Player* player, const bool& bExplosive, const int& iHittarget)
{
	if (player == nullptr || !player->isValid())
		return g_Hooks->oDoAttack(self);

	Vector3 aimPos = player->fields.desiredPos;

	switch (iHittarget)
	{
	case 0:
		aimPos = g_Sdk.getTransformPosition(player->fields.head);
		std::cout << std::format("AIMPOS: {}, {}, {}\n", aimPos.fields.x, aimPos.fields.y, aimPos.fields.z);
		if (bExplosive)
			g_Funcs->pCreateExplosiveBullet(self, aimPos);
		else
			g_Funcs->pCreateBullet(self, aimPos);
		break;
	case 1:
		if (bExplosive)
			g_Funcs->pCreateExplosiveBullet(self, aimPos);
		else
			g_Funcs->pCreateBullet(self, aimPos);
		break;
	}

}

void Combat::bulletMultiplier(Firearms_o* self, const int& iBulletcount)
{
	self->fields.bulletCount = iBulletcount;
}

