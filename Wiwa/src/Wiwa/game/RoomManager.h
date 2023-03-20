#pragma once

#include <Wiwa/core/Core.h>
#include <vector>
#include <Wiwa/game/GameStateManager.h>

typedef size_t SceneId;

namespace Wiwa
{

	class WI_API RoomManager
	{
	public:
		static SceneId s_CurrentRoomIndx;
		static RoomType s_CurrentRoom;
		static SceneId s_RoomsToShop;
		static SceneId s_RoomsToBoss;
		static SceneId s_IntroductionRoom;
		static std::vector<SceneId> s_CombatRooms;
		static std::vector<SceneId> s_RewardRooms;
		static std::vector<SceneId> s_ShopRooms;

		static void StartRun();
		static void NextRoom();
		static void EndRun();
	};
}