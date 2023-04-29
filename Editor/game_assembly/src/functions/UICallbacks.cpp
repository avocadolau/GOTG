#include "UICallbacks.h"
#include <Wiwa/scene/SceneManager.h>
#include <Wiwa/ecs/EntityManager.h>
#include <Wiwa/Ui/UiManager.h>
#include <Wiwa/core/Application.h>
#include <Wiwa/core/Window.h>
#include <Wiwa/audio/Audio.h>
#include "../systems/ships/ship_main_menu.h"
#include <iostream>

GoToMilanoHub_ GoToMilanoHub()
{
	//Wiwa::SceneManager::ChangeSceneByIndex(2);
	Wiwa::EntityManager& em = Wiwa::SceneManager::getActiveScene()->GetEntityManager();
	EntityId parentID = em.GetEntityByName("Parent");
	EntityId shipID = em.GetChildByName(parentID, "sm_milano_01a");
	Wiwa::ShipMainMenu* ship = em.GetSystem<Wiwa::ShipMainMenu>(shipID);
	ship->SetPanToCamera(true);
	return GoToMilanoHub_::hola;
}

GoMainMenu_ GoMainMenu()
{
	Wiwa::SceneManager::ChangeSceneByIndex(1);
	return GoMainMenu_::hola;
}

Quit_ Quit()
{
	Wiwa::Application::Get().Quit();
	return Quit_();
}

Resume_PauseGame_ Resume_PauseGame()
{
	Wiwa::GuiManager& gm = Wiwa::SceneManager::getActiveScene()->GetGuiManager();
	gm.canvas.at(1)->SwapActive();
	Wiwa::SceneManager::PauseCurrentScene();
	gm.canvas.at(0)->SwapActive();
	return Resume_PauseGame_();
}

ActivateOptionsCanvas_ ActivateOptionsCanvas()
{
	Wiwa::GuiManager& gm = Wiwa::SceneManager::getActiveScene()->GetGuiManager();
	gm.canvas.at(1)->SwapActive();
	gm.canvas.at(2)->SwapActive();
	return ActivateOptionsCanvas_();
}

DeActivateOptionsCanvas_ DeActivateOptionsCanvas()
{
	Wiwa::GuiManager& gm = Wiwa::SceneManager::getActiveScene()->GetGuiManager();
	gm.canvas.at(2)->SwapActive();
	gm.canvas.at(1)->SwapActive();
	return DeActivateOptionsCanvas_();
}

SetFullScreen_ SetFullScreen(bool ret)
{
	Wiwa::Application::Get().GetWindow().SetFullScreen(ret);
	return SetFullScreen_();
}

SetVsync_ SetVsync(bool ret)
{
	Wiwa::Application::Get().GetWindow().SetVSync(ret);
	return SetVsync_();
}

SetVolumeMusic_ SetVolumeMusic(float level)
{
	Audio::ChangeMasterVolume((int)level);
	return SetVolumeMusic_();
}

SetVolumeFX_ SetVolumeFX(float level)
{
	Audio::ChangeMasterVolume((int)level);
	return SetVolumeFX_();
}

PlaceHolderButton_ PlaceHolderButton()
{
	return PlaceHolderButton_();
}

ActivateOptionsMenu_ ActivateOptionsMenu()
{
	Wiwa::GuiManager& gm = Wiwa::SceneManager::getActiveScene()->GetGuiManager();
	gm.canvas.at(0)->SwapActive();
	gm.canvas.at(1)->SwapActive();
	return ActivateOptionsMenu_();
}

DeActivateOptionsMenu_ DeActivateOptionsMenu()
{
	Wiwa::GuiManager& gm = Wiwa::SceneManager::getActiveScene()->GetGuiManager();
	gm.canvas.at(1)->SwapActive();
	gm.canvas.at(0)->SwapActive();
	return DeActivateOptionsMenu_();
}

