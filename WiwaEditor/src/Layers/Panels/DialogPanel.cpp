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
			numNodesOnCurrentCreation = 1;
			creatingNewDialog = true;
			currentCreationStep = 0;
		}

		ImGui::Text("WARNING functionallity not fully implemented... yet. IT WILL CRASH IF USED");

		if (creatingNewDialog == true)
		{
			if(currentCreationStep == 0)
			{
				dm.conversationCreator_P = new Conversation();
				currentCreationStep = 1;
			}

			if (currentCreationStep == 1)
			{
				ImGui::PushID(numNodesOnCurrentCreation);

				std::string collapsingHeaderName = "Edit Node #" + numNodesOnCurrentCreation;

				if (ImGui::CollapsingHeader(collapsingHeaderName.c_str()))
				{
					static char textLine1Buffer[1024] = { 0 };
					ImGui::InputText("Line 1 text", textLine1Buffer, IM_ARRAYSIZE(textLine1Buffer));
					static char textLine2Buffer[1024] = { 0 };
					ImGui::InputText("Line 2 text", textLine2Buffer, IM_ARRAYSIZE(textLine2Buffer));
					static char textLine3Buffer[1024] = { 0 };
					ImGui::InputText("Line 3 text", textLine3Buffer, IM_ARRAYSIZE(textLine3Buffer));

					ImGui::Text("WARNING, Only Press this button if you fully finished creating the current dialog node:");
					ImGui::PushID(numNodesOnCurrentCreation);
					if (ImGui::Button("Save Current Node's Text Lines"))
					{
						dm.SetDialogText(textLine1Buffer, textLine2Buffer, textLine3Buffer, "assets/Fonts/Jade_Smile.ttf", *dm.conversationCreator_P);
						numNodesOnCurrentCreation++;
					}
					ImGui::PopID();

					ImGui::Text("WARNING, Only Press this button if you fully finished creating all the dialog nodes:");
					ImGui::PushID(numNodesOnCurrentCreation);
					if (ImGui::Button("Finished Creating All Dialog Nodes"))
					{
						currentCreationStep = 2;
					}
					ImGui::PopID();
				}
				ImGui::PopID();
			}
			//ImGui::SameLine();

			if (currentCreationStep == 2)
			{
				dm.SetContinueIndicatorImage("assets/HUD_Images/dialog_images/dialog_test_placeholder3.png", *dm.conversationCreator_P);
				currentCreationStep = 3;
			}

			if (currentCreationStep == 3)
			{
				if (const ImGuiPayload* payload = ImGui::AcceptDragDropPayload("Bubble image"))
				{
					const wchar_t* path = (const wchar_t*)payload->Data;
					std::wstring ws(path);
					std::string pathS(ws.begin(), ws.end());
					std::filesystem::path p = pathS.c_str();
					if (p.extension() == ".png")
					{
						dm.SetDialogBubbleImage(pathS.c_str(), *dm.conversationCreator_P);
					}
				}
				//dm.SetDialogBubbleImage("assets/HUD_Images/menus/speech menu/ui_speech_menu_starlord_bubble-01.png", *dm.conversationCreator_P);
				ImGui::Text("WARNING, Only Press this button if you finished importing the bubble image:");
				if (ImGui::Button("Finished Importing Bubble Image"))
				{
					currentCreationStep = 4;
				}
			}

			if (currentCreationStep == 4)
			{
				if (const ImGuiPayload* payload = ImGui::AcceptDragDropPayload("Character Image"))
				{
					const wchar_t* path = (const wchar_t*)payload->Data;
					std::wstring ws(path);
					std::string pathS(ws.begin(), ws.end());
					std::filesystem::path p = pathS.c_str();
					if (p.extension() == ".png")
					{
						dm.SetCharacterImage(pathS.c_str(), *dm.conversationCreator_P);
					}
				}
				//dm.SetCharacterImage("assets/HUD_Images/menus/speech menu/ui_speech_menu_starlord_withshadows-01.png", *dm.conversationCreator_P);
				ImGui::Text("WARNING, Only Press this button if you finished importing the character image:");
				if (ImGui::Button("Finished Importing Character Image"))
				{
					currentCreationStep = 5;
				}
			}

			if (currentCreationStep == 5)
			{
				static char conversationNameBuffer[1024] = { 0 };
				ImGui::InputText("Conversation name", conversationNameBuffer, IM_ARRAYSIZE(conversationNameBuffer));
				dm.conversationCreator_P->conversationName = conversationNameBuffer;

				ImGui::Text("WARNING, Only Press this button if you finished creating the conversation");
				if (ImGui::Button("Finish And Save Conversation"))
				{
					currentCreationStep = 6;
				}
			}

			if (currentCreationStep == 6)
			{
				dm.conversations.push_back(dm.conversationCreator_P);
				creatingNewDialog = false;

				delete dm.conversationCreator_P;
			}
		}
	}
	if (ImGui::CollapsingHeader("Edit Conversations"))
	{
		if (dm.conversations.size() > 0)
		{
			for (int i = 0; i < dm.conversations.size(); i++)
			{
				//ImGui::PushID(i);
				//if (ImGui::Button(dm.conversations[i]->conversationName))
				//{
				//
				//}
				//ImGui::PopID();
			}

			ImGui::Text("WARNING functionallity not fully implemented... yet. IT WILL CRASH IF USED");
		}
		else
		{
			ImGui::Text("No conversations have been created yet.");
		}
	}
	
	ImGui::End();
}