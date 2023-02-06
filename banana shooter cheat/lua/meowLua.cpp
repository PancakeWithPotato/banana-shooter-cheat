#include "meowLua.hpp"
#include <ShlObj_core.h>
bool meowLua::setup() 
{
	//create new state
	this->state = luaL_newstate();
	
	this->baseFolder = g_Config::luaStrBase;
	luaL_openlibs(this->state);

	this->registerTables();

	luaL_dofile(state, "C:\\Users\\Pancake\\Documents\\meowware\\banana_shooter\\luas\\first.lua");
	return true;
}

void meowLua::destroy()
{
	state = nullptr;
	lua_close(state);
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

void meowLua::registerTables() 
{
	//vector
	{
		lua_newtable(state);
		lua_pushstring(state, "new");
		lua_pushcfunction(state, luaUtils::vec2New);
		lua_settable(state, -3);
		lua_setglobal(state, "vec2");

		//lua_newtable(state);
		//lua_pushstring(state, "new");
		//lua_settable(state, -3);
		//lua_setglobal(state, "vec3");

		lua_newtable(state);
		lua_pushstring(state, "new");
		lua_pushcfunction(state, luaUtils::vec4New);
		lua_settable(state, -3);
		lua_setglobal(state, "vec4");
	}

	//render
	{
		lua_newtable(state);

		lua_pushstring(state, "text");
		lua_pushcfunction(state, luaVisuals::RenderText);
		lua_settable(state, -3);

		lua_setglobal(state, "render");
	}

	//hack
	{
		lua_newtable(state);

		lua_pushstring(state, "AddCallback");
		lua_pushcfunction(state, luaHack::AddCallback);
		lua_settable(state, -3);

		lua_setglobal(state, "hack");
	}

	//globals
	{

	}
}


//visuals
int luaVisuals::RenderText(lua_State* L)
{
	std::cout << "RenderText is called!\n";
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
	std::cout << "vec2New called\n";
	luaL_argcheck(L, lua_isnumber(L, 1), 1, "Expected number! (1)");
	luaL_argcheck(L, lua_isnumber(L, 2), 2, "Expected number! (1)");

	ImVec2 vector = { (float)lua_tonumber(L, 1), (float)lua_tonumber(L, 2)};
	luaUtils::vec2::vec2New(L, vector);
	return 1;
}

int luaUtils::vec4New(lua_State* L) 
{
	std::cout << "vec4New called\n";
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
	std::cout << "AddCallback is called!\n";
	luaL_argcheck(L, lua_isstring(L, 1), 1, "Expected a string! (1)");
	luaL_argcheck(L, lua_isstring(L, 2), 2, "Expected a string! (1)");

	const char* function = lua_tostring(L, 2);
	const char* callback = lua_tostring(L, 1);
	char meow = 'f';
	std::pair<const char*, const char*> callBackFunction = { callback, function };
	std::cout << "Function: " << function << ", callback: " << callback << ".\n";
	//i may implement fnv hash soon, and then i can make this a switch statement, till then, it stays if else hell lol
	if (callback == "render") {
		g_Lua.renderCallbacks.emplace_back(callBackFunction);
		g_Debug.logState(::SUCCESS, "Render Callback on function %s", function);
	}
	else if (callback == "playerUpdate")
	{
		g_Lua.playerUpdateCallbacks.emplace_back(callBackFunction);
		g_Debug.logState(::SUCCESS, "PlayerUpdate Callback on function %s", function);
	}

	return 0;
}


//extra utils (i hate this already)
static int luaUtils::vec2::vec2New(lua_State* L, ImVec2 vec)
{
	std::cout << "vec2NewUNDER called\n";
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
	std::cout << "vec4NewUNDER called\n";
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

