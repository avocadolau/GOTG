#include <wipch.h>
#include "DialogPanel.h"
#include "imgui.h"
#include "Time.h"

#include <Wiwa/core/Application.h>

using namespace Wiwa;

DialogPanel::DialogPanel(EditorLayer* instance)
	: Panel("Dialog", ICON_FK_LIST, instance)
{
	
}

DialogPanel::~DialogPanel()
{
}

void DialogPanel::Draw()
{
	Wiwa::DialogManager& gm = Wiwa::SceneManager::getActiveScene()->GetDialogManager();

	ImGui::Begin(iconName.c_str(), &active);
	
	ImGui::NewLine();
	ImGui::Text("Nothing to se here... yet.");
	
	ImGui::End();
}