#pragma once
#include "../../utilities/includes.hpp"
#include <variant>
#include "../../dependencies/imgui/imgui.h"
#include <filesystem>
#include <fstream>
namespace g_Config 
{
	namespace Base {
		void Init();
		void Save(const std::string& strName);
		void Load(const std::string& strName);
		inline std::vector<std::string> strConfigs;
		inline std::map<std::string, std::variant<int, bool, float, ImColor>> elements;
		template <typename T>
		inline T& get(std::string name) {
			if (g_Config::Base::elements.count(name))
				return std::get<T>(g_Config::Base::elements[name]);
			else {
				g_Debug.logState(::ERROR, "[Config] ERROR: Could not find config element: %s", name.c_str());
				std::this_thread::sleep_for(std::chrono::seconds(5));
			}
		}
	}
	namespace Combat {
		inline bool NoReload = false;
		inline bool NoRecoil = false;

		inline bool ExplosiveBullets = false;
		inline int BulletsCount = 1;

		inline bool Aimbot = false;
		inline int AimbotHitbox = 0; //0 head, 1 body
	}

	namespace Misc {
		inline bool StartGame = false;
		inline bool bSpotifyDetection = true;
	}

	namespace Visuals {
		inline float fBobSpeed = 17.f;
	}
}
