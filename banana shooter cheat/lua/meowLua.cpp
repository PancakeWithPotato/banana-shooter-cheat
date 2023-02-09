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
	LUA_t newlua;
	newlua.state = luaL_newstate();
	newlua.luaName = name;
	this->luas.emplace_back(newlua);
	luaL_openlibs(this->luas.at(this->currentLuas).state);
	this->registerTables(this->luas.at(this->currentLuas).state);
	std::string file = this->baseFolder + "\\" + name + ".lua";
	int errorCode = luaL_dofile(this->luas.at(this->currentLuas).state, file.c_str());
	if (errorCode != LUA_OK)
	{
		std::string errorMSG = lua_tostring(this->luas.at(this->currentLuas).state, -1);
		std::cout << ERR << std::format("Failed to load in lua {} due to: {}\n", name, errorMSG);
	}
	else
		std::cout << SUCCES << std::format("Loaded lua {}\n", name); 
	this->loadedLuas.emplace_back(name);
	this->currentLuas++;
}

void meowLua::destroy(std::string name)
{
	g_Debug.logState(::SUCCESS, "Unloading lua %s", name);
	for (auto& i : this->luas) {
		if (i.luaName == name) {
			lua_close(i.state);
			i.attackUpdateCallbacks.clear();
			i.renderCallbacks.clear();
			i.playerUpdateCallbacks.clear();
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

		//lua_newtable(L);
		//lua_pushstring(L, "new");
		//lua_settable(L, -3);
		//lua_setglobal(L, "vec3");

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

		lua_setglobal(L, "render");
	}

	//hack
	{
		lua_newtable(L);
		/*int hackTableIDX = lua_gettop(L);
		lua_pushvalue(L, hackTableIDX);
		lua_setglobal(L, "hack");

		lua_pushcfunction(L, luaHack::AddCallback);
		lua_setfield(L, -2, "addCallback");

		lua_pushcfunction(L, luaHack::testFunction);
		lua_setfield(L, -2, "testFunction");

		lua_pushcfunction(L, luaHack::getInstance);
		lua_setfield(L, -2, "getInstance");

		luaL_newmetatable(L, "hackTable");

		lua_pushstring(L, "__index");
		lua_pushvalue(L, hackTableIDX);
		lua_settable(L, -3);*/

		lua_pushstring(L, "addCallback");
		lua_pushcfunction(L, luaHack::AddCallback);
		lua_settable(L, -3);

		lua_setglobal(L, "hack");
	}

	//globals
	{

	}
}

//visuals
int luaVisuals::RenderText(lua_State* L)
{
	int top = lua_gettop(L);
	if (top != 3) {
		luaL_error(L, "Incorrect amount of arguments!");
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

//utils
int luaUtils::vec2New(lua_State* L) 
{
	luaL_argcheck(L, lua_isnumber(L, 1), 1, "Expected number! (1)");
	luaL_argcheck(L, lua_isnumber(L, 2), 2, "Expected number! (1)");

	ImVec2 vector = { (float)lua_tonumber(L, 1), (float)lua_tonumber(L, 2)};
	luaUtils::vec2::vec2New(L, vector);
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
