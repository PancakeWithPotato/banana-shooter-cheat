#include "config.hpp"
#include <ShlObj_core.h>
#include <direct.h>
#include "ini.hpp"

void g_Config::init() 
{

	elements["misc,spotify,b"] = true;

	elements["combat,aimbot_enabled,b"] = false;
	elements["combat,aimbot_target,i"] = 0;
	elements["combat,explosive_bullets,b"] = false;
	elements["combat,bullet_count,i"] = 0;
	elements["combat,norecoil,b"] = false;
	elements["combat,noreload,b"] = false;

	elements["visuals,bob_speed,f"] = 17.f;
	elements["visuals,enemy_box_color,c"] = WHITE;
	elements["visuals,enemy_box,b"] = false;

	//global shit
	elements["global,copied_color,n"] = WHITE;

	auto languageID = GetUserDefaultLCID();
	g_Debug.logState(::WARNING, "Language id is %d", languageID);
	if(languageID == 0x0c07)
		strBase = "C:\\Benutzer\\" + g_Hack->username + "\\Documents\\meowware";
	else
		strBase = "C:\\Users\\" + g_Hack->username + "\\Documents\\meowware";
	//std::cout << std::format("Folder {}\n", strBase);
	if (std::filesystem::create_directory(strBase.c_str())) std::cout << "Created dir!\n";
	//std::cout << std::format("Folder2 {}\n", strBase);
	strBase.append("\\banana_shooter");
	if (std::filesystem::create_directory(strBase.c_str())) std::cout << "Created dir!\n";
	//std::cout << std::format("Folder3 {}\n", strBase);
	luaStrBase = strBase;
	//std::cout << std::format("Folder2_1 {}\n", luaStrBase);
	strBase.append("\\configs");
	if (std::filesystem::create_directory(strBase.c_str()))std::cout << "Created dir!\n";
	//std::cout << std::format("Folder4 {}\n", strBase);
	luaStrBase.append("\\luas");
	//std::cout << std::format("Folder2_2 {}\n", luaStrBase);
	if (std::filesystem::create_directory(luaStrBase.c_str())) std::cout << "Created dir!\n";

	// strBase = "C:\\Users\\" + g_Hack->username + "\\Documents\\meowware";
	////std::cout << std::format("Folder {}\n", strBase);
	//if (_mkdir(strBase.c_str())) std::cout << "Created dir!\n";
	////std::cout << std::format("Folder2 {}\n", strBase);
	//strBase.append("\\banana_shooter");
	//if	(_mkdir(strBase.c_str())) std::cout << "Created dir!\n";
	////std::cout << std::format("Folder3 {}\n", strBase);
	//luaStrBase = strBase;
	////std::cout << std::format("Folder2_1 {}\n", luaStrBase);
	//strBase.append("\\configs");
	//if (_mkdir(strBase.c_str()))std::cout << "Created dir!\n";
	////std::cout << std::format("Folder4 {}\n", strBase);
	//luaStrBase.append("\\luas");
	////std::cout << std::format("Folder2_2 {}\n", luaStrBase);
	//if (_mkdir(strBase.c_str())) std::cout << "Created dir!\n";

	getConfigs();
}

void g_Config::save(const std::string& strName)  {
	if (strName == "") {
		g_Debug.logState(::ERROR, "[Config] No name found, maybe give it a name?");
		return;
	}

	std::string strFile = g_Config::strBase + "\\" + strName + ".meow";

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
			file << std::format("{} = {} \n", splits[1], std::to_string(g_Config::get<bool>(i.first)));
			break;
		case 'i':
			file << std::format("{} = {} \n", splits[1], std::to_string(g_Config::get<int>(i.first)));
			break;
		case 'f':
			file << std::format("{} = {} \n", splits[1], std::to_string(g_Config::get<float>(i.first)));
			break;
		}
	}

	file.close();

	getConfigs(); 
}

void g_Config::load(const std::string& strName) {
	if (strName == "") {
		g_Debug.logState(::ERROR, "[Config] No name found for config,");
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
		switch (splits[2][0])
		{
		case 'b': 
			elements[i.first] = configParser.GetBoolean(splits[0], splits[1], false);
			break;
		case 'i':
			elements[i.first] = configParser.GetInteger(splits[0], splits[1], 0);
			break;
		case 'f':
			elements[i.first] = (float)configParser.GetReal(splits[0], splits[1], 0.f);
			break;
		
		}
	}
}

void g_Config::getConfigs() {
	strConfigs.clear();
	for (const auto& dirEntry : std::filesystem::recursive_directory_iterator(strBase)) {
		auto slash = dirEntry.path().generic_string().find_last_of('/');
		std::string strMeowname = dirEntry.path().generic_string().substr(slash + 1, dirEntry.path().generic_string().length());
		strConfigs.emplace_back(strMeowname.substr(0, strMeowname.length() - 5));
	}
}

void g_Config::openDir() {
	PIDLIST_ABSOLUTE pidl;
	if (SUCCEEDED(SHParseDisplayName(std::wstring(strBase.begin(), strBase.end()).c_str(), 0, &pidl, 0, 0))) {
		ITEMIDLIST idNull = { 0 };
		LPCITEMIDLIST pidlNull[1] = { &idNull };
		SHOpenFolderAndSelectItems(pidl, 1, pidlNull, 0);
		ILFree(pidl);
	}
}
