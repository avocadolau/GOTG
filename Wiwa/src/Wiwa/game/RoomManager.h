#pragma once

#include <Wiwa/core/Core.h>
#include <vector>

typedef size_t SceneId;

namespace Wiwa
{
	enum class RoomType
	{
		INTRODUCTION_ROOM,
		REWARD_ROOM,
		COMBAT_ROOM,
		SHOP,
		BOSS_ROOM
	};
	class WI_API RoomManager
	{
	public:
		static SceneId s_CurrentRoomIndx;
		static RoomType s_CurrentRoom;
		static int s_RoomsToShop;
		static int s_RoomsToBoss;
		static std::vector<SceneId> s_CombatRooms;
		static std::vector<SceneId> s_RewardRooms;
		static std::vector<SceneId> s_ShopRooms;
		static int s_IntroductionRoom;


	};
}