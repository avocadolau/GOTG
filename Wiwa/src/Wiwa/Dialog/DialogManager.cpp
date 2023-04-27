#include <wipch.h>

#include "DialogManager.h"

#include <Wiwa/core/Application.h>
//#include <Wiwa/scene/SceneManager.h>
#include <Wiwa/scene/Scene.h>

//#define STB_IMAGE_WRITE_IMPLEMENTATION
#include "../vendor/stb/stb_image_write.h"

//#define STB_TRUETYPE_IMPLEMENTATION 
#include "../vendor/stb/stb_truetype.h"

#include <Wiwa/scene/SceneManager.h>

#include <Wiwa/utilities/json/JSONDocument.h>

#include <Wiwa/utilities/easings.h>

namespace Wiwa
{
	DialogManager::DialogManager()
	{
		
	}

	DialogManager::~DialogManager()
	{
		Renderer2D& render = Wiwa::Application::Get().GetRenderer2D();

		render.RemoveInstance(m_Scene, continueImgID);

		for (int i = 0; (i < MAX_CONVERSATIONS) && conversations[i].occupied == true; i++)
		{
			render.RemoveInstance(m_Scene, conversations[i].characterImgID);
			render.RemoveInstance(m_Scene, conversations[i].dialogImgID);

			for (int j = 0; (j < MAX_CONVERSATION_NODES) && conversations[i].nodes[j].occupied == true; j++)
			{
				render.RemoveInstance(m_Scene, conversations[i].nodes[j].text1_imgModeID);
				render.RemoveInstance(m_Scene, conversations[i].nodes[j].text2_imgModeID);
				render.RemoveInstance(m_Scene, conversations[i].nodes[j].text3_imgModeID);
			}
		}

		for (int l = 0; l < MAX_CONVERSATIONS; l++)
		{
			conversations[l].occupied = false;

			for (int m = 0; m < MAX_CONVERSATION_NODES; m++)
			{
				conversations[l].nodes[m].occupied = false;
			}
		}
	}

	bool DialogManager::Init(Scene* scene)
	{
		m_Scene = scene;
		
		Renderer2D& render = Wiwa::Application::Get().GetRenderer2D();

		ResourceId textID = Wiwa::Resources::Load<Wiwa::Image>("assets/HUDImages/Menus/SpeechMenu/dialog_test_placeholder3.png");
		Image* continueImg = Wiwa::Resources::GetResourceById<Wiwa::Image>(textID);

		continueImgID = render.CreateInstancedQuadTex(m_Scene, continueImg->GetTextureId(), continueImg->GetSize(), { 1600,800 }, { 50,50 }, Wiwa::Renderer2D::Pivot::UPLEFT);
		render.DisableInstance(m_Scene, continueImgID);

		//conversations[0].conversationName = "NPC_1";

		//SetDialogText("I am not to interfere, Guardian.", "However, I will tell you this: ", "the Phalanx are a formidable species,", "assets/Fonts/Jade_Smile.ttf", 0, 0);
		//SetDialogText("constantly updating their data banks.", "With them the architecture, too, gets", "updated. ", "assets/Fonts/Jade_Smile.ttf", 0, 1);
		//SetDialogText("Surprisingly, some of the collector’s", "inventory of weaponsandgadgets", "has been seen in the vicinity.", "assets/Fonts/Jade_Smile.ttf", 0, 2);
		//SetDialogText("Do not get lost in the labyrinth of", "machinery, Guardian.", "And good luck.", "assets/Fonts/Jade_Smile.ttf", 0, 3);

		//SetDialogBubbleImage("assets/HUD_Images/menus/speech menu/ui_speech_menu_starlord_bubble-01.png", 0);
		//SetCharacterImage("assets/HUD_Images/menus/speech menu/ui_speech_menu_starlord_withshadows-01.png", 0);

		//conversations[0].occupied = true;

		LoadAllDialogs();

		actualConversationState = 2;
		currentNode = 0;

		keyPressRefreshTimer = 0;

		collidingWithNpc = false;
		
		characterImgPos.x = -50;
		characterImgPos.y = 100; // <--
		bubbleImgPos.x = 640; // <--
		bubbleImgPos.y = 100;

		//newConversation = nullptr;

		for (int e = 0; e < MAX_CONVERSATIONS && conversations[e].occupied == true; e++)
		{
			editorConversations[e].bubbleImagePath = conversations[e].bubbleImagePath;
			editorConversations[e].characterImagePath = conversations[e].characterImagePath;
			editorConversations[e].conversationName = conversations[e].conversationName;
			editorConversations[e].occupied = conversations[e].occupied;

			for (int f = 0; f < MAX_CONVERSATION_NODES && conversations[e].nodes[f].occupied == true; f++)
			{
				editorConversations[e].nodes[f].text1 = conversations[e].nodes[f].text1;
				editorConversations[e].nodes[f].text2 = conversations[e].nodes[f].text2;
				editorConversations[e].nodes[f].text3 = conversations[e].nodes[f].text3;
				editorConversations[e].nodes[f].occupied = conversations[e].nodes[f].occupied;

			}


		}

		return true;
	}


	bool DialogManager::Update()
	{
		if ((Wiwa::Input::IsKeyPressed(Wiwa::Key::Space) || Wiwa::Input::IsButtonPressed(0, 3)) && actualConversationState != 1 && keyPressRefreshTimer > 120 && collidingWithNpc == true)
		{
 			conversationToPlayName = NpcConversationTag.c_str();
			actualConversationState = 0;
  
			keyPressRefreshTimer = 0;
		}
		
		if (actualConversationState == 0 || actualConversationState == 1)
		{
			for (int i = 0; (i < MAX_CONVERSATIONS) && conversations[i].occupied == true; i++)
			{
				if (!strcmp(conversations[i].conversationName.c_str(), conversationToPlayName.c_str()))
				{
					UpdateConversation(i, &Wiwa::Application::Get().GetRenderer2D());
				}
			}
		}

		keyPressRefreshTimer += Time::GetDeltaTime();
		
		return true;
	}

	bool DialogManager::Draw()
	{
		
		
		return false;
	}

	void DialogManager::OnDestroy()
	{
		
	}
	
	bool DialogManager::CleanUp()
	{
		

		return true;
	}

	void DialogManager::UpdateConversation(int conversationNumber, Renderer2D* render)
	{
		if (actualConversationState == 0)
		{
			actualConversationState = 1;
			firstTime = true;
			endTime = false;
			firstTimeTimer = 0;
			endTimeTimer = 0;

			characterImgPos.x = -150;
			characterImgPos.y = 100; // <--
			bubbleImgPos.x = 640; // <--
			bubbleImgPos.y = 0;

			render->EnableInstance(m_Scene, conversations[conversationNumber].characterImgID);
			render->EnableInstance(m_Scene, conversations[conversationNumber].dialogImgID);
		}

		if (actualConversationState == 1)
		{

			InstanceRenderer& instanceRenderer = m_Scene->GetInstanceRenderer(conversations[conversationNumber].nodes[currentNode].text1_imgModeID.renderer_id);
			instanceRenderer.UpdateInstanceColor(conversations[conversationNumber].nodes[currentNode].text1_imgModeID.instance_id, BLACK);
			InstanceRenderer& instanceRenderer2 = m_Scene->GetInstanceRenderer(conversations[conversationNumber].nodes[currentNode].text2_imgModeID.renderer_id);
			instanceRenderer2.UpdateInstanceColor(conversations[conversationNumber].nodes[currentNode].text2_imgModeID.instance_id, BLACK);
			InstanceRenderer& instanceRenderer3 = m_Scene->GetInstanceRenderer(conversations[conversationNumber].nodes[currentNode].text3_imgModeID.renderer_id);
			instanceRenderer3.UpdateInstanceColor(conversations[conversationNumber].nodes[currentNode].text3_imgModeID.instance_id, BLACK);

			if (firstTime == false && endTime == false)
			{
				render->EnableInstance(m_Scene, conversations[conversationNumber].nodes[currentNode].text1_imgModeID);
				render->EnableInstance(m_Scene, conversations[conversationNumber].nodes[currentNode].text2_imgModeID);
				render->EnableInstance(m_Scene, conversations[conversationNumber].nodes[currentNode].text3_imgModeID);
			}
			else if (firstTime == true && firstTimeTimer > 850 && endTime == false)
			{
				render->EnableInstance(m_Scene, conversations[conversationNumber].nodes[currentNode].text1_imgModeID);
				render->EnableInstance(m_Scene, conversations[conversationNumber].nodes[currentNode].text2_imgModeID);
				render->EnableInstance(m_Scene, conversations[conversationNumber].nodes[currentNode].text3_imgModeID);
			}

			if (firstTime == true)
			{
				firstTimeTimer += Time::GetDeltaTime();

				characterImgPos.x = EaseBackOut(firstTimeTimer, -1500, -50 + 1500, 850);
				bubbleImgPos.y = EaseBackOut(firstTimeTimer, 1080, 100 - 1080, 850);

				render->UpdateInstancedQuadTexPosition(m_Scene, conversations[conversationNumber].characterImgID, characterImgPos, Wiwa::Renderer2D::Pivot::UPLEFT);
				render->UpdateInstancedQuadTexPosition(m_Scene, conversations[conversationNumber].dialogImgID, bubbleImgPos, Wiwa::Renderer2D::Pivot::UPLEFT);

				if (firstTimeTimer >= 850)
				{
					firstTime = false;

					characterImgPos.x = -50;
					characterImgPos.y = 100; // <--
					bubbleImgPos.x = 640; // <--
					bubbleImgPos.y = 100;

					render->UpdateInstancedQuadTexPosition(m_Scene, conversations[conversationNumber].characterImgID, characterImgPos, Wiwa::Renderer2D::Pivot::UPLEFT);
					render->UpdateInstancedQuadTexPosition(m_Scene, conversations[conversationNumber].dialogImgID, bubbleImgPos, Wiwa::Renderer2D::Pivot::UPLEFT);
				}
			}
			else if (endTime == true)
			{
				endTimeTimer += Time::GetDeltaTime();
				/*
				Example:

				currentPositionX = EaseSineIn(currentTime, startPositionX, finalPositionX - startPositionX, duration);
				currentTime++;
				*/

				characterImgPos.x = EaseBackIn(endTimeTimer, -50, -1500 + 50, 850);
				bubbleImgPos.y = EaseBackIn(endTimeTimer, 100, 1080 - 100, 850);

				render->UpdateInstancedQuadTexPosition(m_Scene, conversations[conversationNumber].characterImgID, characterImgPos, Wiwa::Renderer2D::Pivot::UPLEFT);
				render->UpdateInstancedQuadTexPosition(m_Scene, conversations[conversationNumber].dialogImgID, bubbleImgPos, Wiwa::Renderer2D::Pivot::UPLEFT);

				if (endTimeTimer >= 850)
				{
					render->DisableInstance(m_Scene, conversations[conversationNumber].dialogImgID);
					render->DisableInstance(m_Scene, conversations[conversationNumber].characterImgID);

					endTime = false;

					characterImgPos.x = -150;
					characterImgPos.y = 100; // <--
					bubbleImgPos.x = 640; // <--
					bubbleImgPos.y = 0;

					actualConversationState = 2;
				}
			}

			if (((keyPressRefreshTimer / 450) % 2) == 0)
			{
				if(firstTime == false && endTime == false) render->EnableInstance(m_Scene, continueImgID);
			}
			else
			{
				render->DisableInstance(m_Scene, continueImgID);
			}

			if ((Wiwa::Input::IsKeyPressed(Wiwa::Key::Space) || Wiwa::Input::IsButtonPressed(0, 3)) && (keyPressRefreshTimer > 850 && firstTime == false && endTime == false))
			{
				render->DisableInstance(m_Scene, conversations[conversationNumber].nodes[currentNode].text1_imgModeID);
				render->DisableInstance(m_Scene, conversations[conversationNumber].nodes[currentNode].text2_imgModeID);
				render->DisableInstance(m_Scene, conversations[conversationNumber].nodes[currentNode].text3_imgModeID);

				render->DisableInstance(m_Scene, continueImgID);

				keyPressRefreshTimer = 0;

				currentNode++;
				if (currentNode >= MAX_CONVERSATION_NODES || conversations[conversationNumber].nodes[currentNode].occupied == false)
				{
					currentNode = 0;
					firstTime = false;
					endTime = true;
				}
			}
		}
	}

	void DialogManager::SetDialogText(char* line1Text, char* line2Text, char* line3Text, const char* fontPath, int conversationNumber, int nodeNumber)
	{
		Wiwa::GuiManager& gm = m_Scene->GetGuiManager();

		Text* text1_imgMode = gm.InitFontForDialog(fontPath, line1Text);
		Text* text2_imgMode = gm.InitFontForDialog(fontPath, line2Text);
		Text* text3_imgMode = gm.InitFontForDialog(fontPath, line3Text);

		conversations[conversationNumber].nodes[nodeNumber].text1 = (std::string)line1Text;
		conversations[conversationNumber].nodes[nodeNumber].text2 = (std::string)line2Text;
		conversations[conversationNumber].nodes[nodeNumber].text3 = (std::string)line3Text;

		Renderer2D& render = Wiwa::Application::Get().GetRenderer2D();

		conversations[conversationNumber].nodes[nodeNumber].text1_imgModeID = render.CreateInstancedQuadTex(m_Scene, text1_imgMode->GetTextureId(), text1_imgMode->GetSize(), {730,640}, {1000,650}, Wiwa::Renderer2D::Pivot::UPLEFT);
		render.DisableInstance(m_Scene, conversations[conversationNumber].nodes[nodeNumber].text1_imgModeID);
		
		conversations[conversationNumber].nodes[nodeNumber].text2_imgModeID = render.CreateInstancedQuadTex(m_Scene, text2_imgMode->GetTextureId(), text2_imgMode->GetSize(), { 730,690 }, { 1000,650 }, Wiwa::Renderer2D::Pivot::UPLEFT);
		render.DisableInstance(m_Scene, conversations[conversationNumber].nodes[nodeNumber].text2_imgModeID);
		
		conversations[conversationNumber].nodes[nodeNumber].text3_imgModeID = render.CreateInstancedQuadTex(m_Scene, text3_imgMode->GetTextureId(), text3_imgMode->GetSize(), { 730,740 }, { 1000,650 }, Wiwa::Renderer2D::Pivot::UPLEFT);
		render.DisableInstance(m_Scene, conversations[conversationNumber].nodes[nodeNumber].text3_imgModeID);

		conversations[conversationNumber].nodes[nodeNumber].occupied = true;
	}

	void DialogManager::SetDialogBubbleImage(const char* path, int conversationNumber)
	{
		Renderer2D& render = Wiwa::Application::Get().GetRenderer2D();

		conversations[conversationNumber].bubbleImagePath = (std::string)path;

		ResourceId textID = Wiwa::Resources::Load<Wiwa::Image>(path);
		Image* dialogImg = Wiwa::Resources::GetResourceById<Wiwa::Image>(textID);

		conversations[conversationNumber].dialogImgID = render.CreateInstancedQuadTex(m_Scene, dialogImg->GetTextureId(), dialogImg->GetSize(), { 640,100 }, { 1080,1080 }, Wiwa::Renderer2D::Pivot::UPLEFT);
		render.DisableInstance(m_Scene, conversations[conversationNumber].dialogImgID);
	}

	void DialogManager::SetCharacterImage(const char* path, int conversationNumber)
	{
		Renderer2D& render = Wiwa::Application::Get().GetRenderer2D();

		conversations[conversationNumber].characterImagePath = (std::string)path;

		ResourceId textID = Wiwa::Resources::Load<Wiwa::Image>(path);
		Image* characterImg = Wiwa::Resources::GetResourceById<Wiwa::Image>(textID);

		conversations[conversationNumber].characterImgID = render.CreateInstancedQuadTex(m_Scene, characterImg->GetTextureId(), characterImg->GetSize(), { -50,100 }, { 1024,1024 }, Wiwa::Renderer2D::Pivot::UPLEFT);
		render.DisableInstance(m_Scene, conversations[conversationNumber].characterImgID);
	}

	void DialogManager::SaveAllDialogs()
	{
		Wiwa::JSONDocument doc;

		for (int i = 0; (i < MAX_CONVERSATIONS) && conversations[i].occupied == true; i++)
		{
			std::string s = std::to_string(i);

			std::string memberNameConversation = "Name_Conversation" + s;
			std::string memberNameBubbleImage = "BubbleImagePath_Conversation" + s;
			std::string memberNameCharacterImage = "CharacterImagePath_Conversation" + s;

			doc.AddMember(memberNameConversation.c_str(), conversations[i].conversationName.c_str());
			doc.AddMember(memberNameBubbleImage.c_str(), conversations[i].bubbleImagePath.c_str());
			doc.AddMember(memberNameCharacterImage.c_str(), conversations[i].characterImagePath.c_str());

			for (int j = 0; j < MAX_CONVERSATION_NODES && conversations[i].nodes[j].occupied == true; j++)
			{
				std::string s2 = std::to_string(j);

				std::string conv = "Conversation" + s;

				std::string memberNameLine1 = "_Line1_Node" + s2;
				std::string memberNameLine2 = "_Line2_Node" + s2;
				std::string memberNameLine3 = "_Line3_Node" + s2;
				memberNameLine1 = conv + memberNameLine1;
				memberNameLine2 = conv + memberNameLine2;
				memberNameLine3 = conv + memberNameLine3;

				doc.AddMember(memberNameLine1.c_str(), conversations[i].nodes[j].text1.c_str());
				doc.AddMember(memberNameLine2.c_str(), conversations[i].nodes[j].text2.c_str());
				doc.AddMember(memberNameLine3.c_str(), conversations[i].nodes[j].text3.c_str());

			}
		}

		std::string path = "config/conversations";
		path += ".wiconversation";
		doc.save_file(path.c_str());
	}

	void DialogManager::LoadAllDialogs()
	{

		bool breakConversationsLoop = false;
		bool breakNodesLoop = false;

		Wiwa::JSONDocument doc("config/conversations.wiconversation");
		if (doc.IsObject())
		{
			for (int i = 0; (i < MAX_CONVERSATIONS) && breakConversationsLoop == false; i++)
			{
				std::string s = std::to_string(i);

				std::string memberNameConversation = "Name_Conversation" + s;
				std::string memberNameBubbleImage = "BubbleImagePath_Conversation" + s;
				std::string memberNameCharacterImage = "CharacterImagePath_Conversation" + s;

				if (doc.HasMember(memberNameConversation.c_str())
					&& doc.HasMember(memberNameBubbleImage.c_str())
					&& doc.HasMember(memberNameCharacterImage.c_str()))
				{
					breakNodesLoop = false;
				}

				for (int j = 0; (j < MAX_CONVERSATION_NODES) && breakNodesLoop == false; j++)
				{
					std::string s2 = std::to_string(j);

					std::string conv = "Conversation" + s;

					std::string memberNameLine1 = "_Line1_Node" + s2;
					std::string memberNameLine2 = "_Line2_Node" + s2;
					std::string memberNameLine3 = "_Line3_Node" + s2;
					memberNameLine1 = conv + memberNameLine1;
					memberNameLine2 = conv + memberNameLine2;
					memberNameLine3 = conv + memberNameLine3;

					if (doc.HasMember(memberNameLine1.c_str())
						&& doc.HasMember(memberNameLine2.c_str())
						&& doc.HasMember(memberNameLine3.c_str()))
					{
						SetDialogText((char*)doc[memberNameLine1.c_str()].as_string(), (char*)doc[memberNameLine2.c_str()].as_string(), (char*)doc[memberNameLine3.c_str()].as_string(), "assets/Fonts/Jade_Smile.ttf", i, j);
					}
					else
					{
						breakNodesLoop = true;
					}
				}

				if (doc.HasMember(memberNameConversation.c_str())
					&& doc.HasMember(memberNameBubbleImage.c_str())
					&& doc.HasMember(memberNameCharacterImage.c_str()))
				{
					conversations[i].conversationName = doc[memberNameConversation.c_str()].as_string();
					SetDialogBubbleImage(doc[memberNameBubbleImage.c_str()].as_string(), i);
					SetCharacterImage(doc[memberNameCharacterImage.c_str()].as_string(), i);
					conversations[i].occupied = true;

				}
				else
				{
					breakConversationsLoop = true;
				}

			}
		}

		doc.save_file("config/conversations.wiconversation");
	}
	
}
