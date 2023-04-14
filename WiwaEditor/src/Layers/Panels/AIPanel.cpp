#include "AIPanel.h"

#include "imgui.h"

#include <Wiwa/ecs/EntityManager.h>
#include <Wiwa/scene/SceneManager.h>
#include "../../Utils/EditorUtils.h"
#include "Wiwa/ecs/components/Mesh.h"

//#include <Wiwa/audio/Audio.h>
//#include <Wiwa/Platform/Windows/WindowsPlatformUtils.h>
//#include <Wiwa/utilities/filesystem/FileSystem.h>
//#include <Wiwa/core/Resources.h>

//#include "glew.h"

AIPanel::AIPanel(EditorLayer* instance)
	: Panel("AI Panel", ICON_FK_MAGIC, instance)
{
	m_Mesh = nullptr;
	m_Id = -1;
}

AIPanel::~AIPanel()
{
}

void AIPanel::Draw()
{
	//Wiwa::Model* mod = Wiwa::Resources::GetResourceById<Wiwa::Model>(mesh->meshId);
	//mod = mod->getModelAt(mesh->modelIndex);
	ImGui::Begin(iconName.c_str(), &active);
	HandleDragAndDrop();

	if (ImGui::Button("Create obj"))
		HandleCreationObj();

	ImGui::End();
}

void AIPanel::HandleDragAndDrop()
{
	Wiwa::EntityManager& em = Wiwa::SceneManager::getActiveScene()->GetEntityManager();

	ImGui::Text("Drag and drop from hierarchy");

	if (m_Id != -1)
		m_Mesh = em.GetComponent<Wiwa::Mesh>(m_Id);

	Wiwa::Model* mod = nullptr;
	if (m_Mesh == nullptr){
		AssetContainer("Drag an entity from the hierarchy");
	}
	else
	{
		mod = Wiwa::Resources::GetResourceById<Wiwa::Model>(m_Mesh->meshId);
		if (mod != NULL){
			//mod = mod->getModelAt(m_Mesh->modelIndex);
			AssetContainer(std::filesystem::path(mod->getModelPath()).stem().string().c_str());
		}
	}

	if (ImGui::BeginDragDropTarget()){
		if (const ImGuiPayload* payload = ImGui::AcceptDragDropPayload("HIERARCHY_ENTITY"))
		{
			const EntityId* id = (EntityId*)payload->Data;
			m_Id = *id;
		}
		ImGui::EndDragDropTarget();
	}
}

void AIPanel::HandleCreationObj()
{
	if (m_Mesh == nullptr)
		return;

	Wiwa::Model* mod = Wiwa::Resources::GetResourceById<Wiwa::Model>(m_Mesh->meshId);
	mod->SaveModelAsOBJ(mod, mod->getModelPath());
}
