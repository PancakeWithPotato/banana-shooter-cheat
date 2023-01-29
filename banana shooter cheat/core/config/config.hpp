#pragma once
#include "../../utilities/includes.hpp"

namespace g_Config  {
	void init();
	void save(const std::string& strName);
	void load(const std::string& strName);
	void getConfigs();
	void openDir();

	inline std::vector<std::string> strConfigs;
	inline std::map<std::string, std::variant<int, bool, float, ImVec4>> elements;
	inline std::string strBase;

	template <typename T>
	inline T& get(std::string name) {
		if (g_Config::elements.count(name))
			return std::get<T>(g_Config::elements[name]);
		else {
			g_Debug.logState(::ERROR, "[Config] ERROR: Could not find config element: %s", name.c_str());
			std::this_thread::sleep_for(std::chrono::seconds(5));
		}
	}

	inline ImU32 convertColor(ImVec4 color) {
		return ((ImU32)(color.w * 255.0f) << 24) + ((ImU32)(color.z * 255.0f) << 16) + ((ImU32)(color.y * 255.0f) << 8) + (ImU32)(color.x * 255.0f);
	}

	inline std::string strConfiginput;
}
