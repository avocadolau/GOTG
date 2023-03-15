#include "AnimatorPanel.h"

#include "imgui.h"

#include <Wiwa/core/Application.h>
#include <Wiwa/scene/SceneManager.h>
#include <Wiwa/ecs/components/Mesh.h>
#include <Wiwa/utilities/render/Animation.h>
#include <ImGuizmo.h>
#include <GraphEditor.h>


#include <Wiwa/core/ProjectManager.h>

AnimatorPanel::AnimatorPanel(EditorLayer* instance)
	: Panel("Animator", ICON_FK_MALE, instance)
{
	delegate.AddTemplate();
}

AnimatorPanel::~AnimatorPanel()
{
}

void AnimatorPanel::Draw()
{
	Wiwa::EntityManager& em = Wiwa::SceneManager::getActiveScene()->GetEntityManager();

	ImGui::Begin(iconName.c_str(), &active);

	static float w = 200.0f;
	static float h = 300.0f;
	ImGui::PushStyleVar(ImGuiStyleVar_ItemSpacing, ImVec2(0, 0));
	ImGui::BeginChild("child1", ImVec2(w, h), true);



	ImGui::NewLine();

	if (ImGui::Button("Save file"))
	{
		SaveOnFile("try");
	}

	if (m_EntitySet && (m_CurrentID >= 0))
	{
		const char* entName = em.GetEntityName(m_CurrentID);
		std::string edit = entName;

		ImGui::Text("Name: %s", (char*)edit.c_str());
		ImGui::SameLine();
		ImGui::Text("(%i)", m_CurrentID);

		Wiwa::Model* model = Wiwa::Resources::GetResourceById<Wiwa::Model>(m_CurrentID);

		

		

		if (model != NULL) {
			//if (model->GetAnimations().size() != NULL)
			//{
			//	std::vector<Wiwa::Animation*> aux = model->GetAnimations();
			//	if (aux[0] != nullptr)
			//	{
			//		for (int i = 0; i < aux.size(); i++)
			//		{

			//			if (ImGui::Button(aux[i]->name.c_str(), ImVec2(150, 40)))
			//			{
			//				GraphEditorDelegate::Node node = GraphEditorDelegate::Node();
			//				node.name = aux[i]->name.c_str();
			//				node.templateIndex = 0;
			//				node.mRect = { 0, 0, 100, 75 };
			//				node.mSelected = false;
			//			
			//				delegate.mNodes.push_back(node);
			//														NEED TO ADD TEMPLATE WITH NODE AND ADD ID TEMPLATE TO NODE, I FEEL LIKE THIS IS SUPER INEFICIENT
			//			}

			//		}
			//	}
			//}
		}
	}
	else
	{
		ImGui::Text("Select an entity!");
	}

	
	//if (ImGui::Button("add"))
	//{
	//	delegate.test++;
	//	delegate.mTemplates[0].mInputCount++;
	//	WI_INFO("{}", delegate.test);
	//}
	
	ImGui::EndChild();
	ImGui::SameLine();
	
	if (ImGui::InvisibleButton("splitter", ImVec2(8.0f, h))) 
	{
		w += ImGui::GetIO().MouseDelta.x;
	}
	ImGui::SameLine();
	ImGui::BeginChild("child2", ImVec2(0, h), true);
	
	GraphEditor::Show(delegate, options, viewState, true);

	ImGui::EndChild();
	
	ImGui::PopStyleVar();
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

void AnimatorPanel::SaveOnFile(const char* modelName)
{
	//const char finalName = "library/" + modelName;
	//WI_INFO("{}", finalName);

	// save library and assets

	Wiwa::JSONDocument animFile;

	for (int i = 0; i < delegate.mNodes.size(); i++)
	{
		
		//SaveNode(animFile, i);
	}


	animFile.save_file("library/wry.json");
	//animFile.save_file("assets/wry.json");
	
}

void AnimatorPanel::SaveNode(Wiwa::JSONDocument file, int index)
{
	file.AddMemberObject("name").AddMember("id", index).AddMember("name", delegate.mNodes[index].name).AddMember("template", delegate.mNodes[index].templateIndex)
		.AddMemberObject("Rect").AddMember("min_x", delegate.mNodes[index].mRect.Min.x).AddMember("min_y", delegate.mNodes[index].mRect.Min.y)
		.AddMember("max_x", delegate.mNodes[index].mRect.Max.x).AddMember("max_y", delegate.mNodes[index].mRect.Max.y);
	
}