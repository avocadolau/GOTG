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

		if (creatingNewDialog == true)
		{
			if(currentCreationStep == 0)
			{
				currentNode = 0;
				currentConversation = 0;

				for (int k = 0; dm.conversations[k].occupied == true; k++)
				{
					currentConversation++;
				}

				currentCreationStep = 1;
			}

			if (currentCreationStep == 1)
			{
				

				std::string collapsingHeaderName = "Edit Node #" + numNodesOnCurrentCreation;

				ImGui::Text("Currently Editing Node # %i", numNodesOnCurrentCreation);

				ImGui::InputText("Line 1 text", &dm.conversations[currentConversation].nodes[currentNode].text1);
				ImGui::InputText("Line 2 text", &dm.conversations[currentConversation].nodes[currentNode].text2);
				ImGui::InputText("Line 3 text", &dm.conversations[currentConversation].nodes[currentNode].text3);

				ImGui::Text("WARNING, Only Press this button if you fully finished creating the current dialog node:");
				ImGui::PushID(numNodesOnCurrentCreation);
				if (ImGui::Button("Save Current Node's Text Lines"))
				{
					dm.SetDialogText(
						(char*)dm.conversations[currentConversation].nodes[currentNode].text1.c_str(),
						(char*)dm.conversations[currentConversation].nodes[currentNode].text2.c_str(),
						(char*)dm.conversations[currentConversation].nodes[currentNode].text3.c_str(),
						"assets/Fonts/Jade_Smile.ttf", currentConversation, currentNode);
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
			//ImGui::SameLine();

			if (currentCreationStep == 2)
			{
				//dm.SetContinueIndicatorImage("assets/HUD_Images/dialog_images/dialog_test_placeholder3.png", *dm.conversationCreator_P);

				/*Renderer2D& render = Wiwa::Application::Get().GetRenderer2D();

				ResourceId textID = Wiwa::Resources::Load<Wiwa::Image>("assets/HUD_Images/dialog_images/dialog_test_placeholder3.png");
				Image* continueImg = Wiwa::Resources::GetResourceById<Wiwa::Image>(textID);

				dm.continueImgID = render.CreateInstancedQuadTex(dm.m_Scene, continueImg->GetTextureId(), continueImg->GetSize(), { 1600,800 }, { 50,50 }, Wiwa::Renderer2D::Pivot::UPLEFT);
				render.DisableInstance(dm.m_Scene, dm.continueImgID);*/

				currentCreationStep = 3;
			}

			if (currentCreationStep == 3)
			{
				ImGui::Text("Bubble image:");
				ImGui::SameLine();
				//Wiwa::Image* mat = Wiwa::Resources::GetResourceById<Wiwa::Image>(mesh->materialId);
				//AssetContainer(std::filesystem::path(mat->GetTextureId()).stem().string().c_str());
				AssetContainer(dm.conversations[currentConversation].bubbleImagePath.c_str());
				if (ImGui::BeginDragDropTarget())
				{
					if (const ImGuiPayload* payload = ImGui::AcceptDragDropPayload("CONTENT_BROWSER_ITEM"))
					{
						const wchar_t* path = (const wchar_t*)payload->Data;
						std::wstring ws(path);
						std::string pathS(ws.begin(), ws.end());
						std::filesystem::path p = pathS.c_str();
						if (p.extension() == ".png")
						{
							dm.conversations[currentConversation].bubbleImagePath = pathS;
							dm.SetDialogBubbleImage(pathS.c_str(), currentConversation);
						}
					}
					ImGui::EndDragDropTarget();
				}
				//dm.SetDialogBubbleImage("assets/HUD_Images/menus/speech menu/ui_speech_menu_starlord_bubble-01.png", currentConversation);
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
				AssetContainer(dm.conversations[currentConversation].characterImagePath.c_str());
				if (ImGui::BeginDragDropTarget())
				{
					if (const ImGuiPayload* payload = ImGui::AcceptDragDropPayload("CONTENT_BROWSER_ITEM"))
					{
						const wchar_t* path = (const wchar_t*)payload->Data;
						std::wstring ws(path);
						std::string pathS(ws.begin(), ws.end());
						std::filesystem::path p = pathS.c_str();
						if (p.extension() == ".png")
						{
							dm.conversations[currentConversation].characterImagePath = pathS;
							dm.SetCharacterImage(pathS.c_str(), currentConversation);
						}
					}
					ImGui::EndDragDropTarget();
				}
				//dm.SetCharacterImage("assets/HUD_Images/menus/speech menu/ui_speech_menu_starlord_withshadows-01.png", currentConversation);
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
					dm.SaveAllDialogs();
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
	ImGui::Separator();
	ImGui::Text("Edit Conversations:");
	ImGui::NewLine();

	if (dm.conversations[0].occupied == true)
	{
		for (int i = 0; i < MAX_CONVERSATIONS && dm.conversations[i].occupied == true; i++)
		{
			ImGui::PushID(i);
			if (ImGui::CollapsingHeader(dm.conversations[i].conversationName.c_str()))
			{
				ImGui::Text("Node %i:", currentEditingNode);

				ImGui::PushID(currentEditingNode);
				ImGui::InputText("Line 1 text", &dm.conversations[i].nodes[currentEditingNode].text1);
				ImGui::PopID();
				ImGui::PushID(currentEditingNode);
				ImGui::InputText("Line 2 text", &dm.conversations[i].nodes[currentEditingNode].text2);
				ImGui::PopID();
				ImGui::PushID(currentEditingNode);
				ImGui::InputText("Line 3 text", &dm.conversations[i].nodes[currentEditingNode].text3);
				ImGui::PopID();

				ImGui::PushID(currentEditingNode);
				if (currentEditingNode > 0)
				{
					if (ImGui::Button("< Previous Node"))
					{
						currentEditingNode--;
					}
					
				}
				else
				{
					if (ImGui::Button("       -       "))
					{
						
					}
				}
				ImGui::PopID();
				ImGui::SameLine();
				ImGui::PushID(currentEditingNode);
				if (dm.conversations[i].nodes[currentEditingNode + 1].occupied == true)
				{
					if (ImGui::Button("Next Node >"))
					{
						currentEditingNode++;
					}
				}
				else
				{
					if (ImGui::Button("       -       "))
					{
						
					}
				}
				ImGui::PopID();
				if (ImGui::Button("New Node"))
				{
					bool exit = false;
					for (int j = 0; j < MAX_CONVERSATION_NODES && exit == false; j++)
					{
						if (dm.conversations[i].nodes[j + 1].occupied == false)
						{
							dm.conversations[i].nodes[j + 1].occupied = true;
							exit = true;
						}
					}
				}
				ImGui::SameLine();
				if (ImGui::Button("Delete Node"))
				{
					dm.conversations[i].nodes[currentEditingNode].occupied = false;

					int isNodeOccupiedNumTimesFalse = 0;

					for (int l = 0; (l < MAX_CONVERSATIONS) && (isNodeOccupiedNumTimesFalse < 2); l++)
					{
						if (dm.conversations[i].nodes[l].occupied == false)
						{
							isNodeOccupiedNumTimesFalse++;
						}

						if (isNodeOccupiedNumTimesFalse == 1)
						{
							dm.conversations[i].nodes[l].text1 = dm.conversations[i].nodes[l + 1].text1;
							dm.conversations[i].nodes[l].text2 = dm.conversations[i].nodes[l + 1].text2;
							dm.conversations[i].nodes[l].text3 = dm.conversations[i].nodes[l + 1].text3;
							dm.conversations[i].nodes[l].occupied = dm.conversations[i].nodes[l + 1].occupied;

						}
					}
				}

				ImGui::PushID(currentEditingNode);
				if (ImGui::Button(" -= SAVE NODE =- "))
				{
					dm.SetDialogText(
						(char*)dm.conversations[i].nodes[currentEditingNode].text1.c_str(),
						(char*)dm.conversations[i].nodes[currentEditingNode].text2.c_str(),
						(char*)dm.conversations[i].nodes[currentEditingNode].text3.c_str(),
						"assets/Fonts/Jade_Smile.ttf", i, currentEditingNode);
				}
				ImGui::PopID();

				ImGui::NewLine();

				ImGui::Text("Character image:");
				ImGui::SameLine();
				AssetContainer(dm.conversations[i].characterImagePath.c_str());
				if (ImGui::BeginDragDropTarget())
				{
					if (const ImGuiPayload* payload = ImGui::AcceptDragDropPayload("CONTENT_BROWSER_ITEM"))
					{
						const wchar_t* path = (const wchar_t*)payload->Data;
						std::wstring ws(path);
						std::string pathS(ws.begin(), ws.end());
						std::filesystem::path p = pathS.c_str();
						if (p.extension() == ".png")
						{
							dm.conversations[i].characterImagePath = pathS;
							
						}
					}
					ImGui::EndDragDropTarget();
				}
				ImGui::PushID(i);
				if (ImGui::Button(" -= Save Character Image =- "))
				{
					dm.SetCharacterImage(dm.conversations[i].characterImagePath.c_str(), i);
				}
				ImGui::PopID();
				ImGui::NewLine();

				ImGui::Text("Bubble image:");
				ImGui::SameLine();
				AssetContainer(dm.conversations[i].bubbleImagePath.c_str());
				if (ImGui::BeginDragDropTarget())
				{
					if (const ImGuiPayload* payload = ImGui::AcceptDragDropPayload("CONTENT_BROWSER_ITEM"))
					{
						const wchar_t* path = (const wchar_t*)payload->Data;
						std::wstring ws(path);
						std::string pathS(ws.begin(), ws.end());
						std::filesystem::path p = pathS.c_str();
						if (p.extension() == ".png")
						{
							dm.conversations[i].bubbleImagePath = pathS;
							
						}
					}
					ImGui::EndDragDropTarget();
				}
				ImGui::PushID(i);
				if (ImGui::Button(" -= Save Bubble Image =- "))
				{
					dm.SetDialogBubbleImage(dm.conversations[i].bubbleImagePath.c_str(), i);
				}
				ImGui::PopID();
				ImGui::NewLine();

				ImGui::PushID(i);
				if (ImGui::Button(" ~ Save Conversation ~ "))
				{
					currentEditingNode = 0;
					dm.SaveAllDialogs();
				}
				ImGui::PopID();
				ImGui::SameLine();
				ImGui::PushID(i);
				if (ImGui::Button(" ~ Delete Conversation ~ "))
				{
					dm.conversations[i].occupied = false;

					int isOccupiedNumTimesFalse = 0;

					for (int l = 0; (l < MAX_CONVERSATIONS) && (isOccupiedNumTimesFalse < 2); l++)
					{
						if (dm.conversations[l].occupied == false)
						{
							isOccupiedNumTimesFalse++;
						}

						if (isOccupiedNumTimesFalse == 1)
						{
							dm.conversations[l].bubbleImagePath = dm.conversations[l + 1].bubbleImagePath;
							dm.conversations[l].characterImagePath = dm.conversations[l + 1].characterImagePath;
							//dm.conversations[l].characterImgID = dm.conversations[l + 1].characterImgID;
							dm.conversations[l].conversationName = dm.conversations[l + 1].conversationName;
							//dm.conversations[l].dialogImgID = dm.conversations[l + 1].dialogImgID;
							
							for (int m = 0; m < MAX_CONVERSATION_NODES; m++)
							{
								dm.conversations[l].nodes[m].text1 = dm.conversations[l + 1].nodes[m].text1;
								//dm.conversations[l].nodes[m].text1_imgModeID = dm.conversations[l + 1].nodes[m].text1_imgModeID;
								dm.conversations[l].nodes[m].text2 = dm.conversations[l + 1].nodes[m].text2;
								//dm.conversations[l].nodes[m].text2_imgModeID = dm.conversations[l + 1].nodes[m].text2_imgModeID;
								dm.conversations[l].nodes[m].text3 = dm.conversations[l + 1].nodes[m].text3;
								//dm.conversations[l].nodes[m].text3_imgModeID = dm.conversations[l + 1].nodes[m].text3_imgModeID;
								dm.conversations[l].nodes[m].occupied = dm.conversations[l + 1].nodes[m].occupied;
							}

							dm.conversations[l].occupied = dm.conversations[l + 1].occupied;

						}
					}

					currentEditingNode = 0;
					dm.SaveAllDialogs();
				}
				ImGui::PopID();
			}
			ImGui::PopID();
		}
	}
	else
	{
		ImGui::Text("No conversations have been created yet.");
	}
	
	ImGui::End();
}