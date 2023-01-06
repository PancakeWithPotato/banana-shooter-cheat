#pragma once
#include "../sdk/il2cpp.h"
#include <string>

class Hack
{
private:
	void GetName();
	std::string m_strName;
public:
	bool Setup();
	void Destroy();

	bool shouldUnload = false;

	Multiplayer_Client_ClientPlayer_o* localPlayer = nullptr; // weird compiler error
}; inline Hack* g_Hack = new Hack();

