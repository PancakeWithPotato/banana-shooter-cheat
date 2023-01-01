#include "includes.h"
#include "hooks.h"
void Main(HMODULE hMod) 
{
	if (!g_Hack->Setup()) //set up cheat
		g_Hack->Log(hack::error, "Failed to setup!");
	while (!GetAsyncKeyState(VK_END) & 1) //runs till we press end
	{
		//toggle binds (until we got no gui lol)
		if (GetAsyncKeyState(VK_F1) & 1) {
			Binds::bRecoil = !Binds::bRecoil;
			std::cout << CHANGE << "Toggled norecoil " << (Binds::bRecoil ? "ON" : "OFF") << std::endl;
		}
			
		Sleep(500);
	}
	FreeLibraryAndExitThread(hMod, 0);
}

BOOL WINAPI DllMain(HMODULE hMod, DWORD reason, void* reserved)
{

	switch (reason)
	{
	case DLL_PROCESS_ATTACH:
		//create the thread, close the handle
		_Post_ _Notnull_ CloseHandle(CreateThread(nullptr, 0, reinterpret_cast<LPTHREAD_START_ROUTINE>(Main), hMod, 0, nullptr));
		break;
	case DLL_PROCESS_DETACH:
		g_Hack->Destroy(); //"destroy" hack
		//free the heap allocated instnace
		delete g_Hack;
		break;
	}

	return TRUE;
}
