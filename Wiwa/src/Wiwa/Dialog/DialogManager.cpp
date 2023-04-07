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

		/*Conversation* newConversation = new Conversation();

		SetDialogBubbleImage("assets/HUD_Images/dialog_images/dialog_test_placeholder.png", *newConversation);
		SetContinueIndicatorImage("assets/HUD_Images/dialog_images/dialog_test_placeholder2.png", *newConversation);

		SetDialogText("This is a test dialog in line 1", "assets/Fonts/BADABB__.TTF", *newConversation, 1);
		SetDialogText("This is a test dialog in line 2", "assets/Fonts/BADABB__.TTF", *newConversation, 2);
		SetDialogText("This is a test dialog in line 4", "assets/Fonts/BADABB__.TTF", *newConversation, 4);

		SetDialogText("Now we are in another node in line 2", "assets/Fonts/BADABB__.TTF", *newConversation, 2);
		SetDialogText("Now we are in another node in line 5", "assets/Fonts/BADABB__.TTF", *newConversation, 5);

		conversations.push_back(newConversation);*/

		return true;
	}


	bool DialogManager::Update()  // Continue: mando Y, teclado Space - In total, two custom images: character and bubble - one fix image: continue sign
	{
		//if ((Wiwa::Input::IsKeyPressed(Wiwa::Key::Space) || Wiwa::Input::IsButtonPressed(0, 3)) && actualConversationState != 1) // Just for testing purposes, actual game logic goes here before UpdateConversation();
		//{
		//	//actualConversation = conversations[0];
		//	actualConversationState = 0;
		//}
		//
		//if (actualConversationState == 0 || actualConversationState == 1)
		//{
		//	UpdateConversation(*conversations[0], &Wiwa::Application::Get().GetRenderer2D());
		//}
		
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
		int currentNode = 0;
		if(actualConversationState == 0) actualConversationState = 1;

		if (actualConversationState == 1)
		{
			//render->UpdateInstancedQuadTexPosition(m_Scene, conversation.dialogImgID, { 0,0 }, Wiwa::Renderer2D::Pivot::UPLEFT); // Render character and bubble
			//render->UpdateInstancedQuadTexPosition(m_Scene, conversation.nodes[currentNode].text1_imgModeID, { 500,560 }, Wiwa::Renderer2D::Pivot::UPLEFT); // Render text line 1
			//render->UpdateInstancedQuadTexPosition(m_Scene, conversation.nodes[currentNode].text2_imgModeID, { 500,560 }, Wiwa::Renderer2D::Pivot::UPLEFT); // Render text line 2
			//render->UpdateInstancedQuadTexPosition(m_Scene, conversation.nodes[currentNode].text3_imgModeID, { 500,560 }, Wiwa::Renderer2D::Pivot::UPLEFT); // Render text line 3
			//render->UpdateInstancedQuadTexPosition(m_Scene, conversation.nodes[currentNode].text4_imgModeID, { 500,560 }, Wiwa::Renderer2D::Pivot::UPLEFT); // Render text line 4
			//render->UpdateInstancedQuadTexPosition(m_Scene, conversation.nodes[currentNode].text5_imgModeID, { 500,560 }, Wiwa::Renderer2D::Pivot::UPLEFT); // Render text line 5

			//render->UpdateInstancedQuadTexPosition(m_Scene, conversation.continueImgID, { 650,650 }, Wiwa::Renderer2D::Pivot::UPLEFT); // Render continue button

			render->EnableInstance(m_Scene, conversation.dialogImgID);

			render->EnableInstance(m_Scene, conversation.nodes[currentNode]->text1_imgModeID);
			render->EnableInstance(m_Scene, conversation.nodes[currentNode]->text2_imgModeID);
			render->EnableInstance(m_Scene, conversation.nodes[currentNode]->text3_imgModeID);
			render->EnableInstance(m_Scene, conversation.nodes[currentNode]->text4_imgModeID);
			render->EnableInstance(m_Scene, conversation.nodes[currentNode]->text5_imgModeID);

			render->EnableInstance(m_Scene, conversation.continueImgID);

			if ((Wiwa::Input::IsKeyPressed(Wiwa::Key::Space) || Wiwa::Input::IsButtonPressed(0, 3)))
			{
				render->DisableInstance(m_Scene, conversation.nodes[currentNode]->text1_imgModeID);
				render->DisableInstance(m_Scene, conversation.nodes[currentNode]->text2_imgModeID);
				render->DisableInstance(m_Scene, conversation.nodes[currentNode]->text3_imgModeID);
				render->DisableInstance(m_Scene, conversation.nodes[currentNode]->text4_imgModeID);
				render->DisableInstance(m_Scene, conversation.nodes[currentNode]->text5_imgModeID);

				currentNode++;
				if (currentNode > conversation.nodes.size())
				{
					actualConversationState = 2;
				}
			}
		}
	}

	void DialogManager::SetDialogText(char* dialogText, const char* fontPath, Conversation& conversation, int textLine)
	{
		DialogNode* newDialogNode = new DialogNode();

		Wiwa::GuiManager& gm = m_Scene->GetGuiManager();

		newDialogNode->text1_imgMode = new Text();
		newDialogNode->text2_imgMode = new Text();
		newDialogNode->text3_imgMode = new Text();
		newDialogNode->text4_imgMode = new Text();
		newDialogNode->text5_imgMode = new Text();

		newDialogNode->text1_imgMode = gm.InitFont(fontPath, dialogText);
		newDialogNode->text2_imgMode = gm.InitFont(fontPath, dialogText);
		newDialogNode->text3_imgMode = gm.InitFont(fontPath, dialogText);
		newDialogNode->text4_imgMode = gm.InitFont(fontPath, dialogText);
		newDialogNode->text5_imgMode = gm.InitFont(fontPath, dialogText);

		Renderer2D& render = Wiwa::Application::Get().GetRenderer2D();

		if (textLine == 1)
		{
			newDialogNode->text1_imgModeID = render.CreateInstancedQuadTex(m_Scene, newDialogNode->text1_imgMode->GetTextureId(), newDialogNode->text1_imgMode->GetSize(), { 500,500 }, { 650,45 }, Wiwa::Renderer2D::Pivot::UPLEFT);
			render.DisableInstance(m_Scene, newDialogNode->text1_imgModeID);
		}
		if (textLine == 2)
		{
			newDialogNode->text2_imgModeID = render.CreateInstancedQuadTex(m_Scene, newDialogNode->text2_imgMode->GetTextureId(), newDialogNode->text2_imgMode->GetSize(), { 500,550 }, { 650,45 }, Wiwa::Renderer2D::Pivot::UPLEFT);
			render.DisableInstance(m_Scene, newDialogNode->text2_imgModeID);
		}
		if (textLine == 3)
		{
			newDialogNode->text3_imgModeID = render.CreateInstancedQuadTex(m_Scene, newDialogNode->text3_imgMode->GetTextureId(), newDialogNode->text3_imgMode->GetSize(), { 500,600 }, { 650,45 }, Wiwa::Renderer2D::Pivot::UPLEFT);
			render.DisableInstance(m_Scene, newDialogNode->text3_imgModeID);
		}
		if (textLine == 4)
		{
			newDialogNode->text4_imgModeID = render.CreateInstancedQuadTex(m_Scene, newDialogNode->text4_imgMode->GetTextureId(), newDialogNode->text4_imgMode->GetSize(), { 500,650 }, { 650,45 }, Wiwa::Renderer2D::Pivot::UPLEFT);
			render.DisableInstance(m_Scene, newDialogNode->text4_imgModeID);
		}
		if (textLine == 5)
		{
			newDialogNode->text5_imgModeID = render.CreateInstancedQuadTex(m_Scene, newDialogNode->text5_imgMode->GetTextureId(), newDialogNode->text5_imgMode->GetSize(), { 500,700 }, { 650,45 }, Wiwa::Renderer2D::Pivot::UPLEFT);
			render.DisableInstance(m_Scene, newDialogNode->text5_imgModeID);
		}

		conversation.nodes.push_back(newDialogNode);
	}

	void DialogManager::SetDialogBubbleImage(const char* path, Conversation& conversation)
	{
		Renderer2D& render = Wiwa::Application::Get().GetRenderer2D();

		ResourceId textID = Wiwa::Resources::Load<Wiwa::Image>(path);
		conversation.dialogImg = Wiwa::Resources::GetResourceById<Wiwa::Image>(textID);

		conversation.continueImg = new Image;

		conversation.dialogImgID = render.CreateInstancedQuadTex(m_Scene, conversation.continueImg->GetTextureId(), conversation.continueImg->GetSize(), { 420,0 }, { 1080,1080 }, Wiwa::Renderer2D::Pivot::UPLEFT);
		render.DisableInstance(m_Scene, conversation.dialogImgID);
	}

	void DialogManager::SetContinueIndicatorImage(const char* path, Conversation& conversation)
	{
		Renderer2D& render = Wiwa::Application::Get().GetRenderer2D();

		ResourceId textID = Wiwa::Resources::Load<Wiwa::Image>(path);
		conversation.continueImg = Wiwa::Resources::GetResourceById<Wiwa::Image>(textID);

		conversation.continueImgID = render.CreateInstancedQuadTex(m_Scene, conversation.continueImg->GetTextureId(), conversation.continueImg->GetSize(), { 1020,1020 }, { 50,50 }, Wiwa::Renderer2D::Pivot::UPLEFT);
		render.DisableInstance(m_Scene, conversation.continueImgID);
	}

	//Text* DialogManager::InitFont(const char* path, char* _word, Conversation conversation)
	//{
	//
	//	/* load font file */
	//	long size;
	//	byte* fontBuffer;
	//
	//	//Extracted from https://github.com/justinmeiners/stb-truetype-example/blob/master/main.c
	//	//Read the fontFile lines 19 to 27
	//	FILE* fontFile = fopen(path, "rb");
	//	if (!fontFile)
	//	{
	//		WI_CORE_INFO("Seems like font at {} doesn't exist", path);
	//		return false;
	//	}
	//	fseek(fontFile, 0, SEEK_END);
	//	size = ftell(fontFile);
	//	fseek(fontFile, 0, SEEK_SET);
	//
	//	fontBuffer = (byte*)malloc(size);
	//
	//	fread(fontBuffer, size, 1, fontFile);
	//	fclose(fontFile);
	//
	//
	//	/* prepare font */
	//	stbtt_fontinfo info;
	//	if (!stbtt_InitFont(&info, fontBuffer, 0))
	//	{
	//		WI_CORE_ERROR("Failed to load font at {}", path);
	//		return false;
	//	}
	//
	//	int b_w = 512; /* bitmap width */
	//	int b_h = 512; /* bitmap height */
	//	int l_h = 64; /* line height */
	//
	//	/* create a bitmap for the phrase */
	//	byte* bitmap = (byte*)calloc(b_w * b_h, sizeof(unsigned char));
	//
	//	/* calculate font scaling */
	//	float scale = stbtt_ScaleForPixelHeight(&info, (float)l_h);
	//
	//	char* word = _word;
	//
	//	int x = 0;
	//
	//	int ascent, descent, lineGap;
	//	stbtt_GetFontVMetrics(&info, &ascent, &descent, &lineGap);
	//
	//	ascent = (int)round(ascent * scale);
	//	descent = (int)round(descent * scale);
	//
	//	int i;
	//	for (i = 0; i < strlen(word); ++i)
	//	{
	//		/* how wide is this character */
	//		int ax;
	//		int lsb;
	//		stbtt_GetCodepointHMetrics(&info, word[i], &ax, &lsb);
	//		/* (Note that each Codepoint call has an alternative Glyph version which caches the work required to lookup the character word[i].) */
	//
	//		/* get bounding box for character (may be offset to account for chars that dip above or below the line) */
	//		int c_x1, c_y1, c_x2, c_y2;
	//		stbtt_GetCodepointBitmapBox(&info, word[i], scale, scale, &c_x1, &c_y1, &c_x2, &c_y2);
	//
	//		/* compute y (different characters have different heights) */
	//		int y = ascent + c_y1;
	//
	//		/* render character (stride and offset is important here) */
	//		int byteOffset = (int)(x + roundf(lsb * scale) + (y * b_w));
	//		stbtt_MakeCodepointBitmap(&info, bitmap + byteOffset, c_x2 - c_x1, c_y2 - c_y1, b_w, scale, scale, word[i]);
	//
	//		/* advance x */
	//		x += (int)roundf(ax * scale);
	//
	//		/* add kerning */
	//		int kern;
	//		kern = stbtt_GetCodepointKernAdvance(&info, word[i], word[i + 1]);
	//		x += (int)roundf(kern * scale);
	//	}
	//
	//
	//	Text* text = new Text();
	//	text->Init(b_w, b_h, bitmap);
	//
	//	free(fontBuffer);
	//	free(bitmap);
	//
	//	return text;
	//}

	//void DialogManager::SetActiveNode(size_t id, Conversation conversation)
	//{
	//	if (id >= 0 && id < conversation.nodes.size())
	//	{
	//		conversation.activeNode = &conversation.nodes[id];
	//
	//		size_t opts = conversation.activeNode->options.size();
	//		SDL_Rect bounds =
	//		{
	//			conversation.posX,
	//			conversation.posY + conversation.dialogHeight,
	//			0,
	//			conversation.dialogHeight / 4
	//		};
	//
	//		Font& fontobj = app->fonts->GetFont(conversation.dialogFont);
	//
	//		conversation.buttons.clear();
	//		for (size_t i = 0; i < opts; i++)
	//		{
	//			bounds.w = fontobj.char_w * (conversation.activeNode->options[i].length() + 4);
	//			conversation.buttons.emplace_back(bounds, conversation.dialogImg, conversation.activeNode->options[i].c_str(), conversation.dialogFont);
	//			bounds.x += bounds.w;
	//		}
	//
	//		conversation.texts.clear();
	//		//SplitText(conversation.activeNode->text);
	//
	//		int lines = conversation.texts.size();
	//
	//		conversation.textXOffset = conversation.dialogWidth / 2 - (conversation.max_chars_line * conversation.char_width) / 2;
	//		conversation.textYOffset = conversation.dialogHeight / 2 - (lines * conversation.char_height) / 2;
	//
	//		if (lines > 1)
	//		{
	//			conversation.textYOffset -= ((lines - 1) * conversation.char_height / 2);
	//		}
	//	}
	//}
	//
	//bool DialogManager::Finished(Conversation conversation)
	//{
	//
	//
	//	return false;
	//}
	//
	//void DialogManager::SetPosition(int x, int y, Conversation conversation)
	//{
	//	conversation.posX = x;
	//	conversation.posY = y;
	//}
	//
	//void DialogManager::SetFont(int font, Conversation conversation)
	//{
	//	conversation.dialogFont = font;
	//
	//	Font& fontobj = app->fonts->GetFont(conversation.dialogFont);
	//
	//	SDL_Rect bounds =
	//	{
	//			conversation.posX,
	//			conversation.posY + conversation.dialogHeight,
	//			fontobj.char_w * (sizeof("continue") + 4),
	//			conversation.dialogHeight / 4
	//	};
	//
	//	//conversation.continueButton = Button(bounds, conversation.dialogImg, "continue", conversation.dialogFont);
	//
	//	bounds.w = fontobj.char_w * (sizeof("finish") + 4);
	//	//conversation.finishButton = Button(bounds, conversation.dialogImg, "finish", conversation.dialogFont);
	//
	//	conversation.char_width = fontobj.char_w;
	//	conversation.char_height = fontobj.char_h;
	//
	//	conversation.max_chars_line = conversation.dialogWidth / conversation.char_width - conversation.char_width * 4;
	//}
	//
	//void DialogManager::SetDialogBg(Image* dialog_bg, int width, int height, Conversation conversation)
	//{
	//	conversation.dialogImg = dialog_bg;
	//	conversation.dialogWidth = width;
	//	conversation.dialogHeight = height;
	//
	//	SetFont(conversation.dialogFont, conversation);
	//}
	
}
