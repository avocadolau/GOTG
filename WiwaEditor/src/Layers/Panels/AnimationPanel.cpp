#include "AnimationPanel.h"

#include "imgui.h"

#include <Wiwa/core/Application.h>
#include <Wiwa/scene/SceneManager.h>
#include <Wiwa/ecs/components/Mesh.h>

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

	

	Wiwa::EntityManager& em = Wiwa::SceneManager::getActiveScene()->GetEntityManager();

	ImGui::Begin(iconName.c_str(), &active);
	if (m_EntitySet && (m_CurrentID >= 0))
	{
		const char* entName = em.GetEntityName(m_CurrentID);
		std::string edit = entName;

		ImGui::Text("Entity:%s", (char*)edit.c_str());
		
		
		Wiwa::Mesh* meshId = em.GetComponent<Wiwa::Mesh>(m_CurrentID);
		if (meshId != nullptr)
		{
			Wiwa::Model* model = Wiwa::Resources::GetResourceById<Wiwa::Model>(meshId->meshId);
			model = model->getModelAt(meshId->modelIndex);
			if (ImGui::CollapsingHeader("Animations "))
			{
				//for(int i = 0; i < model->GetAnimations().
				ImGui::Button("Animation");
			}

		}
		
		
	}
	else
	{
		ImGui::Text("Entity:Select an entity!");
		if (ImGui::CollapsingHeader("Animations "))
		{
			ImGui::Text("No animations (no entity selected)");
			
		}
	}


	ImGui::End();
}

void AnimationPanel::OnEvent(Wiwa::Event& e)
{
	Wiwa::EventDispatcher dispatcher(e);
	dispatcher.Dispatch<EntityChangeEvent>({ &AnimationPanel::OnEntityChange, this});
	dispatcher.Dispatch<Wiwa::SceneChangeEvent>({ &AnimationPanel::OnSceneChange, this });
}
bool AnimationPanel::OnEntityChange(EntityChangeEvent& e)
{
	m_CurrentID = e.GetResourceId();
	m_EntitySet = true;


	return false;
}
bool AnimationPanel::OnSceneChange(Wiwa::SceneChangeEvent& e)
{
	m_CurrentID = -1;
	m_EntitySet = false;



	return false;
}