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

namespace Wiwa
{
	DialogManager::DialogManager()
	{

	}

	DialogManager::~DialogManager()
	{
		Renderer2D& render = Wiwa::Application::Get().GetRenderer2D();

		for (int i = 0; (i < MAX_CONVERSATIONS) && conversations[i].occupied == true; i++)
		{
			render.RemoveInstance(m_Scene, conversations[i].characterImgID);
			render.RemoveInstance(m_Scene, conversations[i].dialogImgID);
			render.RemoveInstance(m_Scene, conversations[i].continueImgID);

			for (int j = 0; (j < MAX_CONVERSATION_NODES) && conversations[i].nodes[j].occupied == true; j++)
			{
				render.RemoveInstance(m_Scene, conversations[i].nodes[j].text1_imgModeID);
				render.RemoveInstance(m_Scene, conversations[i].nodes[j].text2_imgModeID);
				render.RemoveInstance(m_Scene, conversations[i].nodes[j].text3_imgModeID);
			}
		}
	}

	bool DialogManager::Init(Scene* scene)
	{
		m_Scene = scene;
		

		/*Conversation* newConversation = new Conversation();

		newConversation->conversationName = "Test conversation";
		
		SetDialogText("This is a test dialog in line 1", "We are on node... 1", "agniag", "assets/Fonts/Jade_Smile.ttf", *newConversation);
		SetDialogText("Aaaaaaaa in line 1", "We are on node... 2", "agbQAGIU", "assets/Fonts/Jade_Smile.ttf", *newConversation);
		SetDialogText("Is this a test dialog in line 1?", "We are on node... 3", "dfgnhNIUFniiauergnb", "assets/Fonts/Jade_Smile.ttf", *newConversation);

		SetContinueIndicatorImage("assets/HUD_Images/dialog_images/dialog_test_placeholder3.png", *newConversation);
		SetDialogBubbleImage("assets/HUD_Images/menus/speech menu/ui_speech_menu_starlord_bubble-01.png", *newConversation);
		SetCharacterImage("assets/HUD_Images/menus/speech menu/ui_speech_menu_starlord_withshadows-01.png", *newConversation);

		conversations.push_back(newConversation);*/

		conversations[0].conversationName = "NPC_1";

		SetDialogText("I am not to interfere, Guardian.", "However, I will tell you this: ", "the Phalanx are a formidable species,", "assets/Fonts/Jade_Smile.ttf", 0, 0);
		SetDialogText("constantly updating their data banks.", "With them the architecture, too, gets", "updated. ", "assets/Fonts/Jade_Smile.ttf", 0, 1);
		SetDialogText("Surprisingly, some of the collector’s", "inventory of weaponsandgadgets", "has been seen in the vicinity.", "assets/Fonts/Jade_Smile.ttf", 0, 2);
		SetDialogText("Do not get lost in the labyrinth of", "machinery, Guardian.", "And good luck.", "assets/Fonts/Jade_Smile.ttf", 0, 3);

		SetContinueIndicatorImage("assets/HUD_Images/dialog_images/dialog_test_placeholder3.png", 0);
		SetDialogBubbleImage("assets/HUD_Images/menus/speech menu/ui_speech_menu_starlord_bubble-01.png", 0);
		SetCharacterImage("assets/HUD_Images/menus/speech menu/ui_speech_menu_watcher-01.png", 0);

		conversations[0].occupied = true;

		actualConversationState = 2;
		currentNode = 0;

		keyPressRefreshTimer = 0;

		collidingWithNpc = false;

		//newConversation = nullptr;

		return true;
	}


	bool DialogManager::Update()  // Continue: mando Y, teclado Space - In total, two custom images: character and bubble - one fix image: continue sign
	{
		if ((Wiwa::Input::IsKeyPressed(Wiwa::Key::Space) || Wiwa::Input::IsButtonPressed(0, 3)) && actualConversationState != 1 && keyPressRefreshTimer > 120 && collidingWithNpc == true)
		{
			conversationToPlayName = NpcConversationTag;
			actualConversationState = 0;

			keyPressRefreshTimer = 0;
		}
		
		if (actualConversationState == 0 || actualConversationState == 1)
		{
			for (int i = 0; (i < MAX_CONVERSATIONS) && conversations[i].occupied == true; i++)
			{
				if (!strcmp(conversations[i].conversationName, conversationToPlayName))
				{
					UpdateConversation(i, &Wiwa::Application::Get().GetRenderer2D());
				}
			}
		}

		keyPressRefreshTimer++;
		
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
		if(actualConversationState == 0) actualConversationState = 1;

		if (actualConversationState == 1)
		{
			render->EnableInstance(m_Scene, conversations[conversationNumber].characterImgID);
			render->EnableInstance(m_Scene, conversations[conversationNumber].dialogImgID);

			render->EnableInstance(m_Scene, conversations[conversationNumber].nodes[currentNode].text1_imgModeID);
			render->EnableInstance(m_Scene, conversations[conversationNumber].nodes[currentNode].text2_imgModeID);
			render->EnableInstance(m_Scene, conversations[conversationNumber].nodes[currentNode].text3_imgModeID);

			if (((keyPressRefreshTimer / 60) % 2) == 0)
			{
				render->EnableInstance(m_Scene, conversations[conversationNumber].continueImgID);
			}
			else
			{
				render->DisableInstance(m_Scene, conversations[conversationNumber].continueImgID);
			}

			if ((Wiwa::Input::IsKeyPressed(Wiwa::Key::Space) || Wiwa::Input::IsButtonPressed(0, 3)) && keyPressRefreshTimer > 120)
			{
				render->DisableInstance(m_Scene, conversations[conversationNumber].nodes[currentNode].text1_imgModeID);
				render->DisableInstance(m_Scene, conversations[conversationNumber].nodes[currentNode].text2_imgModeID);
				render->DisableInstance(m_Scene, conversations[conversationNumber].nodes[currentNode].text3_imgModeID);

				render->DisableInstance(m_Scene, conversations[conversationNumber].dialogImgID);
				render->DisableInstance(m_Scene, conversations[conversationNumber].continueImgID);
				render->DisableInstance(m_Scene, conversations[conversationNumber].characterImgID);

				keyPressRefreshTimer = 0;

				currentNode++;
				if (currentNode >= MAX_CONVERSATION_NODES || conversations[conversationNumber].nodes[currentNode].occupied == false)
				{
					currentNode = 0;
					actualConversationState = 2;
				}
			}
		}
	}

	void DialogManager::SetDialogText(char* line1Text, char* line2Text, char* line3Text, const char* fontPath, int conversationNumber, int nodeNumber)
	{
		Wiwa::GuiManager& gm = m_Scene->GetGuiManager();

		//conversations[conversationNumber].nodes[nodeNumber].text1_imgMode = new Text();
		//conversations[conversationNumber].nodes[nodeNumber].text2_imgMode = new Text();
		//conversations[conversationNumber].nodes[nodeNumber].text3_imgMode = new Text();

		conversations[conversationNumber].nodes[nodeNumber].text1_imgMode = *gm.InitFontForDialog(fontPath, line1Text);
		conversations[conversationNumber].nodes[nodeNumber].text2_imgMode = *gm.InitFontForDialog(fontPath, line2Text);
		conversations[conversationNumber].nodes[nodeNumber].text3_imgMode = *gm.InitFontForDialog(fontPath, line3Text);

		Renderer2D& render = Wiwa::Application::Get().GetRenderer2D();

		conversations[conversationNumber].nodes[nodeNumber].text1_imgModeID = render.CreateInstancedQuadTex(m_Scene, conversations[conversationNumber].nodes[nodeNumber].text1_imgMode.GetTextureId(), conversations[conversationNumber].nodes[nodeNumber].text1_imgMode.GetSize(), {720,650}, {1000,450}, Wiwa::Renderer2D::Pivot::UPLEFT);
		render.DisableInstance(m_Scene, conversations[conversationNumber].nodes[nodeNumber].text1_imgModeID);
		
		conversations[conversationNumber].nodes[nodeNumber].text2_imgModeID = render.CreateInstancedQuadTex(m_Scene, conversations[conversationNumber].nodes[nodeNumber].text2_imgMode.GetTextureId(), conversations[conversationNumber].nodes[nodeNumber].text2_imgMode.GetSize(), { 720,700 }, { 1000,450 }, Wiwa::Renderer2D::Pivot::UPLEFT);
		render.DisableInstance(m_Scene, conversations[conversationNumber].nodes[nodeNumber].text2_imgModeID);
		
		conversations[conversationNumber].nodes[nodeNumber].text3_imgModeID = render.CreateInstancedQuadTex(m_Scene, conversations[conversationNumber].nodes[nodeNumber].text3_imgMode.GetTextureId(), conversations[conversationNumber].nodes[nodeNumber].text3_imgMode.GetSize(), { 720,750 }, { 1000,450 }, Wiwa::Renderer2D::Pivot::UPLEFT);
		render.DisableInstance(m_Scene, conversations[conversationNumber].nodes[nodeNumber].text3_imgModeID);

		conversations[conversationNumber].nodes[nodeNumber].occupied = true;
	}

	void DialogManager::SetDialogBubbleImage(const char* path, int conversationNumber)
	{
		Renderer2D& render = Wiwa::Application::Get().GetRenderer2D();

		ResourceId textID = Wiwa::Resources::Load<Wiwa::Image>(path);
		conversations[conversationNumber].dialogImg = Wiwa::Resources::GetResourceById<Wiwa::Image>(textID);

		conversations[conversationNumber].dialogImgID = render.CreateInstancedQuadTex(m_Scene, conversations[conversationNumber].dialogImg->GetTextureId(), conversations[conversationNumber].dialogImg->GetSize(), { 640,100 }, { 1080,1080 }, Wiwa::Renderer2D::Pivot::UPLEFT);
		render.DisableInstance(m_Scene, conversations[conversationNumber].dialogImgID);
	}

	void DialogManager::SetCharacterImage(const char* path, int conversationNumber)
	{
		Renderer2D& render = Wiwa::Application::Get().GetRenderer2D();

		ResourceId textID = Wiwa::Resources::Load<Wiwa::Image>(path);
		conversations[conversationNumber].characterImg = Wiwa::Resources::GetResourceById<Wiwa::Image>(textID);

		conversations[conversationNumber].characterImgID = render.CreateInstancedQuadTex(m_Scene, conversations[conversationNumber].characterImg->GetTextureId(), conversations[conversationNumber].characterImg->GetSize(), { -50,100 }, { 1024,1024 }, Wiwa::Renderer2D::Pivot::UPLEFT);
		render.DisableInstance(m_Scene, conversations[conversationNumber].characterImgID);
	}

	void DialogManager::SetContinueIndicatorImage(const char* path, int conversationNumber)
	{
		Renderer2D& render = Wiwa::Application::Get().GetRenderer2D();

		ResourceId textID = Wiwa::Resources::Load<Wiwa::Image>(path);
		conversations[conversationNumber].continueImg = Wiwa::Resources::GetResourceById<Wiwa::Image>(textID);

		conversations[conversationNumber].continueImgID = render.CreateInstancedQuadTex(m_Scene, conversations[conversationNumber].continueImg->GetTextureId(), conversations[conversationNumber].continueImg->GetSize(), { 1600,800 }, { 50,50 }, Wiwa::Renderer2D::Pivot::UPLEFT);
		render.DisableInstance(m_Scene, conversations[conversationNumber].continueImgID);
	}
	
}
