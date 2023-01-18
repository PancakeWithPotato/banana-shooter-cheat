#include "DiscordRPC.h"

void DiscordRPC::Handler()
{
	DiscordEventHandlers handler;
	memset(&handler, 0, sizeof(handler));
	Discord_Initialize("1065319308063948810", &handler, 1, NULL);
}

void DiscordRPC::Update()
{
    DiscordRichPresence discordPresence;
    memset(&discordPresence, 0, sizeof(discordPresence));
    discordPresence.details = "Cheating on banana shooter";
    discordPresence.startTimestamp = std::time(0);
    Discord_UpdatePresence(&discordPresence);
}

void DiscordRPC::ShutDown()
{
    Discord_Shutdown();
}

void DiscordRPC::DiscordThread()
{
    Handler();
    Update();
}
