#pragma once
#include "il2cpp.h"
#include "../utilities/offsets.hpp"
#include "../dependencies/imgui/imgui.h"
#include "structures.h"
typedef UnityEngine_Vector3_o Vector3;
typedef UnityEngine_Vector2_o Vector2;
typedef UnityEngine_Transform_o Transform;
typedef UnityEngine_Camera_o Camera;
typedef Multiplayer_NetworkManager_o NetworkManager;
typedef Multiplayer_LobbyManager_o LobbyManager;

struct {
	Camera* localCamera = nullptr;

	enum PLAYER_BONES {
		EYE_LEFT = 9,
		EYE_RIGHT = 8,
		DICK = 0,
		PELVIS = 1,
		STOMACH = 2,
		LEFT_FOOT = 30,
		RIGHT_FOOT = 35,
		HEAD = 5,
		MOUTH = 7,
	};

	enum GAME_MODES {
		READYUP,
		SHOOTING_RANGE,
		BRAWL,
		TEAM_DM,
		INFECTED,
		KILL_CONFIRM,
		RANDOMIZER,
		KOTH,
		GUN_GAME
	};

	enum GLOW_MODES {
		OUTLINE_ALL,
		OUTLINE_VISIBLE,
		OUTLINE_HIDDEN,
		OUTLINE_AND_SILHOUETTE,
		SILHOUETTE_ONLY
	};

	enum CURSOR_LOCK_MODE {
		NONE,
		LOCKED,
		CONFINED,
	};

//thx nerdie, whole private section is your shit LOL
private:
	struct assembly_pair
	{
		const char* name;
		il2cpp::il2cppAssembly* assembly;
	};

	inline MethodInfo* get_method(il2cpp::il2cppAssembly* assembly, const char* namespxce, const char* class_name, const char* method)
	{

		auto klass = find_class(assembly, namespxce, class_name);

		for (int i = 0; i < 16; i++)
		{

			auto info = this->getMethodFromName(klass, method, i);

			if (!info)
				continue;

			return info;

		}

		return nullptr;

	}

	inline il2cpp::il2cppClass* find_class(il2cpp::il2cppAssembly* assembly, const char* namespxce, const char* class_name)
	{
		return getClassFromName(assembly->m_pImage, namespxce, class_name);
	}

public:
	il2cpp::il2cppAssembly* assembly = nullptr;
	std::vector<assembly_pair> assemblies{};

	typedef il2cpp::il2cppAssembly**(__fastcall* getAssembliesFN)(void*, size_t*);
	typedef void*(__fastcall* getDomainFN)(void*);
	typedef MethodInfo*(__fastcall* getMethodFromNameFN)(il2cpp::il2cppClass*, const char*, int);
	typedef il2cpp::il2cppClass*(__fastcall* getClassFromNameFN)(il2cpp::il2cppImage*, const char*, const char*);

	getMethodFromNameFN getMethodFromName;
	getDomainFN getDomain;
	getAssembliesFN getAssemblies;
	getClassFromNameFN getClassFromName;
	void getAllExports() 
	{
		getAssemblies = reinterpret_cast<getAssembliesFN>(GetProcAddress((HMODULE)Offsets::pAssembly, "il2cpp_domain_get_assemblies"));
		getDomain = reinterpret_cast<getDomainFN>(GetProcAddress((HMODULE)Offsets::pAssembly, "il2cpp_domain_get"));
		getMethodFromName = reinterpret_cast<getMethodFromNameFN>(GetProcAddress((HMODULE)Offsets::pAssembly, "il2cpp_class_get_method_from_name"));
		getClassFromName = reinterpret_cast<getClassFromNameFN>(GetProcAddress((HMODULE)Offsets::pAssembly, "il2cpp_class_get_method_from_name"));

		if (!getAssemblies)
			return;
		if (!getDomain)
			return;
		if (!getMethodFromName)
			return;
		if (!getClassFromName)
			return;

		size_t assemblyCount = 0;
		auto assemblies = getAssemblies(getDomain, &assemblyCount);

		for (int i = 0; i < assemblyCount; i++) 
		{
			if (!assemblies[i])
				continue;

			assembly_pair assembly{};
			assembly.name = assemblies[i]->m_aName.m_pName;
			assembly.assembly = assemblies[i];

			if (std::string(assembly.name) == "Assembly-CSharp")
				this->assembly = assemblies[i];

			std::cout << "il2cpp | found assembly: " << assembly.name << "\n";

			this->assemblies.push_back(assembly);
		}
	}

	
	std::string toString(System_String_o* str)
	{

		if (!str)
			return "NULL";
		else
		{
			auto wstr = std::wstring((wchar_t*)(&str->fields._firstChar));
			return std::string(wstr.begin(), wstr.end());
		}

	}

	float getDistance(Vector3 one, Vector3 two) {
		Vector3 delta;
		delta.fields.x = one.fields.x - two.fields.x;
		delta.fields.y = one.fields.y - two.fields.y;
		delta.fields.z = one.fields.z - two.fields.z;

		return sqrtf((delta.fields.x * delta.fields.x) + (delta.fields.y * delta.fields.y) + (delta.fields.z * delta.fields.z));
	}

	bool worldToScreen(Vector3& in, Vector2& out) {
		if (!localCamera)
			return false;

		Vector3 pos = reinterpret_cast<Vector3(__cdecl*)(void*, Vector3)>(Offsets::pAssembly + Offsets::UnityEngine::WorldToScreen)(localCamera, in);

		if (pos.fields.z < 0)
			return false;

		ImVec2 screenSize = ImGui::GetIO().DisplaySize;
		out = Vector2{ pos.fields.x, screenSize.y - pos.fields.y };
		return true;
	}

	Vector3 getTransformPosition(Transform* transform) {
		if (!transform || sizeof(*transform) != sizeof(Transform) )
			return Vector3{};
		
		if (!transform->fields.m_CachedPtr)
			return Vector3{};

		static il2cpp::il2cppAssembly* coremodule = nullptr;

		if (!coremodule) {
			for (const auto& assembly : assemblies) {
				if (std::string(assembly.name) == "UnityEngine.CoreModule")
					coremodule = assembly.assembly;
			}
		}

		return reinterpret_cast<Vector3(__cdecl*)(UnityEngine_Transform_o*)>(get_method(coremodule, "UnityEngine", "Transform", "get_position")->methodPointer)(transform);
	}

	Vector3 getTransformLocalPosition(Transform* transform) {
		if (!transform || sizeof(*transform) != sizeof(Transform))
			return Vector3{};

		return reinterpret_cast<Vector3(__cdecl*)(Transform*)>(Offsets::pAssembly + Offsets::Transform::getLocalPos)(transform);
	}

	void getTransformPosInjected(Transform* transform, Vector3& out) {
		if (!transform || sizeof(*transform) != sizeof(Transform) || !transform->fields.m_CachedPtr) //all them checks, if this still crashes i will be REALLY sad
			return;

		reinterpret_cast<void(__cdecl*)(Transform*, Vector3&)>(Offsets::pAssembly + Offsets::Transform::getPosInjected)(transform, out);
	}

	void setCursorLockedMode(CURSOR_LOCK_MODE mode) {
		return reinterpret_cast<void(__cdecl*)(CURSOR_LOCK_MODE)>(Offsets::pAssembly + Offsets::UnityEngine::SetCursorLock)(mode);
	}

	NetworkManager* getNetworkManager() {
		return reinterpret_cast<NetworkManager*(__cdecl*)()>(Offsets::pAssembly + Offsets::Multiplayer::GetNetworkManager)();
	}

	LobbyManager* getLobbyManager() {
		return reinterpret_cast<LobbyManager*(__cdecl*)()>(Offsets::pAssembly + Offsets::Multiplayer::GetLobbyManager)();
	}

	bool isTeamMode(NetworkManager* self) {
		return reinterpret_cast<bool(__cdecl*)(NetworkManager*)>(Offsets::pAssembly + Offsets::Multiplayer::IsTeamMode)(self);
	}

	bool localInGame() {
		NetworkManager* networkManager = getNetworkManager();
		LobbyManager* lobbyManager = getLobbyManager();

		if (!networkManager || !networkManager->fields.m_CachedPtr)
			return false;

		if (!networkManager->fields.connecting || !networkManager->fields.game)
			return false;

		if (!lobbyManager)
			return false;

		return true;
	}

	NetworkManager* networkManager = nullptr;
} g_Sdk;

class Player : public Multiplayer_Client_ClientPlayer_o {
public:
	bool isValid() {
		if (!this)
			return false;
		if (!isAlive())
			return false;
		if (!this->fields._Username_k__BackingField)
			return false;
		if (!this->fields.currentWeapon)
			return false;

		return true;
	}
	int getHealth() {
		if (!this)
			return 0;
		
		return fields.health;
	}

	bool isAlive() {
		if (!this)
			return false;

		return getHealth() > 0;
	}

	int getTeam() {
		if (!this)
			return -1;
		return fields.team;
	}

	int getMaxHealth() {
		if (!this)
			return 0;

		return fields.maxHealth;
	}

	bool isEnemyWith(Player* player) {
		NetworkManager* networkManager = g_Sdk.getNetworkManager();

		if (!this)
			return false;

		if (!g_Sdk.isTeamMode(networkManager))
			return true;

		return !(player->getTeam() == this->getTeam());
	}
};