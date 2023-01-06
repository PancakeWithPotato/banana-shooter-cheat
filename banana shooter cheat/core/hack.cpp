#include "hack.hpp"
#include "hooks/hooks.hpp"

bool Hack::Setup() 
{
	g_Debug.setupConsole("banana $hooter cheat");

	HWND consoleHWND = GetConsoleWindow();
	
	SetWindowLong(consoleHWND, GWL_EXSTYLE, GetWindowLong(consoleHWND, GWL_EXSTYLE) | WS_EX_LAYERED);
	SetLayeredWindowAttributes(consoleHWND, 0, 225, LWA_ALPHA);

	this->GetName();

	std::cout << "Welcome, " << CHANGE << this->m_strName << std::endl << RESET;

	Offsets::pAssembly = reinterpret_cast<uintptr_t>(GetModuleHandleA("GameAssembly.dll"));
	
	if (Offsets::pAssembly)
		g_Debug.logState(success, "Got GameAssembly.dll %p", (void*)&Offsets::pAssembly);
	else
		return false;

	if(!g_Hooks->Setup())
		return false;

	g_Funcs->Setup();
	return true;
}

void Hack::GetName()
{
	char tmp[MAX_PATH + 1];
	DWORD lenght = sizeof(tmp);
	GetUserNameA(tmp, &lenght);

	m_strName = (strcmp("Pancake", tmp) ? "dev" : std::string(&tmp[0]));
}

void Hack::Destroy() 
{
	g_Hooks->Destroy();
	delete g_Hooks;
}