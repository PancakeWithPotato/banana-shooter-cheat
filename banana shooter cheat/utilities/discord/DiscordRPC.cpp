#include "DiscordRPC.h"

void CDiscordRPC::Handler()
{
	DiscordEventHandlers handler;
	memset(&handler, 0, sizeof(handler));
	Discord_Initialize("1065319308063948810", &handler, 1, NULL);
}

void CDiscordRPC::Update()
{
    DiscordRichPresence discordPresence;
    memset(&discordPresence, 0, sizeof(discordPresence));
    discordPresence.details = "Cheating on banana shooter";
    discordPresence.largeImageKey = "meowware_cover2";
    discordPresence.startTimestamp = std::time(0);
    discordPresence.state = "Being silly :P";
    Discord_UpdatePresence(&discordPresence);
}

void CDiscordRPC::Shutdown()
{
    Discord_Shutdown();
}

void CDiscordRPC::Initalize()
{
    Handler();
    Update();
}
