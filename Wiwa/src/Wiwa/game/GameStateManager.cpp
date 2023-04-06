#include <wipch.h>
#include "GameStateManager.h"
#include "Wiwa/scene/SceneManager.h"

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
	bool GameStateManager::debug = true;
	SceneId GameStateManager::s_CurrentRoomIndx;
	SceneId GameStateManager::s_RoomsToShop;
	SceneId GameStateManager::s_RoomsToBoss;
	SceneId GameStateManager::s_IntroductionRoom;
	SceneId GameStateManager::s_LastCombatRoom;
	SceneId GameStateManager::s_LastRewardRoom;
	SceneId GameStateManager::s_LastShopRoom;
	std::vector<int> GameStateManager::s_CombatRooms;
	std::vector<int> GameStateManager::s_RewardRooms;
	std::vector<int> GameStateManager::s_ShopRooms;
	int GameStateManager::s_CurrentRoomsCount;
	DefaultCharacterSettings GameStateManager::s_CharacterSettings[2];
	int GameStateManager::s_CurrentCharacter = 0;
	float GameStateManager::s_GamepadDeadzone = 0.f;
	EntityId GameStateManager::s_PlayerId = 0;
	EntityManager::ComponentIterator GameStateManager::s_CharacterStats;
	Scene* GameStateManager::s_CurrentScene = nullptr;

	void GameStateManager::ChangeRoomState(RoomState room_state)
	{
		s_RoomState = room_state;
	}

	void GameStateManager::SaveProgression()
	{
		
		if(debug)
			WI_CORE_INFO("Saving player progression");

		JSONDocument doc;
		EntityManager& em = s_CurrentScene->GetEntityManager();
		Character* character = em.GetComponent<Character>(s_PlayerId);
		if (character)
		{
			doc.AddMember("max_health", character->MaxHealth);
			doc.AddMember("health", character->Health);
			doc.AddMember("max_shield", character->MaxShield);
			doc.AddMember("shield", character->Shield);
			doc.AddMember("damage", character->Damage);
			doc.AddMember("rof", character->RateOfFire);
			doc.AddMember("speed", character->Speed);
			doc.AddMember("dash_distance", character->DashDistance);
			doc.AddMember("dash_speed", character->DashSpeed);
			doc.AddMember("dash_cooldown", character->DashCooldown);
			doc.AddMember("walk_threshold", character->WalkTreshold);
		}
		doc.save_file("config/player_data.json");

		if(debug)
			WI_CORE_INFO("Player progression saved");
	}

	void GameStateManager::LoadProgression()
	{
		if (debug)
			WI_CORE_INFO("Loading player progression");
		EntityManager& em = s_CurrentScene->GetEntityManager();
		Character* character = em.GetComponent<Character>(s_PlayerId);
		JSONDocument doc("config/player_data.json");
		if (!character)
			return;
		if (doc.IsObject())
		{
			if (doc.HasMember("max_health"))
				character->MaxHealth = doc["max_health"].as_int();
			if (doc.HasMember("health"))
				character->Health = doc["health"].as_int();
			if (doc.HasMember("max_shield"))
				character->MaxShield = doc["max_shield"].as_int();
			if (doc.HasMember("shield"))
				character->Shield = doc["shield"].as_int();
			if (doc.HasMember("damage"))
				character->Damage = doc["damage"].as_int();
			if (doc.HasMember("rof"))
				character->RateOfFire = doc["rof"].as_float();
			if (doc.HasMember("speed"))
				character->Speed = doc["speed"].as_float();
			if (doc.HasMember("dash_distance"))
				character->DashDistance = doc["dash_distance"].as_float();
			if (doc.HasMember("dash_speed"))
				character->DashSpeed = doc["dash_speed"].as_float();
			if (doc.HasMember("dash_cooldown"))
				character->DashCooldown = doc["dash_cooldown"].as_float();
			if (doc.HasMember("walk_threshold"))
				character->WalkTreshold = doc["walk_threshold"].as_float();
		}
		if (debug)
			WI_CORE_INFO("Player progression loaded");
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
		em.RegisterComponent<WavesSpawner>();
		size_t size = 0;
		ComponentHash cmpHash = FNV1A_HASH("WavesSpawner");
		Wiwa::WavesSpawner* enemySpawnerList = (Wiwa::WavesSpawner*)em.GetComponentsByHash(cmpHash, &size);

		s_TotalSpawners = 0;
		s_SpawnersFinished = 0;
		if (enemySpawnerList)
		{
			for (int i = 0; i < size; i++)
			{
				if (em.IsComponentRemovedByHash(cmpHash, i)) {
					//WI_INFO("Removed at: [{}]", i);
				}
				else
				{
					s_TotalSpawners += 1;

					Wiwa::WavesSpawner* c = &enemySpawnerList[i];
					if (c)
					{
						if (c->hasFinished)
							s_SpawnersFinished += 1;
					}
				}
			}
			s_HasFinshedRoom = (s_SpawnersFinished == s_TotalSpawners);
			s_CanPassNextRoom = s_HasFinshedRoom;
		}
	

		if (s_HasFinshedRoom)
			ChangeRoomState(RoomState::STATE_FINISHED);
		else
			ChangeRoomState(RoomState::STATE_STARTED);
	}

	void GameStateManager::StartRun()
	{
		if (debug) WI_INFO("GAME STATE: StartRun()");
		SaveProgression();
		StartNewRoom();
	}

	void GameStateManager::EndRun()
	{
		if (debug) WI_INFO("GAME STATE: EndRun()");
		SetRoomType(RoomType::NONE);
	}

	void GameStateManager::InitHub()
	{
		if (debug)
			WI_INFO("GAME STATE: InitHub()");
		SetRoomType(RoomType::ROOM_HUB);
		SetRoomState(RoomState::STATE_FINISHED);
		InitPlayerData();
		s_CurrentRoomsCount = 3;
		s_RoomsToBoss = 20;
		s_RoomsToShop = 10;
	}

	void GameStateManager::InitPlayerData()
	{
		if (debug)
			WI_CORE_INFO("Init progression");
	
		EntityManager& em = s_CurrentScene->GetEntityManager();
		EntityManager::ComponentIterator it = em.GetComponentIterator<Character>(s_PlayerId);
		Character* character = (Character*)em.GetComponentByIterator(it);
		JSONDocument doc("config/room_data.json");
		if (!character)
		{
			WI_ERROR("Character was nullptr");
			return; 
		}
		if (doc.IsObject())
		{
			if (doc.HasMember("starlord") && doc.HasMember("rocket"))
			{
				JSONValue characterDoc = doc["starlord"];
				if (s_CurrentCharacter == 1)
					characterDoc = doc["rocket"];
				if (characterDoc.HasMember("max_health"))
					character->MaxHealth = characterDoc["max_health"].as_int();
				character->Health = character->MaxHealth;
				if (characterDoc.HasMember("max_shield"))
					character->MaxShield = characterDoc["max_shield"].as_int();
				character->Shield = character->MaxShield;
				if (characterDoc.HasMember("shield"))
					character->Shield = characterDoc["shield"].as_int();
				if (characterDoc.HasMember("damage"))
					character->Damage = characterDoc["damage"].as_int();
				if (characterDoc.HasMember("rof"))
					character->RateOfFire = characterDoc["rof"].as_float();
				if (characterDoc.HasMember("speed"))
					character->Speed = characterDoc["speed"].as_float();
				if (characterDoc.HasMember("dash_distance"))
					character->DashDistance = characterDoc["dash_distance"].as_float();
				if (characterDoc.HasMember("dash_speed"))
					character->DashSpeed = characterDoc["dash_speed"].as_float();
				if (characterDoc.HasMember("dash_cooldown"))
					character->DashCooldown = characterDoc["dash_cooldown"].as_float();
				if (characterDoc.HasMember("walk_threshold"))
					character->WalkTreshold = characterDoc["walk_threshold"].as_float();
			}
		}
		if (debug)
			WI_CORE_INFO("Player init loaded");
	}

	void GameStateManager::Die()
	{
		if (debug)
			WI_CORE_INFO("Player dead");
		Wiwa::GuiManager& gm = Wiwa::SceneManager::getActiveScene()->GetGuiManager();
		gm.canvas.at(0)->SwapActive(); //Swap active of normal HUD canvas
		gm.canvas.at(3)->SwapActive(); //Activate death UI
		SceneManager::PauseCurrentScene();
		
		//TODO: @Alejandro Pop the deadth menu
	}

	void GameStateManager::StartNewRoom()
	{
		if (debug)
			WI_INFO("GAME STATE: StartNewRoom()");
		s_PlayerTriggerNext = false;
		NextRoom();

		if (s_RoomType == RoomType::ROOM_REWARD || s_RoomType == RoomType::ROOM_SHOP || s_RoomType == RoomType::ROOM_HUB)
		{
			s_HasFinshedRoom = true;
			s_CanPassNextRoom = true;
		}
	}

	void GameStateManager::SetPlayerId(EntityId id, Scene* scene)
	{
		s_PlayerId = id;
		s_CurrentScene = scene;
		s_CharacterStats = scene->GetEntityManager().GetComponentIterator<Character>(id);
		WI_CORE_INFO("Player id set to {}", id);
	}

	void GameStateManager::EndCurrentRoom()
	{
		if (debug) WI_INFO("GAME STATE: EndCurrentRoom()");
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
		if (debug) WI_INFO("GAME STATE: ResetCombatRoomData()");
		Wiwa::EntityManager& em = Wiwa::SceneManager::getActiveScene()->GetEntityManager();
		em.RegisterComponent<WavesSpawner>();
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
		if (debug) WI_INFO("GAME STATE: ResetBooleans()");
		s_HasFinshedRoom = false;
		s_CanPassNextRoom = false;
		s_PlayerTriggerNext = false;
	}

	int GameStateManager::NextRoom()
	{
		WI_INFO("ROOM STATE: NextRoom()");
		if (s_CurrentRoomsCount > s_RoomsToShop)
		{
			GameStateManager::SetRoomType(RoomType::ROOM_HUB);
			SceneManager::ChangeSceneByIndex(s_IntroductionRoom); // Hardcoded hub index (intro scene)
			return 1;
		}

		RoomType type = GameStateManager::GetType();
		switch (type)
		{
		case Wiwa::RoomType::NONE:
			break;
		case Wiwa::RoomType::ROOM_HUB:
		{
			WI_INFO("ROOM STATE: NEXT ROOM COMBAT");
			GameStateManager::SetRoomType(RoomType::ROOM_COMBAT);
			GameStateManager::SetRoomState(RoomState::STATE_STARTED);
			int nextRoom = RAND(0, s_CombatRooms.size() - 1);
			SceneId id = s_CombatRooms[nextRoom];
			SceneManager::ChangeSceneByIndex(id);
			break;
		}
		case Wiwa::RoomType::ROOM_COMBAT:
		{
			WI_INFO("ROOM STATE: NEXT ROOM ROOM_REWARD");
			GameStateManager::SetRoomType(RoomType::ROOM_REWARD);
			GameStateManager::SetRoomState(RoomState::STATE_FINISHED);
			int nextRoom = s_LastRewardRoom;
			/*while (nextRoom == s_LastRewardRoom)
			{
			}*/
			nextRoom = RAND(0, s_RewardRooms.size() - 1);

			s_LastRewardRoom = nextRoom;
			SceneId id = s_RewardRooms[nextRoom];
			SceneManager::ChangeSceneByIndex(id);
			break;
		}
		case Wiwa::RoomType::ROOM_REWARD:
		{
			WI_INFO("ROOM STATE: NEXT ROOM ROOM_COMBAT");
			GameStateManager::SetRoomType(RoomType::ROOM_COMBAT);
			GameStateManager::SetRoomState(RoomState::STATE_FINISHED);
			int nextRoom = s_LastCombatRoom;
			/*while (nextRoom == s_LastCombatRoom)
			{
				
			}*/
			nextRoom = RAND(0, s_CombatRooms.size() - 1);
			s_LastCombatRoom = nextRoom;
			SceneId id = s_CombatRooms[nextRoom];
			SceneManager::ChangeSceneByIndex(id);

			s_RoomsToBoss--;
			s_RoomsToShop--;

			if (s_RoomsToShop == 0)
			{
				WI_INFO("ROOM STATE: NEXT ROOM ROOM_SHOP");
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
			break;
		}
		case Wiwa::RoomType::ROOM_BOSS:
		{
			WI_INFO("ROOM STATE: NEXT ROOM ROOM_BOSS");
			SceneManager::ChangeSceneByName("RunEnd");
			break;
		}
		case Wiwa::RoomType::ROOM_SHOP:
		{
			if (s_RoomsToBoss == 0)
			{
				WI_INFO("ROOM STATE: NEXT ROOM ROOM_BOSS");
				GameStateManager::SetRoomType(RoomType::ROOM_BOSS);
				//SceneManager::LoadScene("RunBoss");
			}
			else
			{
				WI_INFO("ROOM STATE: NEXT ROOM ROOM_COMBAT");
				GameStateManager::SetRoomType(RoomType::ROOM_COMBAT);
				GameStateManager::SetRoomState(RoomState::STATE_STARTED);
				int nextRoom = RAND(0, s_CombatRooms.size() - 1);
				SceneId id = s_CombatRooms[nextRoom];
				SceneManager::ChangeSceneByIndex(id);
			}
			break;
		}
		default:
			break;
		}
		s_CurrentRoomsCount--;
		return 1;
		//SceneManager::getActiveScene()->GetPhysicsManager().OnLoad("game_tags");
		
	}



	void GameStateManager::SerializeData()
	{
		JSONDocument doc;
		doc.AddMember("intro", s_IntroductionRoom);
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

		JSONValue starlord = doc.AddMemberObject("starlord");
		starlord.AddMember("max_health", s_CharacterSettings[0].MaxHealth);
		starlord.AddMember("max_shield", s_CharacterSettings[0].MaxShield);
		starlord.AddMember("damage", s_CharacterSettings[0].Damage);
		starlord.AddMember("rof", s_CharacterSettings[0].RateOfFire);
		starlord.AddMember("speed", s_CharacterSettings[0].Speed);
		starlord.AddMember("dash_speed", s_CharacterSettings[0].DashSpeed);
		starlord.AddMember("dash_distance", s_CharacterSettings[0].DashDistance);
		starlord.AddMember("dash_cooldown", s_CharacterSettings[0].DashCoolDown);
		starlord.AddMember("walk_threshold", s_CharacterSettings[0].WalkTreshold);


		JSONValue rocket = doc.AddMemberObject("rocket");
		rocket.AddMember("max_health", s_CharacterSettings[1].MaxHealth);
		rocket.AddMember("max_shield", s_CharacterSettings[1].MaxShield);
		rocket.AddMember("damage", s_CharacterSettings[1].Damage);
		rocket.AddMember("rof", s_CharacterSettings[1].RateOfFire);
		rocket.AddMember("speed", s_CharacterSettings[1].Speed);
		rocket.AddMember("dash_speed", s_CharacterSettings[1].DashSpeed);
		rocket.AddMember("dash_distance", s_CharacterSettings[1].DashDistance);
		rocket.AddMember("dash_cooldown", s_CharacterSettings[1].DashCoolDown);
		rocket.AddMember("walk_threshold", s_CharacterSettings[1].WalkTreshold);

		doc.AddMember("gamepad_deadzone", s_GamepadDeadzone);
		doc.AddMember("current_character", s_CurrentCharacter);

		doc.save_file("config/room_data.json");
	}

	void GameStateManager::DeserializeData()
	{
		JSONDocument doc;
		doc.load_file("config/room_data.json");
		if (doc.HasMember("intro")) {
			s_IntroductionRoom = doc["intro"].as_int();
		}

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

		if (doc.HasMember("starlord"))
		{
			JSONValue starlord = doc["starlord"];
			s_CharacterSettings[0].MaxHealth = starlord["max_health"].as_int();
			s_CharacterSettings[0].MaxShield = starlord["max_shield"].as_int();
			s_CharacterSettings[0].Damage = starlord["damage"].as_int();
			s_CharacterSettings[0].RateOfFire = starlord["rof"].as_float();
			s_CharacterSettings[0].Speed = starlord["speed"].as_float();
			s_CharacterSettings[0].DashSpeed = starlord["dash_speed"].as_float();
			s_CharacterSettings[0].DashDistance = starlord["dash_distance"].as_float();
			s_CharacterSettings[0].DashCoolDown = starlord["dash_cooldown"].as_float();
			s_CharacterSettings[0].WalkTreshold = starlord["walk_threshold"].as_float();
		}

		if (doc.HasMember("rocket"))
		{
			JSONValue rocket = doc["rocket"];
			s_CharacterSettings[1].MaxHealth = rocket["max_health"].as_int();
			s_CharacterSettings[1].MaxShield = rocket["max_shield"].as_int();
			s_CharacterSettings[1].Damage = rocket["damage"].as_int();
			s_CharacterSettings[1].RateOfFire = rocket["rof"].as_float();
			s_CharacterSettings[1].Speed = rocket["speed"].as_float();
			s_CharacterSettings[1].DashSpeed = rocket["dash_speed"].as_float();
			s_CharacterSettings[1].DashDistance = rocket["dash_distance"].as_float();
			s_CharacterSettings[1].DashCoolDown = rocket["dash_cooldown"].as_float();
			s_CharacterSettings[1].WalkTreshold = rocket["walk_threshold"].as_float();
		}
		if (doc.HasMember("gamepad_deadzone"))
			s_GamepadDeadzone = doc["gamepad_deadzone"].as_float();
		if (doc.HasMember("current_character"))
			s_CurrentCharacter = doc["current_character"].as_int();
	}
}


