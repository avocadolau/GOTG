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
		Conversation() {};

		// All nodes
		std::vector<DialogNode> nodes;

		// Active node
		DialogNode* activeNode;

		// Active buttons
		//std::vector<std::uint32_t> texts_imgMode;
		//std::vector<GuiButton> buttons;  //To delete

		//GuiButton continueButton;
		//GuiButton finishButton;

		// Dialog position
		int posX, posY;
		int max_chars_line;

		// Text offsetting
		int char_width;
		int char_height;
		int textXOffset, textYOffset;

		// Dialog fong
		//int dialogFont;

		// Dialog bg image and continue button
		uint32_t dialogImgID;
		uint32_t continueImgID;
		int dialogWidth, dialogHeight;

		// Whether the dialog has finished or not
		bool finished;
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

		Text* InitFont(const char* path,char* word, Conversation conversation);

		// Set a node ID as active
		void SetActiveNode(size_t id, Conversation conversation);

		// Return if the dialog has finished
		bool Finished(Conversation conversation);

		// Set position of dialog
		void SetPosition(int x, int y, Conversation conversation);

		// Set the dialog font
		void SetFont(int font, Conversation conversation);

		// Set the dialog background image
		void SetDialogBg(Image* dialog_bg, int width, int height, Conversation conversation);

	public:

		Conversation actualConversation;
		std::vector<Conversation> conversations;

		int actualConversationState = 0; // 0: Not conversating; 1: Is conversating; 2: Has just finished conversating;
	};
}