#include "config.hpp"
#include <direct.h>

#include "INIREADER.h"

void g_Config::Base::Init()
{
	std::string strLocaluser = (g_Hack->strName == "dev" ? "Pancake" : g_Hack->strName); //works tho
	std::string strLocalpath = "C:\\Users\\" + strLocaluser + "\\Documents\\meowware";
	if(!_mkdir(strLocalpath.c_str()))
		g_Debug.logState(::SUCCESS, "Created directory 1!");

	strLocalpath.append("\\banana_shooter");
	if (!_mkdir(strLocalpath.c_str()))
		g_Debug.logState(::SUCCESS, "Created directory 2!");

	g_Config::Base::strConfigs.reserve(1); //surely at least 1 config exists/will be made

	//set up elements
	elements["b:combat:aimbot_enabled"] = false;
	elements["i:combat:aimbot_target"] = 0;
	elements["b:combat:noreload"] = false;
	elements["b:combat:norecoil"] = false;
}

//thx stephan (heavily inspired)
void g_Config::Base::Load(const std::string& strName) 
{
	if (!std::filesystem::exists(strName)) {
		g_Debug.logState(::ERROR, "Config " + strName + " doesn't exist!");
		return;
	}

	INIReader configParser(strName);

	if (configParser.ParseError()) {
		g_Debug.logState(::ERROR, "Couldn't load file " + strName);
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

	for (auto& i : elements) {
		std::vector<std::string> splitted = split(i.first, ':');

		switch (splitted[0][0]) {
		case 'i': elements[i.first] = configParser.GetInteger(splitted[1], splitted[2], 0); break;
		case 'f': elements[i.first] = configParser.GetReal(splitted[1], splitted[2], 0.0f); break;
		case 'b': elements[i.first] = configParser.GetBoolean(splitted[1], splitted[2], false); break;
		case 'c': {
			std::string strCol = configParser.Get(splitted[1], splitted[2], "255,255,255,255");
			std::vector<std::string> splittedCol = split(strCol, ',');

			if (splittedCol.size() != 4)
				continue;

			elements[i.first] = ImColor(std::stoi(splittedCol[0]), std::stoi(splittedCol[1]), std::stoi(splittedCol[2]), std::stoi(splittedCol[3]));
			break;
		}
		}
	}

	g_Debug.logState(::SUCCESS, "Loaded config!");
}

//again, thx stephan (heavily inspired)
void g_Config::Base::Save(const std::string& strName) {
	g_Debug.logState(::WARNING, "Saving config " + strName);

	static auto split = [](const std::string& s, char delim) {
		std::vector<std::string> result;
		std::stringstream ss(s);
		std::string item;

		while (getline(ss, item, delim))
			result.push_back(item);

		return result;
	};

	std::string previousSection = "";

	std::ofstream write(strName);

	for (auto i : elements) {
		std::vector<std::string> splitted = split(i.first, ':');

		if (previousSection != splitted[1]) {
			write << "\n[" << splitted[1] << "]\n";
			previousSection = splitted[1];
		}

		switch (splitted[0][0]) {
		case 'i': write << splitted[2] << " = " << std::to_string(std::get<int>(i.second)); break;
		case 'f': write << splitted[2] << " = " << std::to_string(std::get<float>(i.second)); break;
		case 'b': write << splitted[2] << " = " << std::to_string(std::get<bool>(i.second)); break;
		case 'c': {
			ImColor col = std::get<ImColor>(i.second);

			write << splitted[2] << " = " << std::to_string(col.Value.x) << ", " << std::to_string(col.Value.y) << ", " << std::to_string(col.Value.z) << ", " << std::to_string(col.Value.w);
			break;
		}
		}

		write << '\n';
	}

	write.close();


	g_Debug.logState(::SUCCESS, "Saved config!");
}
