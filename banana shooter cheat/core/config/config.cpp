#include "config.hpp"
#include <direct.h>

#include "INIREADER.h"

void g_Config::Base::Update(const int& mode)
{
	switch (mode) {
	case 1:
		elements["b:combat:aimbot_enabled"] = g_Config::Combat::Aimbot;
		elements["i:combat:aimbot_target"] = g_Config::Combat::AimbotHitbox;
		elements["b:combat:noreload"] = g_Config::Combat::NoReload;
		elements["b:combat:norecoil"] = g_Config::Combat::NoRecoil;
		break;
	case 2:
		g_Config::Combat::Aimbot = std::get<bool>(elements["b:combat:aimbot_enabled"]);
		g_Config::Combat::AimbotHitbox = std::get<int>(elements["i:combat:aimbot_target"]);
		g_Config::Combat::NoReload = std::get<bool>(elements["b:combat:noreload"]);
		g_Config::Combat::NoRecoil = std::get<bool>(elements["b:combat:norecoil"]);
		break;
	}
	
}

void g_Config::Base::Init()
{
	std::string strLocaluser = (g_Hack->strName == "dev" ? "Pancake" : g_Hack->strName); //works tho
    strBase = "C:\\Users\\" + strLocaluser + "\\Documents\\meowware";
	if(!_mkdir(strBase.c_str()))
		g_Debug.logState(::SUCCESS, "Created directory 1!");

	strBase.append("\\banana_shooter");
	if (!_mkdir(strBase.c_str()))
		g_Debug.logState(::SUCCESS, "Created directory 2!");

	g_Config::Base::strConfigs.reserve(1); //surely at least 1 config exists/will be made

	//set up elements
	elements["b:combat:aimbot_enabled"] = g_Config::Combat::Aimbot;
	elements["i:combat:aimbot_target"] = g_Config::Combat::AimbotHitbox;
	elements["b:combat:noreload"] = false;
	elements["b:combat:norecoil"] = false;
}

void g_Config::Base::Save(const std::string& strName) 
{
	std::ofstream file;
	file.open(g_Config::Base::strBase + "\\" + strName);
	if (!file.is_open())
		return;

	Update(1);
	g_Debug.logState(::SUCCESS, "Succesfully opened config " + strName);

	//thx @stephan
	static auto split = [](const std::string& s, char delim) {
		std::vector<std::string> result;
		std::stringstream ss(s);
		std::string item;
	
		while (getline(ss, item, delim))
			result.push_back(item);
	
		return result;
	};
	std::string strCurrentSection = "";

	for (auto& i : g_Config::Base::elements) 
	{
		std::vector<std::string> strSplitted = split(i.first, ':');
		if (strCurrentSection == "" || strCurrentSection != strSplitted[1]) 
		{
			strCurrentSection = strSplitted[1];
			file << "[" << strCurrentSection << "]\n";
		}
		switch (strSplitted[0][0])
		{
		case 'i':
			file << strSplitted[2] << " = " << std::to_string(std::get<int>(i.second)) << "\n";
			break;
		case 'f':
			file << strSplitted[2] << " = " << std::to_string(std::get<float>(i.second)) << "\n";
			break;
		case 'b':
			file << strSplitted[2] << " = " << std::to_string(std::get<bool>(i.second)) << "\n";
			break;
		default:
			g_Debug.logState(::WARNING, "Its a color");
			break;
		}

	}

	file.close();
	g_Debug.logState(::SUCCESS, "Loaded in config " + strName);
}

void g_Config::Base::Load(const std::string& strName) 
{
	//thx @stephan again
	static auto split = [](const std::string& s, char delim) {
		std::vector<std::string> result;
		std::stringstream ss(s);
		std::string item;

		while (getline(ss, item, delim))
			result.push_back(item);

		return result;
	};

	std::string strParser = g_Config::Base::strBase + "\\" +  strName;
	INIReader parser(strParser);

	if (parser.ParseError())
		return;

	g_Debug.logState(::SUCCESS, "Opened up config " + strName + " for loading");

	for (auto& i : elements) 
	{
		std::vector<std::string> strSplitted = split(i.first, ':');
		switch (strSplitted[0][0])
		{
		case 'i':
			elements[i.first] = parser.GetInteger(strSplitted[1], strSplitted[2], 0); 
			break;
		case 'f':
			elements[i.first] = parser.GetReal(strSplitted[1], strSplitted[2], 0.f);
			break;
		case 'b':
			elements[i.first] = parser.GetBoolean(strSplitted[1], strSplitted[2], true);
			break;
		default:
			break;
		}
	}
	Update(2);
	g_Debug.logState(::SUCCESS, "Loaded in config " + strName);
}
