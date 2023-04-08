#pragma once

#include <Wiwa/core/Core.h>
#include <Wiwa/core/Renderer2D.h>
#include <Wiwa/utilities/render/Text.h>
#include <vector>
#include <string>
#include <Wiwa/core/Input.h>

#include "DialogNode.h"
//#include "Wiwa/Ui/UiButton.h"

namespace Wiwa
{
	struct Conversation
	{
		// Constructor
		//Conversation() {};

		// All nodes
		std::vector<DialogNode*> nodes;

		// Dialog bg image and continue button
		Wiwa::Renderer2D::InstanceData dialogImgID;
		Wiwa::Renderer2D::InstanceData characterImgID;
		Wiwa::Renderer2D::InstanceData continueImgID;

		Image* dialogImg = nullptr;
		Image* characterImg = nullptr;
		Image* continueImg = nullptr;

		char* conversationName;
		//int dialogWidth, dialogHeight;

		// Whether the dialog has finished or not
		//bool finished;

		// Active node
		//DialogNode* activeNode;

		// Active buttons
		//std::vector<std::uint32_t> texts_imgMode;
		//std::vector<GuiButton> buttons;  //To delete

		//GuiButton continueButton;
		//GuiButton finishButton;

		// Dialog position
		//int posX, posY;
		//int max_chars_line;

		// Text offsetting
		//int char_width;
		//int char_height;
		//int textXOffset, textYOffset;

		// Dialog fong
		//int dialogFont;
	};

	class WI_API DialogManager
	{
	private:
		Scene* m_Scene;
	public:

		// Constructor
		DialogManager();

		// Destructor
		virtual ~DialogManager();

	
		// Init gui manager
		bool Init(Scene* scene);

		// Update gui manager
		bool Update();

		bool Draw();

		// Called before quitting
		bool CleanUp();

		void UpdateConversation(Conversation conversation, Renderer2D* render);

		// Set the dialog font and texts
		void SetDialogText(char* line1Text, char* line2Text, char* line3Text, const char* fontPath, Conversation& conversation);

		// Set the dialog background images
		void SetDialogBubbleImage(const char* path, Conversation& conversation);
		void SetCharacterImage(const char* path, Conversation& conversation);
		void SetContinueIndicatorImage(const char* path, Conversation& conversation);
		
		//void SetDialogBg(Image* dialog_bg, int width, int height, Conversation conversation);

		//Text* InitFont(const char* path,char* word, Conversation conversation);

		// Set a node ID as active
		//void SetActiveNode(size_t id, Conversation conversation);

		// Return if the dialog has finished
		//bool Finished(Conversation conversation);

		// Set position of dialog
		//void SetPosition(int x, int y, Conversation conversation);

	public:

		char* conversationToPlayName;
		std::vector<Conversation*> conversations;

		int actualConversationState = 0; // 0: Not conversating; 1: Is conversating; 2: Has just finished conversating;
		int currentNode = 0;

		int keyPressRefreshTimer = 0;

		Conversation* conversationCreator_P;
	};
}