#include "AnimationPanel.h"

#include "imgui.h"

#include <Wiwa/core/Application.h>

AnimationPanel::AnimationPanel(EditorLayer* instance)
	: Panel("Animation Editor", ICON_FK_SKATE, instance)
{
}

AnimationPanel::~AnimationPanel()
{
}

void AnimationPanel::Draw()
{
	Wiwa::SysInfo& info = Wiwa::Application::Get().GetSystemInfo();

	ImGui::Begin(iconName.c_str(), &active);




	ImGui::End();
}
