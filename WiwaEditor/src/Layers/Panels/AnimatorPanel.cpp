#include "AnimatorPanel.h"

#include "imgui.h"

#include <Wiwa/core/Application.h>
#include <Wiwa/scene/SceneManager.h>
#include <Wiwa/ecs/components/Mesh.h>



AnimatorPanel::AnimatorPanel(EditorLayer* instance)
	: Panel("Animator", ICON_FK_MALE, instance)
{
}

AnimatorPanel::~AnimatorPanel()
{
}

void AnimatorPanel::Draw()
{
	Wiwa::EntityManager& em = Wiwa::SceneManager::getActiveScene()->GetEntityManager();

	ImGui::Begin(iconName.c_str(), &active);
	if (m_EntitySet && (m_CurrentID >= 0))
	{
		const char* entName = em.GetEntityName(m_CurrentID);
		std::string edit = entName;

		ImGui::Text("Name: %s", (char*)edit.c_str());
		ImGui::SameLine();
		ImGui::Text("(%i)", m_CurrentID);
		
		Wiwa::Mesh* meshId = em.GetComponent<Wiwa::Mesh>(m_CurrentID);
		Wiwa::Model* model = Wiwa::Resources::GetResourceById<Wiwa::Model>(meshId->meshId);
		model = model->getModelAt(meshId->modelIndex);
		
		ImGui::Text("Model path: ");
		ImGui::SameLine();
		ImGui::Text(model->getModelPath());

		if (model->animations[0] != nullptr)
		{
			for (int i = 0; i > model->animations.size(); i++)
			{
				ImGui::Button(model->animations[i]->name.c_str());
			}
		}
	}
	else
	{
		ImGui::Text("Select an entity!");
	}

	ImGui::End();
}

	
void AnimatorPanel::OnEvent(Wiwa::Event& e)
{
	Wiwa::EventDispatcher dispatcher(e);
	dispatcher.Dispatch<EntityChangeEvent>({ &AnimatorPanel::OnEntityChangeEvent, this });
	dispatcher.Dispatch<Wiwa::SceneChangeEvent>({ &AnimatorPanel::OnSceneChangeEvent, this });
}

bool AnimatorPanel::OnEntityChangeEvent(EntityChangeEvent& e)
{
	m_CurrentID = e.GetResourceId();
	m_EntitySet = true;
	return false;
}

bool AnimatorPanel::OnSceneChangeEvent(Wiwa::SceneChangeEvent& e)
{
	m_CurrentID = -1;
	m_EntitySet = false;
	return false;
}