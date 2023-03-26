#pragma once

#include <Wiwa/core/Core.h>

#include <Wiwa/ecs/EntityManager.h>
#include <Wiwa/utilities/filesystem/FileSystem.h>

#include <Wiwa/ecs/components/game/Character.h>
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
	struct DefaultCharacterSettings
	{
		int MaxHealth;
		int MaxShield;

		int Damage;
		float RateOfFire;

		float Speed;
		float DashDistance;
		float DashSpeed;
		float DashCoolDown;
		float WalkTreshold;
	};
	struct CharacterStats
	{
		int MaxHealth;
		int Health;
		int MaxShield;
		int Shield;

		int Damage;
		float RateOfFire;

		float Speed;
		float DashDistance;
		float DashSpeed;
		float DashCoolDown;
		float WalkTreshold;
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
		static bool debug;
		// Save & Load Overall Player Progression
		static void SaveProgression(void* scene);
		static void LoadProgression(void* scene);

		static void UpdateRoomState();
		static void UpdateCombatRoom();

		// Run and rooms
		static void StartRun();
		static void EndRun();
		static void InitHub(void* scene);
		static void InitPlayerData(void* scene);
		static void StartNewRoom();
		static void SetPlayerId(EntityId id);
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

		static int NextRoom();



		static void SerializeData();
		static void DeserializeData();


	public:
		static DefaultCharacterSettings s_CharacterSettings[2];
		static CharacterStats s_CharacterStats;
		static int s_CurrentCharacter;
		static float s_GamepadDeadzone;

		static SceneId s_CurrentRoomIndx;
		static SceneId s_RoomsToShop;
		static SceneId s_RoomsToBoss;
		static SceneId s_IntroductionRoom;
		static SceneId s_LastCombatRoom;
		static SceneId s_LastRewardRoom;
		static SceneId s_LastShopRoom;
		static std::vector<int> s_CombatRooms;
		static std::vector<int> s_RewardRooms;
		static std::vector<int> s_ShopRooms;
		static int s_CurrentRoomsCount;

		static EntityId s_PlayerId;
	};
}