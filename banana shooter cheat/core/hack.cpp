#include "hack.hpp"
#include "hooks/hooks.hpp"

bool Hack::Setup()  {
	srand(std::time(0)); //generate new random seed
	g_Debug.setupConsole("banana $hooter cheat");

	HWND consoleHWND = GetConsoleWindow();
	
	SetWindowLong(consoleHWND, GWL_EXSTYLE, GetWindowLong(consoleHWND, GWL_EXSTYLE) | WS_EX_LAYERED);
	SetLayeredWindowAttributes(consoleHWND, 0, 240, LWA_ALPHA);

	this->GetName();

	std::cout << "Welcome, " << CHANGE << this->strName << std::endl << RESET;

	Offsets::pAssembly = reinterpret_cast<uintptr_t>(GetModuleHandleA("GameAssembly.dll"));
	
	if (Offsets::pAssembly)
		g_Debug.logState(SUCCESS, "Got GameAssembly.dll 0x%p", Offsets::pAssembly);
	else
		return false;

	if (!g_Hooks->Setup())
		return false;

	g_Funcs->Setup();
	g_Config::Base::Init();
	return true;
}

void Hack::GetName() {
	char tmp[MAX_PATH + 1];
	DWORD lenght = sizeof(tmp);
	GetUserNameA(tmp, &lenght);

	strName = (strcmp("Pancake", tmp) == 0 ? "dev" : std::string(&tmp[0]));
}

void Hack::Destroy()  {
	g_Hooks->Destroy();
	delete g_Hooks;
	delete g_Funcs;
	g_Debug.DestroyConsole();
}