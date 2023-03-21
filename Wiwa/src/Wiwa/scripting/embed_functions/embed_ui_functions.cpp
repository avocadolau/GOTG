#include "wipch.h"
#include "embed_ui_functions.h"
#include <Wiwa/scene/SceneManager.h>
#include <Wiwa/core/Application.h>
#include <Wiwa/Ui/UiManager.h>


void ActivateGuiCanvas(int id)
{
	Wiwa::GuiManager& gm = Wiwa::SceneManager::getActiveScene()->GetGuiManager();

	gm.canvas.at(id)->SwapActive();
}

void SwapText(const char* text,int id_canvas,int id_gui)
{
	Wiwa::GuiManager& gm = Wiwa::SceneManager::getActiveScene()->GetGuiManager();

	//gm.canvas.at(id_canvas)->controls.at(id_gui).swa
}