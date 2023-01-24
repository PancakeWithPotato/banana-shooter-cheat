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

	//get the current configs
	strConfigs.reserve(1); //at least 1 config will be made / is already saved
	GetConfigs();

	//set up elements
	// 
	//misc
	elements["misc,spotify,b"] = true;

	//combat
	elements["combat,aimbot_enabled,b"] = false;
	elements["combat,aimbot_target,i"] = 0;
	elements["combat,test,f"] = 50.f;
	elements["combat,explosive_bullets,b"] = false;
	elements["combat,bullet_count,i"] = 0;
	elements["combat,norecoil,b"] = false;
	elements["combat,noreload,b"] = false;

	//visuals
	elements["visuals,bob_speed,f"] = 17.f;

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
	file.close();
	GetConfigs(); //so we can see new configs
}

void g_Config::Load(const std::string& strName) {
	if (strName == "") {
		g_Debug.logState(::ERROR, "[Config] No name found for config, internal error");
		return;
	}
	std::string strFile = g_Config::strBase + "\\" + strName + ".meow";
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
		case 'b': 
			g_Debug.logState(::WARNING, "Found int for " + i.first);
			elements[i.first] = configParser.GetBoolean(splits[0], splits[1], false);
			break;
		
		case 'i':
			g_Debug.logState(::WARNING, "Found a int for " + i.first);
			elements[i.first] = configParser.GetInteger(splits[0], splits[1], 0);
			break;
		
		case 'f':
			g_Debug.logState(::WARNING, "Found a float for " + i.first);
			elements[i.first] = (float)configParser.GetReal(splits[0], splits[1], 0.f);
			break;
		
		}
	}
}

void g_Config::GetConfigs()
{
	strConfigs.clear();
	for (const auto& dirEntry : std::filesystem::recursive_directory_iterator(strBase)) {
		auto slash = dirEntry.path().generic_string().find_last_of('/');
		std::string strMeowname = dirEntry.path().generic_string().substr(slash + 1, dirEntry.path().generic_string().length());
		strConfigs.emplace_back(strMeowname.substr(0, strMeowname.length() - 5));
	}
}

void g_Config::OpenDir()
{
	//horrible code ahead :warning:
	std::string strCommand = "explorer.exe " + strBase;
	system(strCommand.c_str());
}

//std::string ToString(char input[]) 
//{
//	return std::string(&input[0]);
//}
//
//char ToChar(std::string input) 
//{
//	return char(&input[0]);
//}
