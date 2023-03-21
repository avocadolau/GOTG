#include <wipch.h>
#include "GameStateManager.h"
#include "Wiwa/scene/SceneManager.h"
#include <Wiwa/game/RoomManager.h>

namespace Wiwa
{
	RoomType GameStateManager::s_RoomType = RoomType::NONE;
	RoomState GameStateManager::s_RoomState = RoomState::NONE;
	bool GameStateManager::s_HasFinshedRoom = false;
	bool GameStateManager::s_CanPassNextRoom = false;
	bool GameStateManager::s_PlayerTriggerNext = false;
	std::vector<GameEvent> GameStateManager::s_PreStartEvents;
	std::vector<GameEvent> GameStateManager::s_MiddleEvents;
	std::vector<GameEvent> GameStateManager::s_PostFinishedEvents;
	int GameStateManager::s_TotalSpawners = 0;
	int GameStateManager::s_SpawnersFinished = 0;

	void GameStateManager::ChangeRoomState(RoomState room_state)
	{
		s_RoomState = room_state;

	}

	void GameStateManager::SaveProgression()
	{
	}

	void GameStateManager::LoadProgression()
	{
	}

	void GameStateManager::SavePlayer(const Character& character)
	{
	}

	void GameStateManager::LoadPlayer(Character& character)
	{
	}

	void GameStateManager::UpdateRoomState()
	{

		if (s_RoomType == RoomType::ROOM_COMBAT)
			UpdateCombatRoom();

		if (s_HasFinshedRoom && s_CanPassNextRoom && s_PlayerTriggerNext)
		{
			ChangeRoomState(RoomState::STATE_AWAITING_NEXT);		
		}

		if (s_PlayerTriggerNext && s_RoomState == RoomState::STATE_AWAITING_NEXT)
		{
			EndCurrentRoom();
			StartNewRoom();
		}
	}

	void GameStateManager::UpdateCombatRoom()
	{
		Wiwa::EntityManager& em = Wiwa::SceneManager::getActiveScene()->GetEntityManager();
		em.RegisterComponent<EnemySpawner>();
		size_t size = 0;
		ComponentHash cmpHash = FNV1A_HASH("EnemySpawner");
		Wiwa::EnemySpawner* enemySpawnerList = (Wiwa::EnemySpawner*)em.GetComponentsByHash(cmpHash, &size);

		s_TotalSpawners = 0;
		s_SpawnersFinished = 0;
		for (int i = 0; i < size; i++)
		{
			if (em.IsComponentRemovedByHash(cmpHash, i)) {
				//WI_INFO("Removed at: [{}]", i);
			}
			else
			{
				s_TotalSpawners += 1;
				Wiwa::EnemySpawner* c = &enemySpawnerList[i];
				if (c)
				{
					if (c->hasFinished)
						s_SpawnersFinished += 1;
				}
			}
		}

		s_HasFinshedRoom = (s_SpawnersFinished == s_TotalSpawners);
		s_CanPassNextRoom = s_HasFinshedRoom;

		if (s_HasFinshedRoom)
			ChangeRoomState(RoomState::STATE_FINISHED);
		else
			ChangeRoomState(RoomState::STATE_STARTED);
	}

	void GameStateManager::StartRun()
	{
		StartNewRoom();
	}

	void GameStateManager::EndRun()
	{
		Wiwa::RoomManager::EndRun();
	}

	void GameStateManager::InitHub()
	{
		Wiwa::RoomManager::InitHub();
	}

	void GameStateManager::StartNewRoom()
	{
		s_PlayerTriggerNext = false;
		LoadProgression();
		Wiwa::RoomManager::NextRoom();

		if (s_RoomType == RoomType::ROOM_REWARD || s_RoomType == RoomType::ROOM_SHOP || s_RoomType == RoomType::ROOM_HUB)
		{
			s_HasFinshedRoom = true;
			s_CanPassNextRoom = true;
		}
	}

	void GameStateManager::EndCurrentRoom()
	{
		if (s_RoomType == RoomType::ROOM_COMBAT)
		{
			ResetCombatRoomData();
			ResetBooleans();
		}
		ChangeRoomState(RoomState::STATE_TRANSITIONING);
		SaveProgression();
	}

	void GameStateManager::LogRoomState()
	{
		WI_INFO("<-- RoomState --> {}", s_RoomStateStr[(int)s_RoomState]);
	}

	const char* GameStateManager::GetRoomState()
	{
		return s_RoomStateStr[(int)s_RoomState];
	}
	const char* GameStateManager::GetRoomType()
	{
		return s_RoomTypeStr[(int)s_RoomType];
	}

	void GameStateManager::ResetCombatRoomData()
	{
		Wiwa::EntityManager& em = Wiwa::SceneManager::getActiveScene()->GetEntityManager();
		em.RegisterComponent<EnemySpawner>();
		s_TotalSpawners = 0;
		s_SpawnersFinished = 0;
	}

	void GameStateManager::setFinishRoom(bool value)
	{
		s_HasFinshedRoom = value;
	}

	void GameStateManager::setCanPassNextRoom(bool value)
	{
		s_CanPassNextRoom = value;
	}

	void GameStateManager::setPlayerTriggerNextRoom(bool value)
	{
		s_PlayerTriggerNext = value;
	}

	void GameStateManager::ResetBooleans()
	{
		s_HasFinshedRoom = false;
		s_CanPassNextRoom = false;
		s_PlayerTriggerNext = false;
	}
}


