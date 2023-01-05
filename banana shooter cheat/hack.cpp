#include "hack.h"
#include "hooks.h"
bool hack::Setup() 
{
	//alloc the console
	AllocConsole();
	freopen_s(&this->m_f, "CONOUT$", "w", stdout); //open filestream

	//style it
	HWND hWnd = GetConsoleWindow();
	system("title banana shooter cheat");
	SetWindowLong(hWnd, GWL_EXSTYLE,
		GetWindowLong(hWnd, GWL_EXSTYLE) | WS_EX_LAYERED);
	SetLayeredWindowAttributes(hWnd, 0, 200, LWA_ALPHA);

	this->m_strName = this->Getname(); //get the user

	std::cout << "Welcome, " << CHANGE << this->m_strName << std::endl << RESET; //welcome the user

	this->pAssembly = reinterpret_cast<uintptr_t>(GetModuleHandleA("GameAssembly.dll")); //get base addr of gameassembly dll
	if (this->pAssembly)
		this->Log(hack::succes, "Got GameAssembly.dll @ ", this->pAssembly);
	else
		return false;

	if(!g_Hooks->Setup())
		return false;

	g_Funcs->Setup();
	return true;
}

void hack::Log(eLogs level, const char* data ...) 
{
#ifdef _DEBUG
	switch (level)
	{
	case hack::succes:
		std::cout << SUCCES << "[+] " << data << RESET << std::endl;
		break;
	case hack::error:
		std::cout << ERROR << "[-] " << data << RESET << std::endl;
		break;
	case hack::warning:
		std::cout << WAIT << "[*] " << data << RESET << std::endl;
		break;
	}
#endif
}

std::string hack::Getname() 
{
	char tmp[MAX_PATH + 1];
	DWORD lenght = sizeof(tmp);
	GetUserNameA(tmp, &lenght); //get the name
	return (strcmp("Pancake", tmp) ? "dev" : std::string(&tmp[0])); //return as string, if it isn't Pancake
}

void hack::Destroy() 
{
	std::cout << "Goodbye!\n";
	//remove console
	fclose(this->m_f);
	FreeConsole();
	//destroy hooks
	g_Hooks->Destroy();
	//free the heap allocated instnace
	delete g_Hooks;
}