#pragma once

namespace g_Config {
	namespace Combat {
		inline bool NoReload = false;
		inline bool NoRecoil = false;

		inline bool ExplosiveBullets = false;
		inline int BulletsCount = 1;

		inline bool Aimbot = false;
		inline int AimbotHitbox = 0;
	}

	namespace Misc {
		inline bool StartGame = false;
	}

	namespace Visuals {
		inline float fBobSpeed = 17.f;
	}
}
