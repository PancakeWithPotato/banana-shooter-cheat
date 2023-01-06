#include "utilities/includes.hpp"
#include "core/hack.hpp"

void Main(HMODULE hMod) 
{
	if (!g_Hack->Setup())
		g_Debug.logState(::error, "Failed to setup!");

	while (!GetAsyncKeyState(VK_END) & 1)
	{
		if (GetAsyncKeyState(VK_F1) & 1) {
			g_Config.NoRecoil = !g_Config.NoRecoil;
			std::cout << CHANGE << "Toggled norecoil " << (g_Config.NoRecoil ? "ON" : "OFF") << std::endl;
		}
			
		std::this_thread::sleep_for(std::chrono::milliseconds(500)); // pancake: https://stackoverflow.com/questions/49071285/sleep-vs-sleep-for
	}

	FreeLibraryAndExitThread(hMod, 0); 
}

BOOL WINAPI DllMain(HMODULE hMod, DWORD reason, void* reserved)
{
	switch (reason)
	{
	case DLL_PROCESS_ATTACH:
		_Post_ _Notnull_ CloseHandle(CreateThread(nullptr, 0, reinterpret_cast<LPTHREAD_START_ROUTINE>(Main), hMod, 0, nullptr));
		break;
	case DLL_PROCESS_DETACH:
		g_Hack->Destroy();
		g_Debug.DestroyConsole();
 
		delete g_Hack;
		break;
	}

	return TRUE;
}
