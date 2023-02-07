#pragma once
#include "include/lua.hpp"
#include "../utilities/includes.hpp"

class meowLua {
public:
	lua_State* state;
	bool setup();
	void destroy();
	void getLuas();
	void openDir();
	//setup stuff as well
	void registerTables(lua_State* L);
	void registerMetaTables(lua_State* L);

	std::string baseFolder = "";
	std::vector<std::pair<const char*, const char*>> renderCallbacks;
	std::vector<std::pair<const char*, const char*>> playerUpdateCallbacks;
	Player* currentPlayer = nullptr;
}; inline meowLua g_Lua;


namespace luaVisuals 
{
	int RenderText(lua_State* L);
}

namespace luaUtils 
{
	namespace vec2 
	{
		static int vec2New(lua_State* L, ImVec2 vec);
		static int vec4New(lua_State* L, ImVec4 vec);
	}
	int vec2New(lua_State* L);
	int vec3New(lua_State* L);
	int vec4New(lua_State* L);
}


namespace luaHack 
{
	int AddCallback(lua_State* L);
	int testFunction(lua_State* L);
	int testFunction2(lua_State* L);
	int getInstance(lua_State* L);
}
namespace tableutils {
	inline int GetInt(lua_State* L, int table_index, const char* var_name) {
		lua_pushstring(L, var_name);
		lua_gettable(L, table_index);
		int num = lua_tointeger(L, -1);
		lua_pop(L, 1);

		return num;
	}

	inline float GetFloat(lua_State* L, int table_index, const char* var_name) {
		lua_pushstring(L, var_name);
		lua_gettable(L, table_index);
		float num = lua_tonumber(L, -1);
		lua_pop(L, 1);

		return num;
	}

	inline bool GetBool(lua_State* L, int table_index, const char* var_name) {
		lua_pushstring(L, var_name);
		lua_gettable(L, table_index);
		bool boolean = lua_toboolean(L, -1);
		lua_pop(L, 1);

		return boolean;
	}

	inline std::string GetString(lua_State* L, int table_index, const char* var_name) {
		lua_pushstring(L, var_name);
		lua_gettable(L, table_index);
		std::string string = lua_tostring(L, -1);
		lua_pop(L, 1);

		return string;
	}

}