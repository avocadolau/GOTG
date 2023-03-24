#include "AIMapBakingPanel.h"

#include "imgui.h"

#include <Wiwa/ecs/EntityManager.h>
#include <Wiwa/scene/SceneManager.h>
#include <Wiwa/AI/AIMapGeneration.h>
#include "../../Utils/EditorUtils.h"
//#include <Wiwa/audio/Audio.h>
//#include <Wiwa/Platform/Windows/WindowsPlatformUtils.h>
//#include <Wiwa/utilities/filesystem/FileSystem.h>
//#include <Wiwa/core/Resources.h>

//#include "glew.h"

AIMapBakingPanel::AIMapBakingPanel(EditorLayer* instance)
	: Panel(" AI Map Baking", ICON_FK_MAGIC, instance)
{
	m_drawGrid = true;
}

AIMapBakingPanel::~AIMapBakingPanel()
{
}

void AIMapBakingPanel::Draw()
{
	ImGui::Begin(iconName.c_str(), &active);
	
	ImGui::TextColored(ImVec4(255, 252, 127, 1), " This button bakes the Map with theese parameters:");
	
	const std::string posLabel = "Position in Tiles:";
	DrawInt2Control(posLabel, &mapSizeInTiles, 1.0f, 300.0f);

	const std::string sizeLabel = "Size of the Tiles:";
	DrawVec2Control(sizeLabel, &sizeOfTiles, 1.0f, 300.0f);

	ImGui::Spacing();
	if (ImGui::Button("Create new Map"))
	{
		Wiwa::AIMapGeneration::CreateWalkabilityMap(mapSizeInTiles.x, mapSizeInTiles.y, sizeOfTiles.x, sizeOfTiles.y, 0.0f);
	}

	if (ImGui::Button("Bake Map"))
	{
		Wiwa::AIMapGeneration::BakeMap();
	}

	ImGui::Checkbox("Draw Map Grid", &m_drawGrid);

	if (m_drawGrid)
	{
		Wiwa::AIMapGeneration::DebugDrawMap();
	}
	
	ImGui::End();
}
