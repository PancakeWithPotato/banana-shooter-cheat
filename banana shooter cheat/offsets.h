#pragma once
#include <cstdint>
namespace Offsets 
{
	namespace Recoil 
	{
		constexpr uintptr_t RecoilFir = 0x3B8C60;
	}

	namespace Firearms {
		constexpr uintptr_t CreateBullet = 0x3ADF90;
		constexpr uintptr_t CreateExplosiveBullet = 0x3AE4F0;
		constexpr uintptr_t DoAttack = 0x3AEC10;
	}

	namespace Cam {
		constexpr uintptr_t GetCurrent = 0x1C80BD0;
		constexpr uintptr_t SetAspect = 0x1C81370;
	}

	namespace Transform {
		constexpr uintptr_t GetPos = 0x1CCE680;
	}
}