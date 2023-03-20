#pragma once

#include <Wiwa/core/Core.h>

#include <Wiwa/ecs/EntityManager.h>
#include <Wiwa/utilities/filesystem/FileSystem.h>

#include <Wiwa/ecs/components/game/Character.h>
#include <Wiwa/ecs/components/game/player/Player.h>
#include <Wiwa/ecs/components/game/player/PlayerSpawner.h>
#include <Wiwa/ecs/components/game/enemy/Enemy.h>
#include <Wiwa/ecs/components/game/enemy/EnemySpawner.h>
#include <Wiwa/ecs/components/game/enemy/Wave.h>

#include <vector>

typedef size_t SceneId;

namespace Wiwa {
	struct EnemySpawner
	{
		int maxEnemiesPerWave;
		int maxWaveCount;
		int currentWaveCount;
		float timeBetweenWaves;
		bool hasFinished;
	};

	enum class RoomType
	{
		NONE = 0,
		ROOM_HUB = 1,
		ROOM_COMBAT = 2,
		ROOM_REWARD = 3,
		ROOM_BOSS = 4,
		ROOM_SHOP = 5
	};
	static const char* s_RoomTypeStr[] =
	{ "NONE", "ROOM_HUB", "ROOM_COMBAT", "ROOM_REWARD", "ROOM_BOSS", "ROOM_SHOP"};

	enum class RoomState
	{
		NONE = 0,
		STATE_STARTED = 1,
		STATE_FINISHED = 2,
		STATE_AWAITING_NEXT = 3,
		STATE_TRANSITIONING = 4
	};
	static const char* s_RoomStateStr[] =
	{ "NONE", "STATE_STARTED", "STATE_FINISHED", "STATE_AWAITING_NEXT", "STATE_TRANSITIONING"};

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

		static bool s_HasFinshedRoom;
		static bool s_CanPassNextRoom;
		static bool s_PlayerTriggerNext;

		static std::vector<GameEvent> s_PreStartEvents;
		static std::vector<GameEvent> s_MiddleEvents;
		static std::vector<GameEvent> s_PostFinishedEvents;

		static void ChangeRoomState(RoomState room_state);

	public:
		static int s_TotalSpawners;
		static int s_SpawnersFinished;
		// Save & Load Overall Player Progression
		static void SaveProgression();
		static void LoadProgression();

		// Save & Load Player data into player entity
		static void SavePlayer(const Character& character);
		static void LoadPlayer(Character& character);

		static void UpdateRoomState();
		static void UpdateCombatRoom();

		// Run and rooms
		static void StartRun();
		static void EndRun();
		static void InitHub();
		static void StartNewRoom();
		static void EndCurrentRoom();

		inline static void SetRoomType(RoomType type) { s_RoomType = type; }
		inline static void SetRoomState(RoomState type) { s_RoomState = type; }
		inline static RoomType GetType() { return s_RoomType; }

		static void LogRoomState();
		static const char* GetRoomState();
		static const char* GetRoomType();

		// Reset combat data
		static void ResetCombatRoomData();
		static void setFinishRoom(bool value);
		static void setCanPassNextRoom(bool value);
		static void setPlayerTriggerNextRoom(bool value);

		// Reset general data
		static void ResetBooleans();
	};
}