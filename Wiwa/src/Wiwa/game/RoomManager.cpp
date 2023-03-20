#include <wipch.h>
#include "RoomManager.h"

#include <Wiwa/scene/SceneManager.h>

namespace Wiwa
{
	std::vector<SceneId> RoomManager::s_CombatRooms;
	std::vector<SceneId> RoomManager::s_RewardRooms;
	std::vector<SceneId> RoomManager::s_ShopRooms;

	SceneId RoomManager::s_CurrentRoomIndx;
	RoomType RoomManager::s_CurrentRoom;
	SceneId RoomManager::s_RoomsToShop;
	SceneId RoomManager::s_RoomsToBoss;
	SceneId RoomManager::s_IntroductionRoom;

	void RoomManager::StartRun()
	{
		s_CurrentRoom = RoomType::ROOM_HUB;
		
		
	}

	void RoomManager::NextRoom()
	{
	}

	void RoomManager::EndRun()
	{
	}

}