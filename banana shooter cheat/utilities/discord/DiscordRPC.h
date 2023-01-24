#pragma once
#include "include/discord_register.h"
#include "include/discord_rpc.h"
#include <Windows.h>
#include <iostream>

#ifdef _DEBUG
#pragma comment(lib, "discord_debug.lib")
#else
#pragma comment(lib, "discord_release.lib")
#endif

class DiscordRPC
{
private:
	void handler();
	void update();

public:
	void discordThread();
	void shutDown();
}; inline DiscordRPC g_Cord;

