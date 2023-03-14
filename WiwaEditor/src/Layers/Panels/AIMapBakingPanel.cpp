#include "AIMapBakingPanel.h"

#include "imgui.h"

#include <Wiwa/ecs/EntityManager.h>
#include <Wiwa/scene/SceneManager.h>
//#include <Wiwa/audio/Audio.h>
//#include <Wiwa/Platform/Windows/WindowsPlatformUtils.h>
//#include <Wiwa/utilities/filesystem/FileSystem.h>
//#include <Wiwa/core/Resources.h>

AIMapBakingPanel::AIMapBakingPanel(EditorLayer* instance)
	: Panel(" AI Map Baking", ICON_FK_MAGIC, instance)
{
}

AIMapBakingPanel::~AIMapBakingPanel()
{
}

void AIMapBakingPanel::Draw()
{
	ImGui::Begin(iconName.c_str(), &active);

	
	ImGui::TextColored(ImVec4(255, 252, 127, 1), " This button bakes the A* Map into the entity named");
	ImGui::SameLine();
	ImGui::TextColored(ImVec4(0, 255, 200, 1), "Floor");

	ImGui::TextColored(ImVec4(255, 0, 0, 1), " It does not take in account AABBs collisions yet");
	

	//ImGui::SameLine();
	

	if (ImGui::Button("Bake Map")) {
		
		//
		Wiwa::EntityManager& entityManager = Wiwa::SceneManager::getActiveScene()->GetEntityManager();
		
		std::unique_ptr<std::vector<SystemHash>> entityList  = std::make_unique<std::vector<SystemHash>>(entityManager);
		
		EntityId eid = EntityId();

		for(int i = 0; i < entityList->size(); i++)
		{
			eid = entityList->at(i);
			if (entityManager.GetEntityName(eid) == "Floor")
			{
				// entityManager.GetComponent<Wiwa::tra>();
				// Get the transform component and call the generation map funtion
			}
			else {
				continue;
			}
		}
	}

	
	ImGui::End();
}
