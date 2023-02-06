#include "hack.hpp"
#include "hooks/hooks.hpp"
#include "../utilities/spotify/inject_sound.hpp"
#pragma comment(lib, "winmm.lib")
bool Hack::setup()  
{
	g_Sdk.getAllExports();
	srand(std::time(0));
	g_Debug.setupConsole("banana $hooter cheat");

	HWND consoleHWND = GetConsoleWindow();
	
	SetWindowLong(consoleHWND, GWL_EXSTYLE, GetWindowLong(consoleHWND, GWL_EXSTYLE) | WS_EX_LAYERED);
	SetLayeredWindowAttributes(consoleHWND, 0, 240, LWA_ALPHA);

	getName();

	std::cout << "Welcome, " << CHANGE << username << std::endl << RESET;

	Offsets::pAssembly = reinterpret_cast<uintptr_t>(GetModuleHandleA("GameAssembly.dll"));
	
	if (Offsets::pAssembly)
		g_Debug.logState(SUCCESS, "Got GameAssembly.dll 0x%p", Offsets::pAssembly);
	else
		return false;

	if (!g_Hooks->setup())
		return false;

	g_Funcs->setup();
	g_Config::init();
	g_Lua.setup();
	PlaySoundA(reinterpret_cast<LPCSTR>(injectSound), NULL, SND_ASYNC | SND_MEMORY); //inject sound
	return true;
}

void Hack::getName() {
	char tmp[MAX_PATH + 1];
	DWORD lenght = sizeof(tmp);
	GetUserNameA(tmp, &lenght);
	username = tmp;
}

void Hack::destroy()  {
	g_Hooks->destroy();
	delete g_Hooks;
	delete g_Funcs;
	g_Lua.destroy();
	g_Debug.destroyConsole();
}