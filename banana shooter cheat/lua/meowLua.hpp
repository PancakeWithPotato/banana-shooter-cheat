#pragma once
#include "include/lua.hpp"
#include "../utilities/includes.hpp"

class meowLua {
public:
	lua_State* state;
	bool setup();
	void destroy();
}; inline meowLua g_Lua;