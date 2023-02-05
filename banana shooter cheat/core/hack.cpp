#include "hack.hpp"
#include "hooks/hooks.hpp"

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
	g_Debug.destroyConsole();
}