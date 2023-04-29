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

#define WI_DEBUG_BUILD

#ifdef WI_DEBUG_BUILD
	#include "windows.h"
	#include "psapi.h"
	#include <Wiwa/utilities/AllocationMetrics.h>
#endif

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
		active = !active;

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

		ImGui::Text("Allocation count %i", Wiwa::AllocationMetrics::allocation_count);
		ImGui::Text("Bytes allocated %i", Wiwa::AllocationMetrics::bytes_allocated);

		PROCESS_MEMORY_COUNTERS_EX pmc;
		GetProcessMemoryInfo(GetCurrentProcess(), (PROCESS_MEMORY_COUNTERS*)&pmc, sizeof(pmc));
		SIZE_T virtualMemUsedByMe = pmc.WorkingSetSize >> 20;

		ImGui::Text("Memory used %i", virtualMemUsedByMe);

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