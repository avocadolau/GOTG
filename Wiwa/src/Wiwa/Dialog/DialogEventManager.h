#pragma once

#include <Wiwa/core/Core.h>
#include <Wiwa/core/Renderer2D.h>
#include <Wiwa/utilities/render/Text.h>
#include <vector>
#include <string>
#include <Wiwa/core/Input.h>

#define MAX_CONVERSATIONEVENTS 25

namespace Wiwa
{
	struct ConversationEvent
	{
		std::string eventName;
	};

	class WI_API DialogEventManager
	{
	public:
		Scene* m_Scene;
	public:

		// Constructor
		DialogEventManager();

		// Destructor
		~DialogEventManager();

	
		// Init gui manager
		bool Init(Scene* scene);

		// Update gui manager
		bool Update();

		bool Draw();

		void OnDestroy();

		// Called before quitting
		bool CleanUp();

	public:

		Wiwa::Renderer2D::InstanceData choice2Opt1ImgID;
		Wiwa::Renderer2D::InstanceData choice2Opt2ImgID;
		Wiwa::Renderer2D::InstanceData choice2Opt3ImgID;

		int selector = 0;

		bool eventStarted = false;
		bool eventFinished = false;

		int eventState = 2; // 0 = started event, 1 = running event, 2 = finished event

		int keyPressTimer = 0;
	};
}