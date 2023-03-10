#pragma once
#include <cstdint>

namespace Offsets 
{
	inline uintptr_t pAssembly;

	namespace Recoil {
		constexpr uintptr_t RecoilFir = 0x3B8C60;
	}

	namespace Bullet {
		constexpr uintptr_t BulletInitialization = 0x3AB3B0;	
	}

	namespace Firearms {
		constexpr uintptr_t CreateBullet = 0x3ADF90;
		constexpr uintptr_t CreateExplosiveBullet = 0x3AE4F0;
		constexpr uintptr_t DoAttack = 0x3AEC10;
		constexpr uintptr_t Reload = 0x3B5AE0;
		constexpr uintptr_t Update = 0x3B6920;
	}

	namespace Cam {
		constexpr uintptr_t GetCurrent = 0x1C80BD0;
		constexpr uintptr_t SetAspect = 0x1C81370;
	}

	namespace Transform {
		constexpr uintptr_t GetPos = 0x1CCE680;
		constexpr uintptr_t getLocalPos = 0x1CCE350;
		constexpr uintptr_t getPosInjected = 0x1CCE630;
	}

	namespace Player {
		constexpr uintptr_t Update = 0x40FE70;
	}

	namespace UnityEngine {
		constexpr uintptr_t SetCursorLock = 0x1CA3130;
		constexpr uintptr_t WorldToScreen = 0x1C80780;
	}

	namespace Multiplayer {
		constexpr uintptr_t GetNetworkManager = 0x3EC930;
		constexpr uintptr_t GetLobbyManager = 0x3E21E0;
		constexpr uintptr_t IsTeamMode = 0x3E7440;
	}

	namespace AntiCheat {
		constexpr uintptr_t Update = 0x440400;
	}
	namespace Chat {
		constexpr uintptr_t Update = 0x33E6E0;
		constexpr uintptr_t SwearCheck = 0x33E470;
		constexpr uintptr_t AddMessage = 0x33D110;
		constexpr uintptr_t SetSteamID = 0x33DE60;
	}
}