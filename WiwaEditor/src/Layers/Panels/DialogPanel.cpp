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
				currentNode = 0;
				currentConversation = 0;

				currentCreationStep = 1;
			}

			if (currentCreationStep == 1)
			{
				ImGui::PushID(numNodesOnCurrentCreation);

				std::string collapsingHeaderName = "Edit Node #" + numNodesOnCurrentCreation;

				if (ImGui::CollapsingHeader("Node editor"))
				{
					ImGui::Text("Currently Editing Node # %i", numNodesOnCurrentCreation);

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
						dm.SetDialogText(textLine1Buffer, textLine2Buffer, textLine3Buffer, "assets/Fonts/Jade_Smile.ttf", currentConversation, currentNode);
						dm.conversations[currentConversation].nodes[currentNode].occupied = true;
						numNodesOnCurrentCreation++;
						currentNode++;
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
				//dm.SetContinueIndicatorImage("assets/HUD_Images/dialog_images/dialog_test_placeholder3.png", *dm.conversationCreator_P);

				Renderer2D& render = Wiwa::Application::Get().GetRenderer2D();

				ResourceId textID = Wiwa::Resources::Load<Wiwa::Image>("assets/HUD_Images/dialog_images/dialog_test_placeholder3.png");
				Image* continueImg = Wiwa::Resources::GetResourceById<Wiwa::Image>(textID);

				dm.continueImgID = render.CreateInstancedQuadTex(dm.m_Scene, continueImg->GetTextureId(), continueImg->GetSize(), { 1600,800 }, { 50,50 }, Wiwa::Renderer2D::Pivot::UPLEFT);
				render.DisableInstance(dm.m_Scene, dm.continueImgID);

				currentCreationStep = 3;
			}

			if (currentCreationStep == 3)
			{
				ImGui::Text("Bubble image:");
				ImGui::SameLine();
				if (const ImGuiPayload* payload = ImGui::AcceptDragDropPayload("CONTENT_BROWSER_ITEM"))
				{
					const wchar_t* path = (const wchar_t*)payload->Data;
					std::wstring ws(path);
					std::string pathS(ws.begin(), ws.end());
					std::filesystem::path p = pathS.c_str();
					if (p.extension() == ".png")
					{
						dm.SetDialogBubbleImage(pathS.c_str(), currentConversation);
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
				ImGui::Text("Character image:");
				ImGui::SameLine();
				if (const ImGuiPayload* payload = ImGui::AcceptDragDropPayload("CONTENT_BROWSER_ITEM"))
				{
					const wchar_t* path = (const wchar_t*)payload->Data;
					std::wstring ws(path);
					std::string pathS(ws.begin(), ws.end());
					std::filesystem::path p = pathS.c_str();
					if (p.extension() == ".png")
					{
						dm.SetCharacterImage(pathS.c_str(), currentConversation);
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
				dm.conversations[currentConversation].conversationName = conversationNameBuffer;

				ImGui::Text("WARNING, Only Press this button if you finished creating the conversation");
				if (ImGui::Button("Finish And Save Conversation"))
				{
					dm.conversations[currentConversation].occupied = true;
					currentConversation++;

					currentCreationStep = 6;
				}
			}

			if (currentCreationStep == 6)
			{
				creatingNewDialog = false;

			}
		}
	}
	if (ImGui::CollapsingHeader("Edit Conversations"))
	{
		if (dm.conversations[0].occupied == true)
		{
			for (int i = 0; i < MAX_CONVERSATIONS && dm.conversations[i].occupied == true; i++)
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