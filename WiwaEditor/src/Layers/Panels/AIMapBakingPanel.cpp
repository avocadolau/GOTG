#include "AIMapBakingPanel.h"

#include "imgui.h"

//#include <Wiwa/audio/Audio.h>
//#include <Wiwa/Platform/Windows/WindowsPlatformUtils.h>
//#include <Wiwa/utilities/filesystem/FileSystem.h>
//#include <Wiwa/core/Resources.h>

AIMapBakingPanel::AIMapBakingPanel(EditorLayer* instance)
	: Panel("AI Map Baking", ICON_FK_MAGIC, instance)
{
}

AIMapBakingPanel::~AIMapBakingPanel()
{
}

void AIMapBakingPanel::Draw()
{
	ImGui::Begin(iconName.c_str(), &active);

	
	ImGui::TextColored(ImVec4(255, 252, 127, 1), "Not loaded");
	

	ImGui::SameLine();
	

	if (ImGui::Button("Bake Map")) {
		
	}

	ImGui::End();
}
