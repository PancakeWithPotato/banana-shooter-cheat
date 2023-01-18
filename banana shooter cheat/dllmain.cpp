#include "utilities/includes.hpp"
#include "core/hack.hpp"
#include "discord/DiscordRPC.h"

void Main(HMODULE hMod)  {
	if (!g_Hack->Setup())
		g_Debug.logState(::ERROR, "Failed to setup!");

	g_Cord.DiscordThread();

	while (!GetAsyncKeyState(VK_END))
		std::this_thread::sleep_for(std::chrono::milliseconds(500));

	g_Hack->shouldUnload = true;
	std::this_thread::sleep_for(std::chrono::milliseconds(1000));

	g_Cord.ShutDown();
	FreeLibraryAndExitThread(hMod, 0); 
}

BOOL WINAPI DllMain(HMODULE hMod, DWORD reason, void* reserved) {
	switch (reason)
	{
	case DLL_PROCESS_ATTACH:
		_Post_ _Notnull_ CloseHandle(CreateThread(nullptr, 0, reinterpret_cast<LPTHREAD_START_ROUTINE>(Main), hMod, 0, nullptr));
		break;
	case DLL_PROCESS_DETACH:
		g_Hack->Destroy();

		delete g_Hack;
		break;
	}

	return TRUE;
}
