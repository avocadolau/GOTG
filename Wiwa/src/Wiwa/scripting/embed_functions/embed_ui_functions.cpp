#include "wipch.h"
#include "embed_ui_functions.h"
#include <Wiwa/scene/SceneManager.h>
#include <Wiwa/core/Application.h>
#include <Wiwa/Ui/UiManager.h>


void ActivateCanvas(int id)
{
	Wiwa::SceneManager::getActiveScene()->GetGuiManager().canvas.at(id)->SwapActive();
}