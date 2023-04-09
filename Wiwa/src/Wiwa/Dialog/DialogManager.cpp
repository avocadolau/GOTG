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

	DialogManager::~DialogManager() {}

	bool DialogManager::Init(Scene* scene)
	{
		m_Scene = scene;
		
		//InitFont("assets/arial.ttf");
		//Test remove once done
		//InitFont("assets/arial.ttf","prueba1");

		Conversation* newConversation = new Conversation();

		newConversation->conversationName = "Test conversation";
		/*
		SetDialogText("This is a test dialog in line 1", "We are on node... 1", "agniag", "assets/Fonts/Jade_Smile.ttf", *newConversation);
		SetDialogText("Aaaaaaaa in line 1", "We are on node... 2", "agbQAGIU", "assets/Fonts/Jade_Smile.ttf", *newConversation);
		SetDialogText("Is this a test dialog in line 1?", "We are on node... 3", "dfgnhNIUFniiauergnb", "assets/Fonts/Jade_Smile.ttf", *newConversation);
		SetDialogText("This is a test dialog in line 1!", "We are on node... 4", "hjs89hbjs", "assets/Fonts/Jade_Smile.ttf", *newConversation);
		SetDialogText("Line 1", "We are on node... 5", "sbgsgjs", "assets/Fonts/Jade_Smile.ttf", *newConversation);
		SetDialogText("....................", "We are on node... 6", "gsks8gksr", "assets/Fonts/Jade_Smile.ttf", *newConversation);

		SetContinueIndicatorImage("assets/HUD_Images/dialog_images/dialog_test_placeholder3.png", *newConversation);
		SetDialogBubbleImage("assets/HUD_Images/menus/speech menu/ui_speech_menu_starlord_bubble-01.png", *newConversation);
		SetCharacterImage("assets/HUD_Images/menus/speech menu/ui_speech_menu_starlord_withshadows-01.png", *newConversation);*/

		conversations.push_back(newConversation);

		actualConversationState = 2;
		currentNode = 0;

		keyPressRefreshTimer = 0;

		return true;
	}


	bool DialogManager::Update()  // Continue: mando Y, teclado Space - In total, two custom images: character and bubble - one fix image: continue sign
	{
		if ((Wiwa::Input::IsKeyPressed(Wiwa::Key::Space) || Wiwa::Input::IsButtonPressed(0, 3)) && actualConversationState != 1 && keyPressRefreshTimer > 120) // Just for testing purposes, actual game logic goes here before UpdateConversation();
		{
			conversationToPlayName = "Test conversation";
			actualConversationState = 0;

			keyPressRefreshTimer = 0;
		}
		
		if (actualConversationState == 0 || actualConversationState == 1)
		{
			for (int i = 0; i < conversations.size(); i++)
			{
				if (conversations[i]->conversationName == conversationToPlayName)
				{
					UpdateConversation(*conversations[i], &Wiwa::Application::Get().GetRenderer2D());
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
	
	bool DialogManager::CleanUp()
	{
		

		return true;
	}

	void DialogManager::UpdateConversation(Conversation conversation, Renderer2D* render)
	{
		if(actualConversationState == 0) actualConversationState = 1;

		if (actualConversationState == 1)
		{
			render->EnableInstance(m_Scene, conversation.characterImgID);
			render->EnableInstance(m_Scene, conversation.dialogImgID);

			render->EnableInstance(m_Scene, conversation.nodes[currentNode]->text1_imgModeID);
			render->EnableInstance(m_Scene, conversation.nodes[currentNode]->text2_imgModeID);
			render->EnableInstance(m_Scene, conversation.nodes[currentNode]->text3_imgModeID);

			if (((keyPressRefreshTimer / 60) % 2) == 0)
			{
				render->EnableInstance(m_Scene, conversation.continueImgID);
			}
			else
			{
				render->DisableInstance(m_Scene, conversation.continueImgID);
			}

			if ((Wiwa::Input::IsKeyPressed(Wiwa::Key::Space) || Wiwa::Input::IsButtonPressed(0, 3)) && keyPressRefreshTimer > 120)
			{
				render->DisableInstance(m_Scene, conversation.nodes[currentNode]->text1_imgModeID);
				render->DisableInstance(m_Scene, conversation.nodes[currentNode]->text2_imgModeID);
				render->DisableInstance(m_Scene, conversation.nodes[currentNode]->text3_imgModeID);

				render->DisableInstance(m_Scene, conversation.dialogImgID);
				render->DisableInstance(m_Scene, conversation.continueImgID);
				render->DisableInstance(m_Scene, conversation.characterImgID);

				keyPressRefreshTimer = 0;

				currentNode++;
				if (currentNode >= conversation.nodes.size())
				{
					currentNode = 0;
					actualConversationState = 2;
				}
			}
		}
	}

	void DialogManager::SetDialogText(char* line1Text, char* line2Text, char* line3Text, const char* fontPath, Conversation& conversation)
	{
		DialogNode* newDialogNode = new DialogNode();

		Wiwa::GuiManager& gm = m_Scene->GetGuiManager();

		newDialogNode->text1_imgMode = new Text();
		newDialogNode->text2_imgMode = new Text();
		newDialogNode->text3_imgMode = new Text();

		newDialogNode->text1_imgMode = gm.InitFontForDialog(fontPath, line1Text);
		newDialogNode->text2_imgMode = gm.InitFontForDialog(fontPath, line2Text);
		newDialogNode->text3_imgMode = gm.InitFontForDialog(fontPath, line3Text);

		Renderer2D& render = Wiwa::Application::Get().GetRenderer2D();

		newDialogNode->text1_imgModeID = render.CreateInstancedQuadTex(m_Scene, newDialogNode->text1_imgMode->GetTextureId(), newDialogNode->text1_imgMode->GetSize(), { 720,650 }, { 1500,450 }, Wiwa::Renderer2D::Pivot::UPLEFT);
		render.DisableInstance(m_Scene, newDialogNode->text1_imgModeID);
		
		newDialogNode->text2_imgModeID = render.CreateInstancedQuadTex(m_Scene, newDialogNode->text2_imgMode->GetTextureId(), newDialogNode->text2_imgMode->GetSize(), { 720,700 }, { 1500,450 }, Wiwa::Renderer2D::Pivot::UPLEFT);
		render.DisableInstance(m_Scene, newDialogNode->text2_imgModeID);
		
		newDialogNode->text3_imgModeID = render.CreateInstancedQuadTex(m_Scene, newDialogNode->text3_imgMode->GetTextureId(), newDialogNode->text3_imgMode->GetSize(), { 720,750 }, { 1500,450 }, Wiwa::Renderer2D::Pivot::UPLEFT);
		render.DisableInstance(m_Scene, newDialogNode->text3_imgModeID);

		conversation.nodes.push_back(newDialogNode);
	}

	void DialogManager::SetDialogBubbleImage(const char* path, Conversation& conversation)
	{
		Renderer2D& render = Wiwa::Application::Get().GetRenderer2D();

		ResourceId textID = Wiwa::Resources::Load<Wiwa::Image>(path);
		conversation.dialogImg = Wiwa::Resources::GetResourceById<Wiwa::Image>(textID);

		conversation.dialogImgID = render.CreateInstancedQuadTex(m_Scene, conversation.dialogImg->GetTextureId(), conversation.dialogImg->GetSize(), { 640,100 }, { 1080,1080 }, Wiwa::Renderer2D::Pivot::UPLEFT);
		render.DisableInstance(m_Scene, conversation.dialogImgID);
	}

	void DialogManager::SetCharacterImage(const char* path, Conversation& conversation)
	{
		Renderer2D& render = Wiwa::Application::Get().GetRenderer2D();

		ResourceId textID = Wiwa::Resources::Load<Wiwa::Image>(path);
		conversation.characterImg = Wiwa::Resources::GetResourceById<Wiwa::Image>(textID);

		conversation.characterImgID = render.CreateInstancedQuadTex(m_Scene, conversation.characterImg->GetTextureId(), conversation.characterImg->GetSize(), { -50,100 }, { 1024,1024 }, Wiwa::Renderer2D::Pivot::UPLEFT);
		render.DisableInstance(m_Scene, conversation.characterImgID);
	}

	void DialogManager::SetContinueIndicatorImage(const char* path, Conversation& conversation)
	{
		Renderer2D& render = Wiwa::Application::Get().GetRenderer2D();

		ResourceId textID = Wiwa::Resources::Load<Wiwa::Image>(path);
		conversation.continueImg = Wiwa::Resources::GetResourceById<Wiwa::Image>(textID);

		conversation.continueImgID = render.CreateInstancedQuadTex(m_Scene, conversation.continueImg->GetTextureId(), conversation.continueImg->GetSize(), { 1600,800 }, { 50,50 }, Wiwa::Renderer2D::Pivot::UPLEFT);
		render.DisableInstance(m_Scene, conversation.continueImgID);
	}
	
}
