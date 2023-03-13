#include "AnimationPanel.h"

#include "imgui.h"

#include <Wiwa/core/Application.h>
#include <Wiwa/scene/SceneManager.h>

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
		//std::vector<EntityId>* entities = em.GetParentEntitiesAlive();
		//EntityId entID = em.GetParentEntitiesAlive()->at(m_CurrentID);
		ImGui::Text("Entity:%s", (char*)edit.c_str());
		
		
		Wiwa::Mesh* meshId = em.GetComponent<Wiwa::Mesh>(m_CurrentID);
		if (meshId != nullptr)
		{
			Wiwa::Model* model = Wiwa::Resources::GetResourceById<Wiwa::Model>(meshId->meshId);
			model = model->getModelAt(meshId->modelIndex);
			if (ImGui::CollapsingHeader("Animations "))
			{
				//model->GetParent()->GetAnimations().size();
				ImGui::Dummy(ImVec2(20.0f, 0.0f));
				ImGui::SameLine();
				ImGui::BeginChild("Names");
				
				ImGui::Text(std::to_string(model->GetParent()->animationTime).c_str());
				//for (int i = 0; i< model->GetParent()->GetAnimations().size(); i++)
				//{
				//	if (model->GetParent()->GetAnimations()[i] != nullptr)
				//	{
				//		if (ImGui::CollapsingHeader(model->GetParent()->GetAnimations()[i]->name.c_str()))
				//		{
				//			
				//			DisplayBones(model, meshId, i);
				//		}

				//		
				//	}
				//	
				//}
				ImGui::EndChild();
				
				
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
	////ImSequencer::Sequencer()
	//static int selectedEntry = -1;
	//static int firstFrame = 0;
	//static bool expanded = true;
	//static int currentFrame = 100;

	//ImGui::PushItemWidth(130);
	//ImGui::InputInt("Frame Min", &mySequence.mFrameMin);
	//ImGui::SameLine();
	//ImGui::InputInt("Frame ", &currentFrame);
	//ImGui::SameLine();
	//ImGui::InputInt("Frame Max", &mySequence.mFrameMax);
	//ImGui::PopItemWidth();
	//ImSequencer::Sequencer(&mySequence, &currentFrame, &expanded, &selectedEntry, &firstFrame, ImSequencer::SEQUENCER_EDIT_STARTEND | ImSequencer::SEQUENCER_ADD | ImSequencer::SEQUENCER_DEL | ImSequencer::SEQUENCER_COPYPASTE | ImSequencer::SEQUENCER_CHANGE_FRAME);
	//// add a UI to edit that particular item
	//if (selectedEntry != -1)
	//{
	//	const MySequence::MySequenceItem& item = mySequence.myItems[selectedEntry];
	//	ImGui::Text("I am a %s, please edit me", SequencerItemTypeNames[item.mType]);
	//	// switch (type) ....
	//}
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

void AnimationPanel::DisplayBones(Wiwa::Model* model, Wiwa::Mesh* meshId, int animationNum)
{
	
	
	//ImGui::Button(model->GetParent()->GetAnimations()[animationNum]->name.c_str());
	//model->GetBoneInfo().size();
	ImGui::Text("test");
	
}