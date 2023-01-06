#pragma once
#include "il2cpp.h"
#include "../core/hack.hpp"
#include "../utilities/offsets.hpp"

typedef UnityEngine_Vector3_o Vector3;
typedef UnityEngine_Transform_o Transform;
typedef UnityEngine_Camera_o Camera;
typedef Multiplayer_Client_ClientPlayer_o Player;

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

	Vector3 getTransformPosition(Transform* transform) {
		if (!transform || !transform->fields.m_CachedPtr)
			return Vector3{};

		return reinterpret_cast<Vector3(__cdecl*)(Transform*)>(Offsets::pAssembly + Offsets::Transform::GetPos)(transform);
	}
} g_Sdk;