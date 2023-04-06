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
	
	ImGui::Text("Width: ");
	ImGui::PushID("Width: ");
	ImGui::InputInt("", &m_MapSizeInTiles.x);
	ImGui::PopID();

	ImGui::Text("Height: ");
	ImGui::PushID("Height: ");
	ImGui::InputInt("", &m_MapSizeInTiles.y);
	ImGui::PopID();

	ImGui::Text("Tile Width: ");
	ImGui::PushID("Tile Width: ");
	ImGui::InputFloat("", &m_SizeOfTiles.x);
	ImGui::PopID();

	ImGui::Text("Tile Height: ");
	ImGui::PushID("Tile Height: ");
	ImGui::InputFloat("", &m_SizeOfTiles.y);
	ImGui::PopID();

	DrawVec2Control(m_PosLabel, &m_StartingPosition, 0.0f, 0.0f);
	/*DrawInt2Control(m_NumLabel, &m_MapSizeInTiles, 1.0f, 0.0f);
	DrawVec2Control(m_SizeLabel, &m_SizeOfTiles, 1.0f, 0.0f);*/

	ImGui::Spacing();
	if (ImGui::Button("Create new Map"))
	{
		Wiwa::AIMapGeneration::CreateWalkabilityMap(m_MapSizeInTiles.x, m_MapSizeInTiles.y, m_SizeOfTiles.x, m_SizeOfTiles.y, m_StartingPosition);
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
	Wiwa::AIMapGeneration::m_DebugDraw = m_DrawGrid;

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
	m_StartingPosition = data.startingPosition;
}
