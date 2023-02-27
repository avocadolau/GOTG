#include "GameLayer.h"
#include <Wiwa/scene/SceneManager.h>
#include <Wiwa/render/RenderManager.h>
#include <Wiwa/core/Application.h>
#include <Wiwa/core/Renderer2D.h>

GameLayer::GameLayer()
{

}

GameLayer::~GameLayer()
{

}

void GameLayer::OnAttach()
{
	// TODO: LOAD SCENE FROM GAME FILE
	size_t scene_id = Wiwa::SceneManager::CreateScene();
	Wiwa::Scene* scene = Wiwa::SceneManager::getScene(scene_id);

	Wiwa::SceneManager::SetScene(scene_id);

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

}
