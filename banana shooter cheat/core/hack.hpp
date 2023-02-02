#pragma once
#include "../utilities/includes.hpp"

class Hack {
private:
	void getName();
	
public:
	bool setup();
	void destroy();
	std::string username;

	bool shouldUnload = false;

	Player* closestPlayer = nullptr;
	Player* localPlayer = nullptr;
	std::unordered_map<unsigned long long, Player*> players;
}; inline Hack* g_Hack = new Hack();

