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

	Wiwa::SceneManager::LoadScene((uint32_t)0, Wiwa::SceneManager::LOAD_SEPARATE);

	Wiwa::RenderManager::SetRenderOnMainWindow(true);
}

void GameLayer::OnDetach()
{

}

void GameLayer::OnUpdate()
{

}

void GameLayer::OnImGuiRender()
{

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