#pragma once
#include "include/discord_register.h"
#include "include/discord_rpc.h"
#include <Windows.h>
#include <iostream>
//credits: https://www.unknowncheats.me/forum/general-programming-and-reversing/361227-adding-discord-rich-presence-cheat.html

#ifdef _DEBUG
#pragma comment(lib, "discord_debug.lib")
#else
#pragma comment(lib, "discord_release.lib")
#endif

class CDiscordRPC
{
private:
	void Handler();
	void Update();

public:
	void Initalize();
	void Shutdown();
}; inline CDiscordRPC g_Cord;

