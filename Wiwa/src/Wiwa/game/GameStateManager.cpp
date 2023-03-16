#include <wipch.h>
#include "GameStateManager.h"
#include "Wiwa/scene/SceneManager.h"

namespace Wiwa
{
	RoomType GameStateManager::s_RoomType = RoomType::NONE;
	RoomState GameStateManager::s_RoomState = RoomState::NONE;
	bool GameStateManager::s_HasFinshedLevel = false;
	bool GameStateManager::s_CanPassNextLevel = false;
	bool GameStateManager::s_PlayerTriggerNext = false;
	std::vector<GameEvent> GameStateManager::s_PreStartEvents;
	std::vector<GameEvent> GameStateManager::s_MiddleEvents;
	std::vector<GameEvent> GameStateManager::s_PostFinishedEvents;

	void GameStateManager::ChangeRoomState(RoomState room_state)
	{
		s_RoomState = room_state;
		EntityManager& em = SceneManager::getActiveScene()->GetEntityManager();
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
		//LogRoomState();

		if (s_HasFinshedLevel && s_CanPassNextLevel == false)
			ChangeRoomState(RoomState::STATE_FINISHED);
		else if (s_HasFinshedLevel && s_CanPassNextLevel)
			ChangeRoomState(RoomState::STATE_AWAITING_NEXT);

		if (s_PlayerTriggerNext && s_RoomState == RoomState::STATE_AWAITING_NEXT)
		{
			EndCurrentRoom();
			StartNewRoom();
		}
	}

	void GameStateManager::QuerySpawners()
	{
	}

	void GameStateManager::StartNewRoom()
	{
		ChangeRoomState(RoomState::STATE_STARTING);
		LoadProgression();
	}

	void GameStateManager::EndCurrentRoom()
	{
		ChangeRoomState(RoomState::STATE_TRANSITIONING);
		SaveProgression();
	}

	void GameStateManager::LogRoomState()
	{
		switch (s_RoomState)
		{
		case Wiwa::RoomState::NONE:
			WI_INFO("<-- RoomState --> {}", "NONE");
			break;
		case Wiwa::RoomState::STATE_STARTING:
			WI_INFO("<-- RoomState --> {}", "STATE_STARTING");
			break;
		case Wiwa::RoomState::STATE_MIDDLE:
			WI_INFO("<-- RoomState --> {}", "STATE_MIDDLE");
			break;
		case Wiwa::RoomState::STATE_FINISHED:
			WI_INFO("<-- RoomState --> {}", "STATE_FINISHED");
			break;
		case Wiwa::RoomState::STATE_AWAITING_NEXT:
			WI_INFO("<-- RoomState --> {}", "STATE_AWAITING_NEXT");
			break;
		case Wiwa::RoomState::STATE_TRANSITIONING:
			WI_INFO("<-- RoomState --> {}", "STATE_TRANSITIONING");
			break;
		default:
			break;
		}
	}
}


