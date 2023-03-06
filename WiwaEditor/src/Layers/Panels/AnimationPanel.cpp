#include "AnimationPanel.h"

#include "imgui.h"

#include <Wiwa/core/Application.h>
#include <Wiwa/scene/SceneManager.h>
#include <Wiwa/ecs/components/Mesh.h>
#include <ImSequencer.h>

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
	

	static float w = 400.0f;
	static float h = 300.0f;

	ImGui::PushStyleVar(ImGuiStyleVar_ItemSpacing, ImVec2(0, 0));
	ImGui::BeginChild("select", ImVec2(w, h), true);
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
				ImGui::Text(model->GetParent()->GetAnimations()[0]->name.c_str());
				ImGui::Text(std::to_string(model->GetParent()->animationTime).c_str());
				ImGui::Text(std::to_string(model->GetParent()->animationTime).c_str());

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
	ImGui::EndChild();
	ImGui::SameLine();
	ImGui::BeginChild("Editor", ImVec2(w, h), true);
	//ImSequencer::Sequencer()

	ImGui::EndChild();
	ImGui::PopStyleVar();
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