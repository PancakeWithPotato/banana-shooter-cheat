#include "DiscordRPC.h"

void DiscordRPC::handler()
{
	DiscordEventHandlers handler;
	memset(&handler, 0, sizeof(handler));
	Discord_Initialize("1065319308063948810", &handler, 1, NULL);
}

void DiscordRPC::update()
{
    DiscordRichPresence discordPresence;
    memset(&discordPresence, 0, sizeof(discordPresence));
    discordPresence.details = "Cheating on banana shooter";
    discordPresence.startTimestamp = std::time(0);
    Discord_UpdatePresence(&discordPresence);
}

void DiscordRPC::shutDown()
{
    Discord_Shutdown();
}

void DiscordRPC::discordThread()
{
    handler();
    update();
}
