#pragma once
#include "../../utilities/includes.hpp"

namespace g_Config 
{

	void Init();
	void Save();
	void Load();
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
