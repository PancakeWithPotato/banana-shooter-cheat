#pragma once
#include "../sdk/il2cpp.h"
#include <string>
#include <vector>

class Hack
{
private:
	void GetName();
	
public:
	bool Setup();
	void Destroy();
	std::string strName;

	bool shouldUnload = false;

	Multiplayer_Client_ClientPlayer_o* closestPlayer = nullptr;
	Multiplayer_Client_ClientPlayer_o* localPlayer = nullptr;
	std::vector<Multiplayer_Client_ClientPlayer_o*> players;
}; inline Hack* g_Hack = new Hack();

