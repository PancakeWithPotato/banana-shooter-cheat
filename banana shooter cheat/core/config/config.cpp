#include "config.hpp"
#include <direct.h>

#include "INIREADER.h"


void g_Config::Init()
{
	std::string strLocaluser = (g_Hack->strName == "dev" ? "Pancake" : g_Hack->strName); //works tho
    strBase = "C:\\Users\\" + strLocaluser + "\\Documents\\meowware";
	if(!_mkdir(strBase.c_str()))
		g_Debug.logState(::SUCCESS, "Created directory 1!");

	strBase.append("\\banana_shooter");
	if (!_mkdir(strBase.c_str()))
		g_Debug.logState(::SUCCESS, "Created directory 2!");

	g_Config::strConfigs.reserve(1); //surely at least 1 config exists/will be made

	//set up elements
	
	//combat
	elements["combat,aimbot_enabled,b"] = false;
	elements["combat,aimbot_target,i"] = 0;
	elements["combat,test,f"] = 50.f;
}

void g_Config::Save(const std::string& strName) 
{
	if (strName == "") {
		g_Debug.logState(::ERROR, "[Config] No name found, maybe give it a name?");
		return;
	}

	//get config place
	std::string strFile = g_Config::strBase + "\\" + strName + ".meow";

	//open/create if it doesnt exist
	std::ofstream file(strFile);
	if (!file.is_open()) {
		g_Debug.logState(::ERROR, "[Config] Failed to open %s for writing", strFile);
		return;
	}

	static auto split = [](const std::string& s, char delim) {
		std::vector<std::string> result;
		std::stringstream ss(s);
		std::string item;

		while (getline(ss, item, delim))
			result.push_back(item);

		return result;
	};

	std::string strCurrentSection = "";

	for (auto& i : elements) {
		std::vector<std::string> splits = split(i.first, ',');
		//combat,aimbot_bool,b
		if (strCurrentSection == "") {
			strCurrentSection = splits[0];
			file << "[" << strCurrentSection << "]\n";
		}
		else if (strCurrentSection != splits[0]) {
			file << "\n";
			strCurrentSection = splits[0];
			file << "[" << strCurrentSection << "]\n";
		}
		switch (splits[2][0]) {
		case 'b':
			g_Debug.logState(::WARNING, "Found a bool from " + i.first);
			//file << splits[1] << " = " << std::to_string(g_Config::get<bool>(i.first)) << "\n";
			file << std::format("{} = {} \n", splits[1], std::to_string(g_Config::get<bool>(i.first)));
			break;
		case 'i':
			g_Debug.logState(::WARNING, "Found an int from " + i.first);
			file << std::format("{} = {} \n", splits[1], std::to_string(g_Config::get<int>(i.first)));
			break;
		case 'f':
			g_Debug.logState(::WARNING, "Found a float from " + i.first);
			file << std::format("{} = {} \n", splits[1], std::to_string(g_Config::get<float>(i.first)));
			break;
		}
	}
}

void g_Config::Load(const std::string& strName) {
	if (strName == "") {
		g_Debug.logState(::ERROR, "[Config] No name found for config, internal error");
		return;
	}
	std::string strFile = g_Config::strBase + "\\" + strName;
	INIReader configParser(strFile);
	if (configParser.ParseError()) {
		g_Debug.logState(::ERROR, "Failed to parse file");
		return;
	}

	static auto split = [](const std::string& s, char delim) {
		std::vector<std::string> result;
		std::stringstream ss(s);
		std::string item;

		while (getline(ss, item, delim))
			result.push_back(item);

		return result;
	};

	for (auto& i : elements) 
	{
		std::vector<std::string> splits = split(i.first, ',');
		//combat,aimbot_enabled,b
		switch (splits[2][0])
		{
		case 'b': {
			g_Debug.logState(::WARNING, "Found int for " + i.first);
			auto stuff = configParser.GetBoolean(splits[0], splits[1], false);
			std::cout << "Got " << (stuff ? "TRUE" : "FALSE") << " from file\n";
			elements[i.first] = stuff;
			break;
		}
		case 'i':
		{	g_Debug.logState(::WARNING, "Found a int for " + i.first);
			auto stuff = configParser.GetInteger(splits[0], splits[1], 0);
			std::cout << "Got " << stuff << " from file\n";
			elements[i.first] = stuff;
			break;
		}
		case 'f':
		{	g_Debug.logState(::WARNING, "Found a float for " + i.first);
			auto stuff = (float)configParser.GetReal(splits[0], splits[1], 0.f);
			std::cout << "Got " << stuff << " from file\n";
			elements[i.first] = stuff;
			break;
		}
		}
	}
}
