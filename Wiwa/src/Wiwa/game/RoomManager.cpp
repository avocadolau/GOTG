#include <wipch.h>
#include "RoomManager.h"

#include <Wiwa/scene/SceneManager.h>
#include <Wiwa/game/GameStateManager.h>

namespace Wiwa
{
	std::vector<SceneId> RoomManager::s_CombatRooms;
	std::vector<SceneId> RoomManager::s_RewardRooms;
	std::vector<SceneId> RoomManager::s_ShopRooms;

	SceneId RoomManager::s_CurrentRoomIndx;
	SceneId RoomManager::s_RoomsToShop;
	SceneId RoomManager::s_RoomsToBoss;
	SceneId RoomManager::s_IntroductionRoom;
	SceneId RoomManager::s_LastCombatRoom;
	SceneId RoomManager::s_LastRewardRoom;
	SceneId RoomManager::s_LastShopRoom;

	void RoomManager::StartRun()
	{
		GameStateManager::SetRoomType(RoomType::ROOM_HUB);
		
	}

	void RoomManager::NextRoom()
	{
		RoomType type = GameStateManager::GetType();
		switch (type)
		{
		case Wiwa::RoomType::NONE:
			break;
		case Wiwa::RoomType::ROOM_HUB:
		{
			GameStateManager::SetRoomType(RoomType::ROOM_COMBAT);
			int nextRoom = RAND(0, s_CombatRooms.size() - 1);
			SceneId id = s_CombatRooms[nextRoom];
			SceneManager::ChangeSceneByIndex(id);
		}break;
		case Wiwa::RoomType::ROOM_COMBAT:
		{
			GameStateManager::SetRoomType(RoomType::ROOM_REWARD);
			int nextRoom = s_LastRewardRoom;
			while (nextRoom == s_LastRewardRoom)
			{
				nextRoom = RAND(0, s_RewardRooms.size() - 1);
			}
			s_LastRewardRoom = nextRoom;
			SceneId id = s_RewardRooms[nextRoom];
			SceneManager::ChangeSceneByIndex(id);
		}break;
		case Wiwa::RoomType::ROOM_REWARD:
		{
			GameStateManager::SetRoomType(RoomType::ROOM_COMBAT);
			int nextRoom = s_LastCombatRoom;
			while (nextRoom == s_LastCombatRoom)
			{
				nextRoom = RAND(0, s_CombatRooms.size() - 1);
			}
			s_LastCombatRoom = nextRoom;
			SceneId id = s_CombatRooms[nextRoom];
			SceneManager::ChangeSceneByIndex(id);
		}break;
		case Wiwa::RoomType::ROOM_BOSS:
			break;
		case Wiwa::RoomType::ROOM_SHOP: 
		{

		}break;
		default:
			break;
		}
	}

	void RoomManager::EndRun()
	{
	}

}