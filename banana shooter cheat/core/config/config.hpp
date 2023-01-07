#pragma once
//i legit got no idea why a struct fucked with imgui, but with namespace it works well so i wont complain

namespace g_Config 
{
	//combat tab

	inline bool NoRecoil = false;
	inline bool NoReload = false;
	inline bool ExplosiveBullets = false;

	inline bool bMagicBullets = false;
	inline int iMagicBullets = 0;
}
//struct {
//	bool NoRecoil = false;
//	bool NoReload = false;
//	bool ExplosiveBullets = false;
//} g_Config;