#include "wipch.h"
#include "embed_ui_functions.h"
#include <Wiwa/scene/SceneManager.h>
#include <Wiwa/core/Application.h>
#include <Wiwa/Ui/UiManager.h>

void Activate(int GuiID,bool active)
{
    Wiwa::GuiManager& guiManager = Wiwa::SceneManager::getActiveScene()->GetGuiManager();

    guiManager.controls.at(GuiID)->SwapActive(Wiwa::Application::Get().GetRenderer2D(),active);
}

void SwapTexture(int GuiID,std::string path)
{
    Wiwa::GuiManager& guiManager = Wiwa::SceneManager::getActiveScene()->GetGuiManager();

    //guiManager.controls.at(GuiID).SwapTexture(path.c_str());
}
