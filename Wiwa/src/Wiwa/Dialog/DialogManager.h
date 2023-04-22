#pragma once

#include <Wiwa/core/Core.h>
#include <Wiwa/core/Renderer2D.h>
#include <Wiwa/utilities/render/Text.h>
#include <vector>
#include <string>
#include <Wiwa/core/Input.h>

#include "DialogNode.h"

#define MAX_CONVERSATIONS 25
#define MAX_CONVERSATION_NODES 25

namespace Wiwa
{
	struct Conversation
	{

		// All nodes
		DialogNode nodes[MAX_CONVERSATION_NODES];

		// Dialog bg image, character and continue button
		Wiwa::Renderer2D::InstanceData dialogImgID;
		Wiwa::Renderer2D::InstanceData characterImgID;

		std::string conversationName;

		std::string bubbleImagePath;
		std::string characterImagePath;

		bool occupied = false;
	};

	class WI_API DialogManager
	{
	public:
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

		void OnDestroy();

		// Called before quitting
		bool CleanUp();

		void UpdateConversation(int conversationNumber, Renderer2D* render);

		// Set the dialog font and texts
		void SetDialogText(char* line1Text, char* line2Text, char* line3Text, const char* fontPath, int conversationNumber, int nodeNumber);

		// Set the dialog background images
		void SetDialogBubbleImage(const char* path, int conversationNumber);
		void SetCharacterImage(const char* path, int conversationNumber);
		//void SetContinueIndicatorImage(const char* path, int conversationNumber);

		void SaveAllDialogs();
		void LoadAllDialogs();

	public:

		std::string conversationToPlayName;
		Conversation conversations[MAX_CONVERSATIONS];
		Conversation editorConversations[MAX_CONVERSATIONS];

		int actualConversationState = 0; // 0: Not conversating; 1: Is conversating; 2: Has just finished conversating;
		int currentNode = 0;

		int keyPressRefreshTimer = 0;

		//Conversation* conversationCreator_P;

		bool collidingWithNpc = false;
		std::string NpcConversationTag;

		Wiwa::Renderer2D::InstanceData continueImgID;
	};
}