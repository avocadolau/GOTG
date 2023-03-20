#include <wipch.h>
#include "RoomManager.h"

#include <Wiwa/scene/SceneManager.h>
#include <Wiwa/game/GameStateManager.h>

#include <Wiwa/utilities/json/JSONDocument.h>

namespace Wiwa
{
	std::vector<int> RoomManager::s_CombatRooms;
	std::vector<int> RoomManager::s_RewardRooms;
	std::vector<int> RoomManager::s_ShopRooms;

	SceneId RoomManager::s_CurrentRoomIndx;
	SceneId RoomManager::s_RoomsToShop = 10;
	SceneId RoomManager::s_RoomsToBoss = 20;
	SceneId RoomManager::s_IntroductionRoom;
	SceneId RoomManager::s_LastCombatRoom;
	SceneId RoomManager::s_LastRewardRoom;
	SceneId RoomManager::s_LastShopRoom;
	int RoomManager::s_CurrentRoomsCount = 3;

	void RoomManager::InitHub()
	{
		GameStateManager::SetRoomType(RoomType::ROOM_HUB);
		GameStateManager::SetRoomState(RoomState::STATE_FINISHED);
	}

	int RoomManager::NextRoom()
	{
		if (s_CurrentRoomsCount <= 0)
		{
			GameStateManager::SetRoomType(RoomType::ROOM_HUB);
			SceneManager::ChangeSceneByIndex(2);
			return 1;
		}

		RoomType type = GameStateManager::GetType();
		switch (type)
		{
		case Wiwa::RoomType::NONE:
			break;
		case Wiwa::RoomType::ROOM_HUB:
		{
			GameStateManager::SetRoomType(RoomType::ROOM_COMBAT);
			GameStateManager::SetRoomState(RoomState::STATE_STARTED);
			int nextRoom = RAND(0, s_CombatRooms.size() - 1);
			SceneId id = s_CombatRooms[nextRoom];
			SceneManager::ChangeSceneByIndex(id);
		}break;
		case Wiwa::RoomType::ROOM_COMBAT:
		{
			GameStateManager::SetRoomType(RoomType::ROOM_REWARD);
			GameStateManager::SetRoomState(RoomState::STATE_FINISHED);
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
			GameStateManager::SetRoomState(RoomState::STATE_FINISHED);
			int nextRoom = s_LastCombatRoom;
			while (nextRoom == s_LastCombatRoom)
			{
				nextRoom = RAND(0, s_CombatRooms.size() - 1);
			}
			s_LastCombatRoom = nextRoom;
			SceneId id = s_CombatRooms[nextRoom];
			SceneManager::ChangeSceneByIndex(id);

			s_RoomsToBoss--;
			s_RoomsToShop--;
			
			if (s_RoomsToShop == 0)
			{
				GameStateManager::SetRoomType(RoomType::ROOM_SHOP);
				GameStateManager::SetRoomState(RoomState::STATE_FINISHED);
				nextRoom = s_LastShopRoom;
				while (nextRoom == s_LastShopRoom)
				{
					nextRoom = RAND(0, s_ShopRooms.size() - 1);
				}
				id = s_ShopRooms[id];
				SceneManager::ChangeSceneByIndex(id);
				s_RoomsToShop = 10;
			}
		}break;
		case Wiwa::RoomType::ROOM_BOSS:
		{
			SceneManager::ChangeSceneByName("RunEnd");
		}break;
		case Wiwa::RoomType::ROOM_SHOP: 
		{
			if (s_RoomsToBoss == 0)
			{
				GameStateManager::SetRoomType(RoomType::ROOM_BOSS);
				SceneManager::LoadScene("RunBoss");
			}
			else
			{
				GameStateManager::SetRoomType(RoomType::ROOM_COMBAT);
				GameStateManager::SetRoomState(RoomState::STATE_STARTED);
				int nextRoom = RAND(0, s_CombatRooms.size() - 1);
				SceneId id = s_CombatRooms[nextRoom];
				SceneManager::ChangeSceneByIndex(id);
			}
			
		}break;
		default:
			break;
		}
		s_CurrentRoomsCount--;
		return 1;
		//SceneManager::getActiveScene()->GetPhysicsManager().OnLoad("game_tags");
	}

	void RoomManager::EndRun()
	{
		GameStateManager::SetRoomType(RoomType::NONE);
	}

	void RoomManager::SerializeData()
	{
		JSONDocument doc;
		JSONValue combatRooms = doc.AddMemberArray("combat");
		for (size_t i = 0; i < s_CombatRooms.size(); i++)
		{
			std::string indx = std::to_string(i);
			combatRooms.PushBack(s_CombatRooms[i]);
		}
		JSONValue rewardRooms = doc.AddMemberArray("reward");
		for (size_t i = 0; i < s_RewardRooms.size(); i++)
		{
			std::string indx = std::to_string(i);
			rewardRooms.PushBack(s_RewardRooms[i]);
		}
		JSONValue shopRooms = doc.AddMemberArray("shop");
		for (size_t i = 0; i < s_ShopRooms.size(); i++)
		{
			std::string indx = std::to_string(i);
			shopRooms.PushBack(s_ShopRooms[i]);
		}
		doc.save_file("config/room_data.json");
	}

	void RoomManager::DeserializeData()
	{
		JSONDocument doc;
		doc.load_file("config/room_data.json");
		if (doc.HasMember("combat")) {

			JSONValue scene_list = doc["combat"];

			if (scene_list.IsArray()) {
				size_t size = scene_list.Size();
				for (size_t i = 0; i < size; i++) {
					JSONValue scene = scene_list[(uint32_t)i];
					s_CombatRooms.push_back(scene.as_int());
				}
			}
		}

		if (doc.HasMember("reward")) {

			JSONValue scene_list = doc["reward"];

			if (scene_list.IsArray()) {
				size_t size = scene_list.Size();
				for (size_t i = 0; i < size; i++) {
					JSONValue scene = scene_list[(uint32_t)i];
					s_RewardRooms.push_back(scene.as_int());
				}
			}
		}

		if (doc.HasMember("shop")) {

			JSONValue scene_list = doc["shop"];

			if (scene_list.IsArray()) {
				size_t size = scene_list.Size();
				for (size_t i = 0; i < size; i++) {
					JSONValue scene = scene_list[(uint32_t)i];
					s_ShopRooms.push_back(scene.as_int());
				}
			}
		}
	}

}