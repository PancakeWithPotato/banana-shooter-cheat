#pragma once
#include <cstdint>
#include <Windows.h>
#include "ext/termcolor/termcolor.hpp"
#include "binds.h"


#define SUCCES termcolor::green
#define WAIT termcolor::yellow
#define ERROR termcolor::red 
#define RESET termcolor::reset
#define CHANGE termcolor::cyan
class hack
{
private:
	FILE* m_f; //for out console
	std::string Getname();
	std::string m_strName;
public:
	bool Setup();
	void Destroy();
	//logginsg stuff
	enum eLogs
	{
		succes = 1,
		error,
		warning //this cuold be wait, its gonna be the yellow
	};
	void Log(eLogs, const char* stuff ...);
	uintptr_t pAssembly = 0x0;
}; inline hack* g_Hack = new hack();

