#pragma once

#include <Wiwa/core/Core.h>

#include <Wiwa/ecs/EntityManager.h>
#include <Wiwa/utilities/filesystem/FileSystem.h>

#include <Wiwa/ecs/components/game/Character.h>
#include <Wiwa/ecs/components/game/player/CharacterController.h>
#include <Wiwa/ecs/components/game/player/Player.h>
#include <Wiwa/ecs/components/game/player/PlayerSpawner.h>
#include <Wiwa/ecs/components/game/enemy/Enemy.h>
#include <Wiwa/ecs/components/game/enemy/EnemySpawner.h>
#include <Wiwa/ecs/components/game/enemy/Wave.h>

#include <vector>

typedef size_t SceneId;

namespace Wiwa {

	enum class RoomType
	{
		NONE = -1,
		ROOM_HUB = 1,
		ROOM_COMBAT = 2,
		ROOM_REWARD = 3
	};
	static const char* s_RoomTypeStr[] =
	{ "ROOM_HUB", "ROOM_COMBAT", "ROOM_REWARD"};

	enum class RoomState
	{
		NONE = -1,
		STATE_STARTING = 1,
		STATE_MIDDLE = 2,
		STATE_FINISHED = 3,
		STATE_AWAITING_NEXT = 4,
		STATE_TRANSITIONING = 5
	};
	static const char* s_RoomStateStr[] =
	{ "STATE_STARTING", "STATE_MIDDLE", "STATE_FINISHED", "STATE_AWAITING_NEXT" };

	struct GameEvent
	{
		GameEvent(const char* event_name, const char* event_description, bool is_completed = false, size_t id_ = 0) :
			name(event_name), description(event_description), isCompleted(is_completed), id(id_) {};
		const char* name;
		const char* description;
		bool isCompleted = false;
		size_t id;
	};

	class WI_API GameStateManager {
	private:
		static RoomType s_RoomType;
		static RoomState s_RoomState;

		static bool s_HasFinshedLevel;
		static bool s_CanPassNextLevel;
		static bool s_PlayerTriggerNext;

		static std::vector<GameEvent> s_PreStartEvents;
		static std::vector<GameEvent> s_MiddleEvents;
		static std::vector<GameEvent> s_PostFinishedEvents;

		static void ChangeRoomState(RoomState room_state);
	public:

		// Save & Load Overall Player Progression
		static void SaveProgression();
		static void LoadProgression();

		// Save & Load Player data into player entity
		static void SavePlayer(const Character& character);
		static void LoadPlayer(Character& character);

		static void UpdateRoomState();
		static void QuerySpawners();

		static void StartNewRoom();
		static void EndCurrentRoom();

		static void LogRoomState();
	};
}