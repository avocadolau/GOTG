#include "AnimatorPanel.h"

#include "imgui.h"

#include <Wiwa/core/Application.h>

AnimatorPanel::AnimatorPanel(EditorLayer* instance)
	: Panel("Animator", ICON_FK_MALE, instance)
{
}

AnimatorPanel::~AnimatorPanel()
{
}

void AnimatorPanel::Draw()
{
	Wiwa::SysInfo& info = Wiwa::Application::Get().GetSystemInfo();

	ImGui::Begin(iconName.c_str(), &active);
	
	ImGui::End();
}
