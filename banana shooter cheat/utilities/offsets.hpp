#pragma once
#include <cstdint>

namespace Offsets 
{
	inline uintptr_t pAssembly;

	namespace Recoil {
		constexpr uintptr_t RecoilFir = 0x3B8C60;
	}

	namespace Firearms {
		constexpr uintptr_t CreateBullet = 0x3ADF90;
		constexpr uintptr_t CreateExplosiveBullet = 0x3AE4F0;
		constexpr uintptr_t DoAttack = 0x3AEC10;
		constexpr uintptr_t Reload = 0x3B5AE0;
	}

	namespace Cam {
		constexpr uintptr_t GetCurrent = 0x1C80BD0;
		constexpr uintptr_t SetAspect = 0x1C81370;
	}

	namespace Transform {
		constexpr uintptr_t GetPos = 0x1CCE680;
	}

	namespace Player {
		constexpr uintptr_t Update = 0x40FE70;
	}

	namespace Cursor {
		constexpr uintptr_t SetLock = 0x1CA3130;
	}

	namespace Multiplayer {
		constexpr uintptr_t GetNetworkManager = 0x3EC930;
	}
}