#pragma once
#include "il2cpp.h"
#include "../core/hack.hpp"
#include "../utilities/offsets.hpp"

#include "../dependencies/imgui/imgui.h" // for screen size (w2s)

typedef UnityEngine_Vector3_o Vector3;
typedef UnityEngine_Vector2_o Vector2;
typedef UnityEngine_Transform_o Transform;
typedef UnityEngine_Camera_o Camera;
typedef Multiplayer_Client_ClientPlayer_o Player;
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

	Vector3 getTransformPosition(Transform* transform) 
	{

		//std::cout << "Getting transofmr pos\n";
		if (transform == nullptr || sizeof(*transform) != sizeof(Transform))
			return Vector3{};

		//std::cout << "Passed first check//\n";

		if (!transform->fields.m_CachedPtr)
			return Vector3{};

		//std::cout << "Passed second check\n";
		return reinterpret_cast<Vector3(__cdecl*)(Transform*)>(Offsets::pAssembly + Offsets::Transform::GetPos)(transform);
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

	bool IsTeamMode(NetworkManager* self)
	{
		return reinterpret_cast<bool(__cdecl*)(NetworkManager*)>(Offsets::pAssembly + Offsets::Multiplayer::IsTeamMode)(self);
	}

	bool localInGame() {
		//NetworkManager* networkManager = getNetworkManager();
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