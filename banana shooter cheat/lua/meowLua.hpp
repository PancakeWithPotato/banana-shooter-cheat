#pragma once
#include "include/lua.hpp"
#include "../utilities/includes.hpp"
#pragma comment(lib, "lua51.lib")
#pragma comment(lib, "luajit.lib")

struct LUA_t 
{
	std::vector<std::pair<const char*, const char*>> renderCallbacks;
	std::vector<std::pair<const char*, const char*>> playerUpdateCallbacks;
	std::vector<std::pair<const char*, const char*>> attackUpdateCallbacks;

	lua_State* state;
	std::string luaName;
};
class meowLua 
{
private:
	//this way sucks ASS probably, idk
	int playerTableIDX = 0;
public:
	lua_State* state;
	void openLua(std::string name);
	bool setup();
	void destroy(std::string name); //unload
	void getLuas();
	void openDir();
	//setup stuff as well
	void registerTables(lua_State* L);
	void registerMetaTables(lua_State* L);

	std::string baseFolder = "fake";
	std::vector<std::pair<const char*, const char*>> renderCallbacks;
	std::vector<std::pair<const char*, const char*>> playerUpdateCallbacks;
	std::vector<std::pair<const char*, const char*>> attackUpdateCallbacks;
	std::vector<LUA_t> luas;
	std::vector<std::string> allLuas;
	int currentLuas = 0;
	std::string selectedLua = "none";
	Player* currentPlayer = nullptr;
	std::vector<std::string> loadedLuas;
}; inline meowLua g_Lua;


namespace luaVisuals 
{
	int RenderText(lua_State* L);
	int renderRect(lua_State* L);
}

namespace luaUtils 
{
	namespace vec2 
	{
		static int vec2New(lua_State* L, ImVec2 vec);
		static int vec4New(lua_State* L, ImVec4 vec);
		static int vec3New(lua_State* L, Vector3 vec);
		int addVec2(lua_State* L);
	}
	int vec2New(lua_State* L);
	int vec3New(lua_State* L);
	int vec4New(lua_State* L);
}


namespace luaHack 
{
	int AddCallback(lua_State* L);
	int addNotif(lua_State* L);
}

namespace luaSDK 
{
	int world2Screen(lua_State* L);
	int getBonePositionAtIndex(lua_State* L);
	int getTransformPosition(lua_State* L);
}

namespace luaPlayers 
{
	int getPlayer(lua_State* L);
	int getHealth(lua_State* L);
	int getPlayersSize(lua_State* L);
	int getBones(lua_State* L);
}
namespace luaConfig 
{
	int getConfig(lua_State* L);
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