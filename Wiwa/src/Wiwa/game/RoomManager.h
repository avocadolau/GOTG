#pragma once

#include <Wiwa/core/Core.h>
#include <vector>


typedef size_t SceneId;

namespace Wiwa
{

	class WI_API RoomManager
	{
	public:
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

		static void InitHub();
		static void NextRoom();
		static void EndRun();

		static void SerializeData();
		static void DeserializeData();
	};
}