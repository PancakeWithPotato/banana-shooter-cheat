#include "config.hpp"
#include <direct.h>
void g_Config::Init()
{
	std::string strLocaluser = (g_Hack->strName == "dev" ? "Pancake" : g_Hack->strName);
	g_Debug.logState(::WARNING, "Welcome, " + strLocaluser + " in the config section!");
	std::string strLocalpath = "C:\\Users\\" + strLocaluser + "\\Documents\\meowware";
	if(!_mkdir(strLocalpath.c_str()))
		g_Debug.logState(::SUCCESS, "Created directory 1!");

	strLocalpath.append("\\banana_shooter");
	if (!_mkdir(strLocalpath.c_str()))
		g_Debug.logState(::SUCCESS, "Created directory 2!");
}
