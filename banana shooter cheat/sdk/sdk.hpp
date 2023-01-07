#pragma once
#include "il2cpp.h"
#include "../core/hack.hpp"
#include "../utilities/offsets.hpp"

typedef UnityEngine_Vector3_o Vector3;
typedef UnityEngine_Transform_o Transform;
typedef UnityEngine_Camera_o Camera;
typedef Multiplayer_Client_ClientPlayer_o Player;
typedef Multiplayer_NetworkManager_o NetworkManager;
typedef Multiplayer_LobbyManager_o LobbyManager;

struct {
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

	Vector3 getTransformPosition(Transform* transform) {
		if (!transform || !transform->fields.m_CachedPtr)
			return Vector3{};

		return reinterpret_cast<Vector3(__cdecl*)(Transform*)>(Offsets::pAssembly + Offsets::Transform::GetPos)(transform);
	}

	void setCursorLockedMode(CURSOR_LOCK_MODE mode) {
		return reinterpret_cast<void(__cdecl*)(CURSOR_LOCK_MODE)>(Offsets::pAssembly + Offsets::Cursor::SetLock)(mode);
	}

	NetworkManager* getNetworkManager() {
		return reinterpret_cast<NetworkManager*(__cdecl*)()>(Offsets::pAssembly + Offsets::Multiplayer::GetNetworkManager)();
	}

	bool localConnecting() {
		NetworkManager* networkManager = getNetworkManager();
		return networkManager->fields.connecting;
	}

	bool localInGame() {
		NetworkManager* networkManager = getNetworkManager();

		if (!networkManager || networkManager->fields.connecting || !networkManager->fields.game)
			return false;

		return true;
	}
} g_Sdk;