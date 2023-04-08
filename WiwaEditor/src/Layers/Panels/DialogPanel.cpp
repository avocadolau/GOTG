#include <wipch.h>
#include "DialogPanel.h"
#include "imgui.h"
#include "Time.h"

#include <Wiwa/core/Application.h>

using namespace Wiwa;

DialogPanel::DialogPanel(EditorLayer* instance)
	: Panel("Dialog", ICON_FK_LIST, instance)
{
	
}

DialogPanel::~DialogPanel()
{
}

void DialogPanel::Draw()
{
	Wiwa::DialogManager& dm = Wiwa::SceneManager::getActiveScene()->GetDialogManager();

	

	ImGui::Begin(iconName.c_str(), &active);
	
	ImGui::NewLine();

	if (ImGui::CollapsingHeader("Create Conversations"))
	{
		if (ImGui::Button("New Conversation"))
		{

		}

		ImGui::Text("Nothing functional here... yet.");
	}
	if (ImGui::CollapsingHeader("Edit Conversations"))
	{
		if (dm.conversations.size() > 0)
		{
			for (int i = 0; i < dm.conversations.size(); i++)
			{
				ImGui::PushID(i);
				if (ImGui::Button(dm.conversations[i]->conversationName))
				{

				}
				ImGui::PopID();
			}

			ImGui::Text("Nothing functional here... yet.");
		}
		else
		{
			ImGui::Text("No conversations have been created yet.");
		}
	}
	
	ImGui::End();
}