#include "GameLayer.h"
#include <Wiwa/scene/SceneManager.h>
#include <Wiwa/render/RenderManager.h>
#include <Wiwa/core/Application.h>
#include <Wiwa/core/Renderer2D.h>

#include <Wiwa/utilities/functions/Action.h>
#include <Wiwa/core/ProjectManager.h>
#include <Wiwa/core/Input.h>

#include <Wiwa/ecs/systems/MeshRenderer.h>
#include <Wiwa/ecs/EntityManager.h>
#include <Wiwa/ecs/components/game/Character.h>

#define WI_DEBUG_BUILD

#ifdef WI_DEBUG_BUILD
	#include "windows.h"
	#include "psapi.h"
	#include <Wiwa/utilities/AllocationMetrics.h>
#endif
#define WI_DIST
GameLayer::GameLayer()
{

}

GameLayer::~GameLayer()
{

}

void GameLayer::OnAttach()
{
	Action<Wiwa::Event&> action = { &Wiwa::Application::OnEvent, &Wiwa::Application::Get() };
	Wiwa::OnLoadEvent load_ev;
	action.execute(load_ev);

	Wiwa::SceneManager::LoadSceneByIndex(0, Wiwa::SceneManager::LOAD_SEPARATE);

	Wiwa::RenderManager::SetRenderOnMainWindow(true);
	Wiwa::Time::Update();
	Wiwa::Time::SetTargetFPS(60);
	Wiwa::Time::SetTimeScale(1);
	Wiwa::Application::Get().FinishedImport = true;

}

void GameLayer::OnDetach()
{

}

void GameLayer::OnUpdate()
{
	
}

void GameLayer::OnImGuiRender()
{
#ifdef WI_DEBUG_BUILD

	static bool active = false;

	if (Wiwa::Input::IsKeyPressed(Wiwa::Key::F1))
		active = true;
	if (Wiwa::Input::IsKeyPressed(Wiwa::Key::F2))
		active = false;

	ImGuiContext* ctx = Wiwa::Application::Get().GetImGuiContext();
	ImGui::SetCurrentContext(ctx);
	if (active)
	{
		ImGui::Begin("Stats counter", &active);

		ImGui::Text("Game time since startup %.2fs", Wiwa::Time::GetTime());
		ImGui::Text("Game delta time %.2fms", Wiwa::Time::GetDeltaTime());
		ImGui::Text("Game FPS %.2fms", 1000.f / Wiwa::Time::GetDeltaTime());
		ImGui::Text("Game frame count %i", Wiwa::Time::GetGameFrameCount());
		ImGui::Text("Game time scale %i", Wiwa::Time::GetTimeScale());
		ImGui::Separator();
		ImGui::Text("Time since startup %.2fs", Wiwa::Time::GetRealTimeSinceStartup());
		ImGui::Text("Delta time %.2fms", Wiwa::Time::GetRealDeltaTime());
		ImGui::Text("FPS %.2fms", 1000.f / Wiwa::Time::GetRealDeltaTime());
		ImGui::Text("Frame count %i", Wiwa::Time::GetFrameCount());
		

		ImGui::Text("Allocation count %i", Wiwa::AllocationMetrics::allocation_count);
		ImGui::Text("Bytes allocated %i", Wiwa::AllocationMetrics::bytes_allocated);

		PROCESS_MEMORY_COUNTERS_EX pmc;
		GetProcessMemoryInfo(GetCurrentProcess(), (PROCESS_MEMORY_COUNTERS*)&pmc, sizeof(pmc));
		SIZE_T virtualMemUsedByMe = pmc.WorkingSetSize >> 20;

		ImGui::Text("Memory used %i", virtualMemUsedByMe);

		ImGui::End();


		ImGui::Begin("Game log", &active);

		if (ImGui::CollapsingHeader("Chances"))
		{
			ImGui::Text("Enemy item drop chance %i", &Wiwa::GameStateManager::s_EnemyDropChances);
			ImGui::Separator();
			ImGui::Text("Active skill drop chance %i", Wiwa::GameStateManager::s_ActiveSkillChances);
			ImGui::Text("Passive skill drop chance %i", Wiwa::GameStateManager::s_PassiveSkillChances);
			ImGui::Text("Buff drop chance %i", Wiwa::GameStateManager::s_BuffChances);
			ImGui::Text("NPC room chance %i", Wiwa::GameStateManager::s_NPCRoomChances);
		}
		ImGui::Separator();

		const char* roomType = Wiwa::GameStateManager().GetRoomType();
		ImGui::Text("Room type: ");
		ImGui::SameLine();
		ImGui::TextColored(ImVec4(0, 255, 200, 1), roomType);

		const char* roomState = Wiwa::GameStateManager().GetRoomState();
		ImGui::Text("Room state: ");
		ImGui::SameLine();
		ImGui::TextColored(ImVec4(0, 255, 200, 1), roomState);

		ImGui::Separator();
		ImGui::Text("Total spawners: ");
		ImGui::SameLine();
		ImGui::TextColored(ImVec4(0, 0, 200, 1), "%d", Wiwa::GameStateManager::s_TotalSpawners);

		ImGui::Text("Finished spawners: ");
		ImGui::SameLine();
		ImGui::TextColored(ImVec4(255, 0, 0, 1), "%d", Wiwa::GameStateManager::s_SpawnersFinished);

		ImGui::Separator();

		ImGui::Separator();
		ImGui::Text("Rooms to boss %i", Wiwa::GameStateManager::s_RoomsToBoss);
		ImGui::Text("Rooms to shop %i", Wiwa::GameStateManager::s_RoomsToShop);
		ImGui::Text("Rooms to end and return tu hub %i", Wiwa::GameStateManager::s_CurrentRoomsCount);
		ImGui::Separator();
		ImGui::PushID(0);
		ImGui::Text("Battle rooms");
		for (const auto& room : Wiwa::GameStateManager::s_CombatRooms)
		{
			ImGui::Text("Room id %i", room);
		}
		ImGui::PopID();
		ImGui::Separator();
		ImGui::PushID(1);
		ImGui::Text("Reward rooms");
		for (const auto& room : Wiwa::GameStateManager::s_RewardRooms)
		{
			ImGui::Text("Room id %i", room);
		}
		ImGui::Separator();
		ImGui::PopID();
		ImGui::PushID(2);
		ImGui::Text("Shop rooms");
		for (const auto& room : Wiwa::GameStateManager::s_ShopRooms)
		{
			ImGui::Text("Room id %i", room);
		}
		ImGui::PopID();
		ImGui::Separator();

		ImGui::Text("Current player id %i", Wiwa::GameStateManager::s_PlayerId);

		const char* startCharacter
			= Wiwa::GameStateManager::s_CurrentCharacter == 0 ? "StarLord" : Wiwa::GameStateManager::s_CurrentCharacter == 1 ? "Rocket" : "None";
		ImGui::Text("Current character %s", startCharacter);

		if (ImGui::CollapsingHeader("Current player stats"))
		{
			if (Wiwa::GameStateManager::s_CurrentScene && Wiwa::Time::IsPlaying())
			{
				if (Wiwa::GameStateManager::s_CharacterStats.c_id != WI_INVALID_INDEX)
				{
					Wiwa::Character* character =
						(Wiwa::Character*)Wiwa::GameStateManager::s_CurrentScene->GetEntityManager().GetComponentByIterator(Wiwa::GameStateManager::s_CharacterStats);
					if (character)
					{
						ImGui::Text("Max Health %i", character->MaxHealth);
						ImGui::Text("Health %i", character->Health);
						ImGui::Text("Max Shield %i", character->MaxShield);
						ImGui::Text("Shield %i", character->Shield);
						ImGui::Text("Damage %i", character->Damage);
						ImGui::Text("Rate of fire %f", character->RateOfFire);
						ImGui::Text("Speed %f", character->Speed);
						ImGui::Text("Dash distance %f", character->DashDistance);
						ImGui::Text("Dash speed %f", character->DashSpeed);
						ImGui::Text("Dash cooldown %f", character->DashCooldown);
						ImGui::Text("Walk threshold %f", character->WalkTreshold);
					}
				}
			}
			else
			{
				ImGui::Text("Not playing");
			}
		}

		ImGui::InputFloat("Gamepad deadzone", &Wiwa::GameStateManager::s_GamepadDeadzone);

		if (ImGui::Button("Save"))
		{
			Wiwa::GameStateManager::SerializeData();
		}
		ImGui::End();

		ImGui::Begin("Physics debug", &active);
		Wiwa::PhysicsManager& py = Wiwa::SceneManager::getActiveScene()->GetPhysicsManager();
		
		static int selected = -1;
		for (const auto& [key, value] : py.filterMapStringKey)
		{
			/*std::string tempStr = py.GetFilterTag(n);
			int bits = 1 << n;
			tempStr += "_" + std::to_string(bits);*/
			std::string tagNameWitBits = key;
			tagNameWitBits += "_" + std::to_string(value);
			ImGui::Text(tagNameWitBits.c_str());

		}

		ImGui::End();
	}
#endif
}

void GameLayer::OnEvent(Wiwa::Event& e)
{
	Wiwa::EventDispatcher dispatcher(e);
	dispatcher.Dispatch<Wiwa::OnSaveEvent>({ &GameLayer::OnGameSave, this });
	dispatcher.Dispatch<Wiwa::OnLoadEvent>({ &GameLayer::OnGameLoad, this });
}

bool GameLayer::OnGameSave(Wiwa::OnSaveEvent& e)
{


	return false;
}

bool GameLayer::OnGameLoad(Wiwa::OnLoadEvent& e)
{
	return false;
}