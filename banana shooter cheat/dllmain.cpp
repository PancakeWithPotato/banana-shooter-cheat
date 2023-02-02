#include "utilities/includes.hpp"
#include "core/hack.hpp"

#include "utilities/discord/DiscordRPC.h"
#include "utilities/spotify/spotify.hpp"

void Main(HMODULE hMod)  {
	if (!g_Hack->setup()) 
		g_Debug.logState(::ERROR, "Failed to setup!");

	g_Cord.discordThread();

	while (!GetAsyncKeyState(VK_END)) 
		std::this_thread::sleep_for(std::chrono::milliseconds(500));

	g_Hack->shouldUnload = true;
	std::this_thread::sleep_for(std::chrono::milliseconds(1000));

	g_Cord.shutDown();
	FreeLibraryAndExitThread(hMod, 0); 
}

void SpotifyUpdate() {
	while (!g_Hack->shouldUnload) 
		g_Spotify.Update();

	ExitThread(0);
}

BOOL WINAPI DllMain(HMODULE hMod, DWORD reason, void* reserved) {
	switch (reason)
	{
	case DLL_PROCESS_ATTACH:
		_Post_ _Notnull_ CloseHandle(CreateThread(nullptr, 0, reinterpret_cast<LPTHREAD_START_ROUTINE>(Main), hMod, 0, nullptr));
		_Post_ _Notnull_ CloseHandle(CreateThread(0, 0, reinterpret_cast<LPTHREAD_START_ROUTINE>(SpotifyUpdate), NULL, 0, 0));
		break;
	case DLL_PROCESS_DETACH:
		g_Hack->destroy();
		delete g_Hack;
		break;
	}

	return TRUE;
}
