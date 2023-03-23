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
	
	
	glm::ivec2 mapSizeInTiles = { 0,0 };
	mapSizeInTiles.x = Wiwa::AIMapGeneration::GetMapData().width;
	mapSizeInTiles.y = Wiwa::AIMapGeneration::GetMapData().height;
	
	const std::string posLabel = "Position in Tiles:";
	DrawInt2Control(posLabel, &mapSizeInTiles, 1.0f, 300.0f);

	Wiwa::AIMapGeneration::GetMapData().width = mapSizeInTiles.x;
	Wiwa::AIMapGeneration::GetMapData().height = mapSizeInTiles.y;

	glm::vec2 sizeOfTiles = { 0.0f, 0.0f };
	sizeOfTiles.x = Wiwa::AIMapGeneration::GetMapData().tileWidth;
	sizeOfTiles.y = Wiwa::AIMapGeneration::GetMapData().tileHeight;

	const std::string sizeLabel = "Size of the Tiles:";
	DrawVec2Control(posLabel, &sizeOfTiles, 1.0f, 300.0f);

	Wiwa::AIMapGeneration::GetMapData().tileWidth  = sizeOfTiles.x;
	Wiwa::AIMapGeneration::GetMapData().tileHeight = sizeOfTiles.y;

	if (ImGui::Button("Bake Map")) {
		
		//
		Wiwa::EntityManager& entityManager = Wiwa::SceneManager::getActiveScene()->GetEntityManager();
		
		//std::unique_ptr<std::vector<SystemHash>> entityList  = std::make_unique<std::vector<SystemHash>>(entityManager);
		std::vector<EntityId>* entityList = entityManager.GetEntitiesAlive();
		
		EntityId eid = EntityId();

		//for(int i = 0; i < entityList->size(); i++)
		//{
		//	eid = entityList->at(i);
		//	if (entityManager.GetEntityName(eid) == "Floor")
		//	{
		//		// entityManager.GetComponent<Wiwa::tra>();
		//		// Get the transform component and call the generation map funtion
		//	}
		//	else {
		//		continue;
		//	}
		//}

		Wiwa::AIMapGeneration::CreateWalkabilityMap(50, 50, 1.0f, 1.0f, 0.0f);
	}

	ImGui::Checkbox("Draw Map Grid", &m_drawGrid);

	if (m_drawGrid)
	{
	}
	
	ImGui::End();
}
