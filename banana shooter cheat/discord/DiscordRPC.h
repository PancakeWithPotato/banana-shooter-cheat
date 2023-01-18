#pragma once
#include "include/discord_register.h"
#include "include/discord_rpc.h"
#include <Windows.h>
#include <iostream>
#pragma comment(lib, "discord.lib")
//credits: https://www.unknowncheats.me/forum/general-programming-and-reversing/361227-adding-discord-rich-presence-cheat.html

class DiscordRPC
{
private:
	void Handler();
	void Update();

public:
	void DiscordThread();
	void ShutDown();
}; inline DiscordRPC g_Cord;

