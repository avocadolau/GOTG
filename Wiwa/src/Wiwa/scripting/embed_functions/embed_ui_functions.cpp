#include "wipch.h"
#include "embed_ui_functions.h"
#include <Wiwa/scene/SceneManager.h>
#include <Wiwa/core/Application.h>
#include <Wiwa/Ui/UiManager.h>

#include <mono/metadata/reflection.h>

void ActivateGuiCanvas(int id)
{
	Wiwa::GuiManager& gm = Wiwa::SceneManager::getActiveScene()->GetGuiManager();

	gm.canvas.at(id)->SwapActive();
}

void SwapToNewTexture(MonoString* path,int id_canvas,int id_gui)
{
	Wiwa::GuiManager& gm = Wiwa::SceneManager::getActiveScene()->GetGuiManager();
	Wiwa::Renderer2D& r2d = Wiwa::Application::Get().GetRenderer2D();

	char* newPath = mono_string_to_utf8(path);
	gm.canvas.at(id_canvas)->controls.at(id_gui)->SwapToNewTexture(newPath,r2d);
}

void SwapText(MonoString* word, int id_canvas, int id_gui)
{
	Wiwa::GuiManager& gm = Wiwa::SceneManager::getActiveScene()->GetGuiManager();
	Wiwa::Renderer2D& r2d = Wiwa::Application::Get().GetRenderer2D();

	char* newWord = mono_string_to_utf8(word);
	Wiwa::Text* newText = new Wiwa::Text();
	newText = gm.InitFont("assets/Fonts/Jade_Smile.ttf", newWord);
	gm.canvas.at(id_canvas)->controls.at(id_gui)->SwapText(newWord, r2d,newText);
}

void SetValueForUIbar(float value,float valueMaxHealth, int id_canvas, int id_gui)
{
	Wiwa::GuiManager& gm = Wiwa::SceneManager::getActiveScene()->GetGuiManager();
	gm.canvas.at(id_canvas)->controls.at(id_gui)->SetValueForUIbar(value, valueMaxHealth);
}

void SetNextFrame(int value, int id_canvas, int id_gui)
{
	Wiwa::GuiManager& gm = Wiwa::SceneManager::getActiveScene()->GetGuiManager();
	Wiwa::Renderer2D& r2d = Wiwa::Application::Get().GetRenderer2D();
	gm.canvas.at(id_canvas)->controls.at(id_gui)->SetNextFrame(value,&r2d);
}