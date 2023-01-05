#include "hooks.h"
#include <format>
Hooks::HookReturn Hooks::AddHook(void* pTarget, void* detour) 
{
	void* orig;
	auto status = MH_CreateHook(pTarget, detour, &orig); //create the hook
	
	MH_EnableHook(pTarget);//enable the hook

	this->m_iCount++;

	return { status, orig };
}

bool Hooks::Setup() 
{
	if (MH_Initialize() != MH_OK)
		return false;
	this->oRecoil = reinterpret_cast<Hooks::RecoilFirFN>(this->AddHook(reinterpret_cast<void*>(g_Hack->pAssembly + Offsets::Recoil::RecoilFir), this->hRecoilFir).oAddr);
	if (!this->oRecoil) {
		g_Hack->Log(hack::error, "Failed to get hook number ", this->m_iCount);
		return false;
	}

	this->oCreateBullet = reinterpret_cast<Hooks::CreateBulletFN>(this->AddHook(reinterpret_cast<void*>(g_Hack->pAssembly + Offsets::Firearms::CreateBullet), this->hCreateBullet).oAddr);
	if (!this->oCreateBullet) {
		g_Hack->Log(hack::error, "Failed to get hook number ", this->m_iCount);
		return false;
	}
		
	return true;
}
void Hooks::Destroy() 
{
	MH_DisableHook(MH_ALL_HOOKS);
	MH_RemoveHook(MH_ALL_HOOKS);
	MH_Uninitialize();
}
void __cdecl Hooks::hRecoilFir(void* thisptr, float x, float y, float z)
{
	if(Binds::bRecoil)
		return g_Hooks->oRecoil(thisptr, 0, 0, 0);
	//no need for "else" here, ty stephan for seeing this and telling me lol (i was tired when writing this originally)
	return g_Hooks->oRecoil(thisptr, x, y, z);
}

void __cdecl Hooks::hCreateBullet(void* thisptr, UnityEngine_Vector3_o pos)
{
	for (size_t i = 0; i < 15; i++)
	{
		g_Hooks->oCreateBullet(thisptr, pos);
	}
	return 	g_Hooks->oCreateBullet(thisptr, pos);
}
