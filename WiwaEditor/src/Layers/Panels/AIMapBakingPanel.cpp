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
	m_DrawGrid = true;
}

AIMapBakingPanel::~AIMapBakingPanel()
{
}

void AIMapBakingPanel::Draw()
{
	ImGui::Begin(iconName.c_str(), &active);
	
	ImGui::TextColored(ImVec4(255, 252, 127, 1), " This button bakes the Map with theese parameters:");
	
	const std::string posLabel = "Position in Tiles:";
	DrawInt2Control(posLabel, &m_MapSizeInTiles, 1.0f, 0.0f);

	const std::string sizeLabel = "Size of the Tiles:";
	DrawVec2Control(sizeLabel, &m_SizeOfTiles, 1.0f, 0.0f);

	ImGui::Spacing();
	if (ImGui::Button("Create new Map"))
	{
		Wiwa::AIMapGeneration::CreateWalkabilityMap(m_MapSizeInTiles.x, m_MapSizeInTiles.y, m_SizeOfTiles.x, m_SizeOfTiles.y, glm::vec3(0.0f));
		RefreshData();
	}

	if (ImGui::Button("Bake Map"))
	{
		Wiwa::AIMapGeneration::BakeMap();
	}

	if (ImGui::Button("Save current Map"))
	{
		Wiwa::AIMapGeneration::OnSave();
	}

	if (ImGui::Button("Load last saved Map"))
	{
		Wiwa::AIMapGeneration::OnLoad();
		RefreshData();
	}

	if (ImGui::Button("Clear Map"))
	{
		Wiwa::AIMapGeneration::ClearMap();
		RefreshData();
	}

	ImGui::Checkbox("Draw Map Grid", &m_DrawGrid);

	if (m_DrawGrid)
	{
		Wiwa::AIMapGeneration::DebugDrawMap();
	}
	
	ImGui::End();
}

void AIMapBakingPanel::RefreshData()
{
	Wiwa::AIMapGeneration::MapData& data = Wiwa::AIMapGeneration::GetMapData();
	m_MapSizeInTiles = { data.width, data.height };
	m_SizeOfTiles = { data.tileWidth, data.tileHeight };
}
