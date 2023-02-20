#include "meowLua.hpp"
#include <ShlObj_core.h>
bool meowLua::setup() 
{
	this->baseFolder = g_Config::luaStrBase;
	this->getLuas();
	return true;
}
void meowLua::openLua(std::string name) 
{
	if (name == "")
		return;
	//create and push back new lua
	LUA_t newlua;
	newlua.state = luaL_newstate();
	newlua.luaName = name;
	this->luas.emplace_back(newlua);

	//open "default" libs
	luaL_openlibs(this->luas.at(this->currentLuas).state);

	//luajit
	luaopen_ffi(this->luas.at(this->currentLuas).state);

	//register tables and metatables
	this->registerTables(this->luas.at(this->currentLuas).state);
	this->registerMetaTables(this->luas.at(this->currentLuas).state);

	//open the lua
	std::string file = this->baseFolder + "\\" + name + ".lua";
	int errorCode = luaL_dofile(this->luas.at(this->currentLuas).state, file.c_str());

	//do error checking
	//also waiting on sounds lol (reccomend me some good error sound, and good succes sound)
	if (errorCode != LUA_OK)
	{
		std::string errorMSG = lua_tostring(this->luas.at(this->currentLuas).state, -1);
		std::cout << ERR << std::format("Failed to load in lua {} due to: {}\n", name, errorMSG);
		g_Notifs.AddNotif(errorMSG);
		//play some error sound here
		this->destroy(name);
	}
	else 
	{
		std::cout << SUCCES << std::format("Loaded lua {}\n", name);
		//play some sound here
		g_Notifs.AddNotif(std::format("Succesfully loaded lua {}", name));
	}

	//emplace loaded lua name
	this->loadedLuas.emplace_back(name);
	this->currentLuas++;
}

void meowLua::destroy(std::string name)
{
	g_Debug.logState(::SUCCESS, "Unloading lua %s", name.c_str());
	for (int i = 0; i < this->luas.size(); i++)
	{
		//we found the lua to unload
		if (this->luas.at(i).luaName == name) {
			this->luas.erase(this->luas.begin() + i);
			break;
		}
	}
	this->currentLuas--;
}

void meowLua::getLuas() 
{
	//i love this function
	this->allLuas.clear();
	for (const auto& dirEntry : std::filesystem::recursive_directory_iterator(this->baseFolder)) {
		auto slash = dirEntry.path().generic_string().find_last_of('/');
		std::string strMeowname = dirEntry.path().generic_string().substr(slash + 1, dirEntry.path().generic_string().length());
		this->allLuas.emplace_back(strMeowname.substr(0, strMeowname.length() - 4));
	}
}

void meowLua::openDir()
{
	PIDLIST_ABSOLUTE pidl;
	if (SUCCEEDED(SHParseDisplayName(std::wstring(baseFolder.begin(), baseFolder.end()).c_str(), 0, &pidl, 0, 0))) {
		ITEMIDLIST idNull = { 0 };
		LPCITEMIDLIST pidlNull[1] = { &idNull };
		SHOpenFolderAndSelectItems(pidl, 1, pidlNull, 0);
		ILFree(pidl);
	}
}

void meowLua::registerTables(lua_State* L) 
{
	//vector
	{
		lua_newtable(L);
		lua_pushstring(L, "new");
		lua_pushcfunction(L, luaUtils::vec2New);
		lua_settable(L, -3);
		lua_setglobal(L, "vec2");

		lua_newtable(L);
		lua_pushstring(L, "new");
		lua_pushcfunction(L, luaUtils::vec3New);
		lua_settable(L, -3);
		lua_setglobal(L, "vec3");

		lua_newtable(L);
		lua_pushstring(L, "new");
		lua_pushcfunction(L, luaUtils::vec4New);
		lua_settable(L, -3);
		lua_setglobal(L, "vec4");
	}

	//render
	{
		lua_newtable(L);

		lua_pushstring(L, "text");
		lua_pushcfunction(L, luaVisuals::RenderText);
		lua_settable(L, -3);
		lua_pushstring(L, "rect");
		lua_pushcfunction(L, luaVisuals::renderRect);
		lua_settable(L, -3);

		lua_setglobal(L, "render");
	}

	//hack
	{
		lua_newtable(L);

		lua_pushstring(L, "addCallback");
		lua_pushcfunction(L, luaHack::AddCallback);
		lua_settable(L, -3);

		lua_pushstring(L, "addNotification");
		lua_pushcfunction(L, luaHack::addNotif);
		lua_settable(L, -3);

		lua_setglobal(L, "hack");
	}

	//config
	{
		lua_newtable(L);

		lua_pushstring(L, "get");
		lua_pushcfunction(L, luaConfig::getConfig);
		lua_settable(L, -3);

		lua_setglobal(L, "config");
	}

	//players
	{
		lua_newtable(L);
		this->playerTableIDX = lua_gettop(L);

		lua_pushstring(L, "get");
		lua_pushcfunction(L, luaPlayers::getPlayer);
		lua_settable(L, -3);

		lua_pushstring(L, "getSize");
		lua_pushcfunction(L, luaPlayers::getPlayersSize);
		lua_settable(L, -3);

		lua_pushstring(L, "getHealth");
		lua_pushcfunction(L, luaPlayers::getHealth);
		lua_settable(L, -3);

		lua_setglobal(L, "players");

	}

	//sdk
	{
		lua_newtable(L);

		lua_pushstring(L, "worldToScreen");
		lua_pushcfunction(L, luaSDK::world2Screen);
		lua_settable(L, -3);

		lua_pushstring(L, "getBonePos");
		lua_pushcfunction(L, luaSDK::getBonePositionAtIndex);
		lua_settable(L, -3);

		lua_setglobal(L, "SDK");
	}
}

void meowLua::registerMetaTables(lua_State* L)
{
	//players
	{
		luaL_newmetatable(L, "PlayerMetaTable");
		lua_pushstring(L, "__index");
		lua_pushvalue(L, this->playerTableIDX);
		lua_settable(L, -3);
	}

	//vectors
	{
		luaL_newmetatable(L, "vec2");
		lua_pushstring(L, "__add");
		lua_pushcfunction(L, luaUtils::vec2::addVec2);
		lua_settable(L, -3);
	}
}

//visuals
int luaVisuals::RenderText(lua_State* L)
{
	//i could also do this in every function, but maybe later lol
	int top = lua_gettop(L);
	if (top != 3) {
		luaL_error(L, "Incorrect amount of arguments! 3 expected, got %i", top);
		return 0;
	}
	luaL_argcheck(L, lua_isstring(L, 1), 1, "Expected a string! (1)");
	luaL_argcheck(L, lua_istable(L, 2), 2, "Expected an vec2 table! (2)");
	luaL_argcheck(L, lua_istable(L, 3), 3, "Expected a color table! (4)");
	ImVec2 vector = { tableutils::GetFloat(L, 2, "x"), tableutils::GetFloat(L, 2, "y") };

	ImVec4 color = { tableutils::GetFloat(L, 3, "x"), tableutils::GetFloat(L, 3, "y"), tableutils::GetFloat(L, 3, "z"), tableutils::GetFloat(L, 3, "w") };

	g_Visuals.renderText(lua_tostring(L, 1), vector, color);
	return 0;
}
int luaVisuals::renderRect(lua_State* L)
{
	luaL_argcheck(L, lua_istable(L, 1), 1, "Expected an vec2 table! (2)");
	luaL_argcheck(L, lua_istable(L, 2), 2, "Expected an vec2 table! (2)");
	luaL_argcheck(L, lua_istable(L, 3), 3, "Expected a color table! (4)");
	ImVec2 vectorMin = { tableutils::GetFloat(L, 1, "x"), tableutils::GetFloat(L, 1, "y") };
	ImVec2 vectorMax = { tableutils::GetFloat(L, 1, "x"), tableutils::GetFloat(L, 1, "y") };

	ImVec4 color = { tableutils::GetFloat(L, 3, "x"), tableutils::GetFloat(L, 3, "y"), tableutils::GetFloat(L, 3, "z"), tableutils::GetFloat(L, 3, "w") };

	g_Visuals.renderBox(vectorMin, vectorMax, color);
	return 0;
}
//config
int luaConfig::getConfig(lua_State* L)
{
	luaL_argcheck(L, lua_isstring(L, 1), 1, "Expected string (1)!"); //lua_isstring accepts numbers as well, so I will make a strict check later, for now, this works fine
	std::string configKey = lua_tostring(L, 1);
	static auto split = [](const std::string& s, char delim) {
		std::vector<std::string> result;
		std::stringstream ss(s);
		std::string item;

		while (getline(ss, item, delim))
			result.push_back(item);

		return result;
	};
	auto stuff = split(configKey, ',');
	//this is a test comment!
	switch (stuff[2][0])
	{
	case 'b': {
		bool configBool = g_Config::get<bool>(configKey);
		lua_pushboolean(L, configBool);
		return 1;
	}
	case 'f': {
		float configFloat = g_Config::get<float>(configKey);
		lua_pushnumber(L, configFloat);
		return 1;
	}
	case 'i': {
		int configInt = g_Config::get<int>(configKey);
		lua_pushinteger(L, configInt);
		return 1;
	}
	}
	std::string errorMessage = "Could not find config item " + (std::string)lua_tostring(L, 1);
	luaL_argerror(L, 1, errorMessage.data());
	return 1;
}

//utils
int luaUtils::vec2New(lua_State* L) 
{
	luaL_argcheck(L, lua_isnumber(L, 1), 1, "Expected number! (1)");
	luaL_argcheck(L, lua_isnumber(L, 2), 2, "Expected number! (1)");

	ImVec2 vector = { (float)lua_tonumber(L, 1), (float)lua_tonumber(L, 2)};
	luaUtils::vec2::vec2New(L, vector);
	return 1;
}

int luaUtils::vec3New(lua_State* L)
{
	luaL_argcheck(L, lua_isnumber(L, 1), 1, "Expected number! (1)");
	luaL_argcheck(L, lua_isnumber(L, 2), 2, "Expected number! (1)");
	luaL_argcheck(L, lua_isnumber(L, 2), 2, "Expected number! (1)");

	Vector3 vector = { (float)lua_tonumber(L, 1), (float)lua_tonumber(L, 2), (float)lua_tonumber(L, 3) };
	luaUtils::vec2::vec3New(L, vector);
	return 1;
}

int luaUtils::vec2::vec3New(lua_State* L, Vector3 vec) 
{
	int x, y, z;
	x = vec.fields.x; y = vec.fields.y, z = vec.fields.z;

	lua_newtable(L);
	lua_pushstring(L, "x");
	lua_pushnumber(L, x);
	lua_settable(L, -3);

	lua_pushstring(L, "y");
	lua_pushnumber(L, y);
	lua_settable(L, -3);

	lua_pushstring(L, "z");
	lua_pushnumber(L, z);
	lua_settable(L, -3);

	luaL_getmetatable(L, "vec3");
	lua_setmetatable(L, -2);

	return 1;
}

int luaUtils::vec2::addVec2(lua_State* L)
{
	luaL_argcheck(L, lua_istable(L, 1), 1, "expected vec2 table (2)");
	luaL_argcheck(L, lua_istable(L, 2), 2, "expected vec2 table (2)");

	ImVec2 vector = {tableutils::GetFloat(L, 1, "x"), tableutils::GetFloat(L, 1, "y") };
	ImVec2 vector2 = { tableutils::GetFloat(L, 2, "x"), tableutils::GetFloat(L, 2, "y") };

	ImVec2 vectorAdded = {(vector.x + vector2.x), (vector.y + vector2.y)};

	luaUtils::vec2::vec2New(L, vectorAdded);
	return 1;
}

int luaUtils::vec4New(lua_State* L) 
{

	luaL_argcheck(L, lua_isnumber(L, 1), 1, "Expected number! (1)");
	luaL_argcheck(L, lua_isnumber(L, 2), 2, "Expected number! (1)");
	luaL_argcheck(L, lua_isnumber(L, 3), 3, "Expected number! (1)");
	luaL_argcheck(L, lua_isnumber(L, 4), 4, "Expected number! (1)");

	ImVec4 vector = { (float)lua_tonumber(L, 1), (float)lua_tonumber(L, 2), (float)lua_tonumber(L, 3), (float)lua_tonumber(L, 4) };

	luaUtils::vec2::vec4New(L, vector);
	return 1;
}

//"hack"
int luaHack::AddCallback(lua_State* L)
{
	luaL_argcheck(L, lua_isstring(L, 1), 1, "Expected a string! (1)");
	luaL_argcheck(L, lua_isstring(L, 2), 2, "Expected a string! (1)");

	const char* function = lua_tostring(L, 2);
	const char* callback = lua_tostring(L, 1);
	std::pair<const char*, const char*> callBackFunction = { callback, function };
	std::cout << "Function: " << function << ", callback: " << callback << ".\n";
	//i may implement fnv hash soon, and then i can make this a switch statement, till then, it stays if else hell lol
	if (strcmp(callback, "render") == 0) {
		g_Lua.luas.at(g_Lua.currentLuas ).renderCallbacks.emplace_back(callBackFunction);
		g_Debug.logState(::SUCCESS, "Render Callback on function %s", function);
	}
	else if (strcmp(callback, "playerUpdate") == 0)
	{
		g_Lua.luas.at(g_Lua.currentLuas ).playerUpdateCallbacks.emplace_back(callBackFunction);
		g_Debug.logState(::SUCCESS, "PlayerUpdate Callback on function %s", function);
	}
	else if (strcmp(callback, "attackUpdate") == 0)
	{
		g_Lua.luas.at(g_Lua.currentLuas ).attackUpdateCallbacks.emplace_back(callBackFunction);
		g_Debug.logState(::SUCCESS, "AttackUpdate Callback on function %s", function);
	}
	return 0;
}

int luaHack::addNotif(lua_State* L)
{
	luaL_argcheck(L, lua_isstring(L, 1), 1, "Expected a string! (1)");
	std::string strNotif = lua_tostring(L, 1);
	g_Notifs.AddNotif(strNotif);
	return 0;
}
//extra utils (i hate this already)
static int luaUtils::vec2::vec2New(lua_State* L, ImVec2 vec)
{
	int x, y;
	x = vec.x; y = vec.y;

	lua_newtable(L);
	lua_pushstring(L, "x");
	lua_pushnumber(L, x);
	lua_settable(L, -3);

	lua_pushstring(L, "y");
	lua_pushnumber(L, y);
	lua_settable(L, -3);

	luaL_getmetatable(L, "vec2");
	lua_setmetatable(L, -2);

	return 1;
}

static int luaUtils::vec2::vec4New(lua_State* L, ImVec4 vector)
{
	int x, y, z, w;
	x = vector.x; y = vector.y; z = vector.z; w = vector.w;

	lua_newtable(L);
	lua_pushstring(L, "x");
	lua_pushnumber(L, x);
	lua_settable(L, -3);

	lua_pushstring(L, "y");
	lua_pushnumber(L, y);
	lua_settable(L, -3);

	lua_pushstring(L, "z");
	lua_pushnumber(L, z);
	lua_settable(L, -3);

	lua_pushstring(L, "w");
	lua_pushnumber(L, w);
	lua_settable(L, -3);

	luaL_getmetatable(L, "vec4");
	lua_setmetatable(L, -2);

	return 1;
}

int luaPlayers::getPlayer(lua_State* L)
{
	luaL_argcheck(L, lua_isnumber(L, 1), 1, "expected a number! (1)");
	int idx = (int)lua_tonumber(L, 1);
	
	//if index is -1, we want to return localplayer
	Player* player;
	if (idx == -1) {
		if (!g_Hack->localPlayer) {
			lua_pushnil(L);
			return 1;
		}
		player = g_Hack->localPlayer;
	}
	else {
		player = g_Hack->players.at(idx);
	}

	//this way it wont actually update, unless you update the player constantly. There is prob a better way, that doesnt involve calling a function in lua  (like localplayer.getHealth(), yet this value stays updated.
	//i believe its okay tho, since you should update the player values in the playerupdate callback anyways, so thats that
	lua_newtable(L);
	lua_pushstring(L, "health");
	lua_pushnumber(L, player->getHealth());
	lua_settable(L, -3);

	lua_pushstring(L, "isValid");
	lua_pushboolean(L, player->isValid());
	lua_settable(L, -3);

	lua_pushstring(L, "bones");
	lua_pushlightuserdata(L, player->fields.bones);
	lua_settable(L, -3);

	luaL_getmetatable(L, "playerMeta");
	lua_setmetatable(L, -2);
	return 1;
}

int luaPlayers::getHealth(lua_State* L)
{
	bool bPlayerPassed = false;
	if (lua_isuserdata(L, 1))
		bPlayerPassed = true;
	
	if (bPlayerPassed) {
		Player* player = (Player*)(lua_touserdata(L, 1)); //get the player object
		lua_pushinteger(L, player->getHealth()); //push the health value
		return 1; //return it to lua
	}
	lua_pushinteger(L, g_Hack->players.at((int)lua_tonumber(L, 1))->getHealth());

	return 1;
}

int luaPlayers::getPlayersSize(lua_State* L) 
{
	lua_pushinteger(L, g_Hack->players.size());
	return 1;
}

int luaPlayers::getBones(lua_State* L) 
{
	return 1;
}

int luaSDK::world2Screen(lua_State* L) 
{
	luaL_argcheck(L, lua_istable(L, 1), 1, "Expected vector3! (3)");

	Vector3 vector = { tableutils::GetFloat(L, 1, "x"), tableutils::GetFloat(L, 1, "y"), tableutils::GetFloat(L, 1, "z") };

	Vector2 outvector;
	if (g_Sdk.worldToScreen(vector, outvector)) {
		luaUtils::vec2::vec2New(L, { outvector.fields.x, outvector.fields.y });
		return 1;
	}

	luaUtils::vec2::vec2New(L, { 0.f, 0.f });
	return 1;
}

int luaSDK::getBonePositionAtIndex(lua_State* L) 
{
	luaL_argcheck(L, lua_islightuserdata(L, 1), 1, "Expected player.bones! (1)");
	luaL_argcheck(L, lua_isnumber(L, 2), 2, "Expected boneID (number)! (1)");

	System_Collections_Generic_List_Transform__o* bones = (System_Collections_Generic_List_Transform__o*)lua_touserdata(L, 1);
	int boneIDX = (int)lua_tonumber(L, 2);
	auto pos = g_Sdk.bone_position_at_index(bones, boneIDX);

	luaUtils::vec2::vec3New(L, pos);
	return 1;
}
