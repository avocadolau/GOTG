#include <wipch.h>
#include "GameStateManager.h"
#include "Wiwa/scene/SceneManager.h"
#include "Items/ItemManager.h"
#include <Wiwa/ecs/components/game/Character.h>
#include "Achievements/AchievementsManager.h"
#include <Wiwa/ecs/components/game/items/Item.h>
#include <Wiwa/ecs/components/game/wave/WaveSpawner.h>
#include <Wiwa/ecs/systems/MeshRenderer.h>
#include <Wiwa/audio/Audio.h>

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
	
	int GameStateManager::s_RoomsToShop = 5;
	int GameStateManager::s_RoomsToBoss = 5;

	SceneId GameStateManager::s_BossRoomIndx;
	SceneId GameStateManager::s_HUBRoomIndx;
	SceneId GameStateManager::s_CurrentRoomIndx;
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
	
	int GameStateManager::s_ActiveSkillChances = 20;
	int GameStateManager::s_BuffChances = 20;
	int GameStateManager::s_PassiveSkillChances = 35;
	int GameStateManager::s_NPCRoomChances = 25;
	int GameStateManager::s_EnemyDropChances = 100;

	EntityManager::ComponentIterator GameStateManager::s_CharacterStats;
	Scene* GameStateManager::s_CurrentScene = nullptr;
	Inventory* GameStateManager::s_PlayerInventory =  new Inventory();
	GamePoolingManager* GameStateManager::s_PoolManager = new GamePoolingManager();

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
		s_PlayerInventory->Serialize(&doc);
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
		s_PlayerInventory->Deserialize(&doc);
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
		size_t size = 0;
		Wiwa::WaveSpawner* enemySpawnerList = nullptr;
		enemySpawnerList = em.GetComponents<WaveSpawner>(&size);

		s_TotalSpawners = 0;
		s_SpawnersFinished = 0;
		if (enemySpawnerList)
		{
			for (int i = 0; i < size; i++)
			{
				if (em.IsComponentRemoved<WaveSpawner>(i)) {
					//WI_INFO("Removed at: [{}]", i);
				}
				else
				{
					s_TotalSpawners += 1;

					Wiwa::WaveSpawner* spawner = &enemySpawnerList[i];
					if (spawner)
					{
						if (spawner->hasFinished)
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
		s_PlayerInventory->Clear();
	}

	void GameStateManager::InitHub()
	{
		if (debug)
			WI_INFO("GAME STATE: InitHub()");
		SetRoomType(RoomType::ROOM_HUB);
		SetRoomState(RoomState::STATE_FINISHED);
		InitPlayerData();
	}

	void GameStateManager::InitPlayerData()
	{
		if (debug)
			WI_CORE_INFO("Init progression");
		Character* character = GetPlayerCharacterComp();
		
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

	void GameStateManager::Update()
	{
		OPTICK_EVENT("Game state manager update");
		s_PlayerInventory->Update();
	}

	void GameStateManager::Die()
	{
		if (debug)
			WI_CORE_INFO("Player dead");		
		EndRun();
	}

	Character* GameStateManager::GetPlayerCharacterComp()
	{
		EntityManager& em = s_CurrentScene->GetEntityManager();
		EntityManager::ComponentIterator it = em.GetComponentIterator<Character>(s_PlayerId);
		return (Character*)em.GetComponentByIterator(it);
	}

	void GameStateManager::DamagePlayer(uint32_t damage)
	{
		Wiwa::GuiManager& gm = s_CurrentScene->GetGuiManager();
		Character* character = GetPlayerCharacterComp();
		if (!character)
		{
			WI_CORE_CRITICAL("Player can't take damage because character is nullptr");
			return;
		}
		// The damage from this function doesn't expand to the health
		// this means that if there's remaining shield the damage doesn't apply
		// to the health even when this surpases the shield ammount

		// If there's no shield we take damage from the health

		MeshRenderer* renderer = s_CurrentScene->GetEntityManager().GetSystem<MeshRenderer>(s_PlayerId);

		
		if (character->Shield <= 0)
		{
			character->Health -= damage;
			if (renderer != nullptr) 
			{
				renderer->GetMaterial()->SetUniformData("u_Color", glm::vec4(1.0f, 1.0f, 1.0f, 1.0f));
				renderer->GetMaterial()->SetUniformData("u_Hit", true);
				renderer->Update();
				renderer->GetMaterial()->SetUniformData("u_Hit", false);
			}
			Audio::PostEvent("player_hit");
			if (character->Health <= 0)
			{
				Audio::PostEvent("player_dead");
				Die();
			}
			return;
		}
		
		
		if (character->Shield > 0)
		{
			character->Shield -= damage;
			Audio::PostEvent("player_hit_shield");
			if (renderer != nullptr)
			{
				renderer->GetMaterial()->SetUniformData("u_Color", glm::vec4(0, 0, 1.0f, 1.0f));
				renderer->GetMaterial()->SetUniformData("u_Hit", true);
				renderer->Update();
				renderer->GetMaterial()->SetUniformData("u_Hit", false);
			}
		}

		if (character->Shield <= 0)
			character->Shield = 0;


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
			std::uniform_int_distribution<> dist(0, s_CombatRooms.size() - 1);
			int nextRoom = dist(Application::s_Gen);
			SceneId id = s_CombatRooms[nextRoom];
			SceneManager::ChangeSceneByIndex(id);
			break;
		}
		case Wiwa::RoomType::ROOM_COMBAT:
		{
			WI_INFO("ROOM STATE: NEXT ROOM ROOM_REWARD");
			GameStateManager::SetRoomType(RoomType::ROOM_REWARD);
			GameStateManager::SetRoomState(RoomState::STATE_FINISHED);
			std::uniform_int_distribution<> dist(0, s_RewardRooms.size() - 1);
			int nextRoom = dist(Application::s_Gen);

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
			std::uniform_int_distribution<> dist(0, s_CombatRooms.size() - 1);
			int nextRoom = dist(Application::s_Gen);
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
				
				std::uniform_int_distribution<> dist(0, s_ShopRooms.size() - 1);
				int nextRoom = dist(Application::s_Gen);
				
				id = s_ShopRooms[0];
				SceneManager::ChangeSceneByIndex(id);
			}
			break;
		}
		case Wiwa::RoomType::ROOM_BOSS:
		{
			SceneManager::LoadSceneByIndex(s_HUBRoomIndx);
			break;
		}
		case Wiwa::RoomType::ROOM_SHOP:
		{
			if (s_RoomsToBoss == 0)
			{
				WI_INFO("ROOM STATE: NEXT ROOM ROOM_BOSS");
				GameStateManager::SetRoomType(RoomType::ROOM_BOSS);
				SceneManager::LoadSceneByIndex(s_BossRoomIndx);
			}
			else
			{
				WI_INFO("ROOM STATE: NEXT ROOM ROOM_COMBAT");
				GameStateManager::SetRoomType(RoomType::ROOM_COMBAT);
				GameStateManager::SetRoomState(RoomState::STATE_STARTED);
				std::uniform_int_distribution<> dist(0, s_CombatRooms.size() - 1);
				int nextRoom = dist(Application::s_Gen);
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

	void GameStateManager::CleanUp()
	{
		WI_INFO("Game state manager clean up");
		delete s_PlayerInventory;
		s_RewardRooms.clear();
		s_CombatRooms.clear();
		s_ShopRooms.clear();
		delete s_PoolManager;
	}


	void GameStateManager::SerializeData()
	{
		JSONDocument doc;
		
		doc.AddMember("enemy_item_chance", s_EnemyDropChances);
		doc.AddMember("active_skill_chance", s_ActiveSkillChances);
		doc.AddMember("passive_skill_chance", s_PassiveSkillChances);
		doc.AddMember("buff_chance", s_BuffChances);
		doc.AddMember("npc_chance", s_NPCRoomChances);

		doc.AddMember("hub", s_HUBRoomIndx);
		doc.AddMember("boss", s_BossRoomIndx);
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

		ItemManager::Serialize(&doc);

		AchievementsManager::Serialize(&doc);

		doc.save_file("config/room_data.json");
	}

	void GameStateManager::DeserializeData()
	{
		JSONDocument doc;
		doc.load_file("config/room_data.json");
		
		if(doc.HasMember("enemy_item_chance"))
			s_EnemyDropChances = doc["enemy_item_chance"].as_int();

		if (doc.HasMember("active_skill_chance"))
			s_ActiveSkillChances = doc["active_skill_chance"].as_int();

		if (doc.HasMember("passive_skill_chance"))
			s_PassiveSkillChances = doc["passive_skill_chance"].as_int();

		if (doc.HasMember("buff_chance"))
			s_BuffChances = doc["buff_chance"].as_int();
		
		if (doc.HasMember("npc_chance"))
			s_NPCRoomChances = doc["npc_chance"].as_int();

		if (doc.HasMember("hub")) {
			s_HUBRoomIndx = doc["hub"].as_int();
		}
		if (doc.HasMember("boss"))
		{
			s_BossRoomIndx = doc["boss"].as_int();
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

		Wiwa::ItemManager::Deserialize(&doc);

		AchievementsManager::Deserialize(&doc);
	}
	void GameStateManager::SpawnRandomItem(glm::vec3 position, uint8_t type)
	{
		uint32_t itemRand;
		std::string name;
		int i = 0;
		switch (type)
		{
		case 0:
		{
			std::uniform_int_distribution<> itemRandom(0, ItemManager::GetAbilities().size());
			int randomNum = itemRandom(Application::s_Gen);
			for (const auto& ability : ItemManager::GetAbilities())
			{
				if (i == randomNum)
					name = ability.second.Name;
				i++;
			}
		}break;
		case 1:
		{
			std::uniform_int_distribution<> itemRandom(0, ItemManager::GetSkills().size());
			int randomNum = itemRandom(Application::s_Gen);
			for (const auto& ability : ItemManager::GetSkills())
			{
				if (i == randomNum)
					name = ability.second.Name;
				i++;
			}
		}break;
		case 2:
		{
			std::uniform_int_distribution<> itemRandom(0, ItemManager::GetBuffs().size());
			int randomNum = itemRandom(Application::s_Gen);
			for (const auto& ability : ItemManager::GetBuffs())
			{
				if (i == itemRand)
					name = ability.second.Name;
				i++;
			}
		}break;
		case 3:
		{
			std::uniform_int_distribution<> itemRandom(0, ItemManager::GetConsumables().size());
			int randomNum = itemRandom(Application::s_Gen);
			for (const auto& ability : ItemManager::GetConsumables())
			{
				if (i == itemRand)
					name = ability.second.Name;
				i++;
			}
		}break;
		default:
		{
			WI_CORE_ERROR("Item type doesn't match any type!");
			return;
		}break;
		}
		EntityManager& em = SceneManager::getActiveScene()->GetEntityManager();
		EntityId id = em.LoadPrefab("assets/Prefabs/RewardChest.wiprefab");
		EntityId childItem = em.GetChildByName(id, "Item");

		Item* item = em.GetComponent<Item>(childItem);
		Transform3D* t3d = em.GetComponent<Transform3D>(id);

		t3d->localPosition = position;
		item->item_type = type;

		name.copy(item->Name, 128);		
		item->Name[name.size()] = '\0';
	}
	void GameStateManager::SpawnShopRandomItem(glm::vec3 position, uint8_t type)
	{
		uint32_t itemRand;
		std::string name;
		int i = 0;
		switch (type)
		{
		case 0:
		{
			std::uniform_int_distribution<> itemRandom(0, ItemManager::GetAbilities().size());
			int randomNum = itemRandom(Application::s_Gen);
			for (const auto& ability : ItemManager::GetAbilities())
			{
				if (i == randomNum)
					name = ability.second.Name;
				i++;
			}
		}break;
		case 1:
		{
			std::uniform_int_distribution<> itemRandom(0, ItemManager::GetSkills().size());
			int randomNum = itemRandom(Application::s_Gen);
			for (const auto& ability : ItemManager::GetSkills())
			{
				if (i == randomNum)
					name = ability.second.Name;
				i++;
			}
		}break;
		case 2:
		{
			std::uniform_int_distribution<> itemRandom(0, ItemManager::GetBuffs().size());
			int randomNum = itemRandom(Application::s_Gen);
			for (const auto& ability : ItemManager::GetBuffs())
			{
				if (i == itemRand)
					name = ability.second.Name;
				i++;
			}
		}break;
		case 3:
		{
			std::uniform_int_distribution<> itemRandom(0, ItemManager::GetConsumables().size());
			int randomNum = itemRandom(Application::s_Gen);
			for (const auto& ability : ItemManager::GetConsumables())
			{
				if (i == itemRand)
					name = ability.second.Name;
				i++;
			}
		}break;
		default:
		{
			WI_CORE_ERROR("Item type doesn't match any type!");
			return;
		}break;
		}
		EntityManager& em = SceneManager::getActiveScene()->GetEntityManager();
		EntityId id = em.LoadPrefab("assets/Prefabs/Item.wiprefab");

		Item* item = em.GetComponent<Item>(id);
		Transform3D* t3d = em.GetComponent<Transform3D>(id);

		t3d->localPosition = position;
		item->item_type = type;

		name.copy(item->Name, 128);
		item->Name[name.size()] = '\0';
	}
	void GameStateManager::SpawnItem(glm::vec3 position, uint8_t type, const char* name)
	{
		std::string_view newName;
		switch (type)
		{
		case 0:
		{
			newName = ItemManager::GetAbility(name)->Name.c_str();
		}break;
		case 1:
		{
			newName = ItemManager::GetPassive(name)->Name.c_str();
		}break;
		case 2:
		{
			newName = ItemManager::GetBuff(name)->Name.c_str();
		}break;
		case 3:
		{
			newName = ItemManager::GetConsumable(name)->Name.c_str();
		}break;
		default:
		{
			WI_CORE_ERROR("Item type doesn't match any type!");
			return;
		}break;
		}

		EntityManager& em = GameStateManager::GetCurrentScene()->GetEntityManager();
		EntityId id = em.LoadPrefab("assets/Prefabs/Item.wiprefab");
		
		Item* item = em.GetComponent<Item>(id);
		Transform3D* t3d = em.GetComponent<Transform3D>(id);


		WI_CORE_INFO("Spawning item at {}x{}y{}z", position.x, position.y, position.z);
		
		t3d->localPosition = position;

		item->item_type = type;
		for (uint32_t i = 0; i < 128; i++)
		{
			if (i >= newName.size())
			{
				break;
			}
			item->Name[i] = newName[i];
		}
	}
	Transform3D* GameStateManager::GetPlayerTransform()
	{
		return s_CurrentScene->GetEntityManager().GetComponent<Transform3D>(s_PlayerId);
	}
}


