#include "meowLua.hpp"

bool meowLua::setup() 
{
	//create new state
	this->state = luaL_newstate();

	luaL_dostring(state, "x = 23");
	lua_getglobal(state, "x");
	if (lua_isnumber(state, -1)) {
		float x = (float)lua_tonumber(state, -1);
		std::cout << "Number from lua is: " << x << std::endl;
	}

	return true;
}

void meowLua::destroy()
{
	lua_close(state);
}
