#include "wipch.h"
#include "embed_ui_functions.h"
#include <Wiwa/scene/SceneManager.h>
#include <Wiwa/core/Application.h>
#include <Wiwa/Ui/UiManager.h>

void Play()
{
    //Wiwa::SceneManager::ChangeScene((Wiwa::SceneManager::getActiveSceneId() + 1));
}

void ChangeScene(int scene_id)
{
    //Wiwa::SceneManager::ChangeScene(scene_id);
}

void Quit()
{
    Wiwa::Application::Get().Quit();
}
