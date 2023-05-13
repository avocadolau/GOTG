#include <wipch.h>

#include "DialogEventManager.h"
#include "DialogManager.h"

#include <Wiwa/ecs/components/game/Character.h>

#include "Wiwa/audio/Audio.h"

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
	DialogEventManager::DialogEventManager()
	{

	}

	DialogEventManager::~DialogEventManager()
	{

	}

	bool DialogEventManager::Init(Scene* scene)
	{
		m_Scene = scene;


		return true;
	}

	bool DialogEventManager::Update()
	{
		//DialogManager* dm = Wiwa::Application::Get();
		Wiwa::Scene * _scene = (Wiwa::Scene*)m_Scene;


		if (_scene->GetDialogManager().triggerEvent == true)
		{
			if (!strcmp(_scene->GetDialogManager().dialogEventToTrigger.c_str(), "Choice_Uatu"))
			{
				
			}
			else if (!strcmp(_scene->GetDialogManager().dialogEventToTrigger.c_str(), "Choice_Aron"))
			{

			}
			else if (!strcmp(_scene->GetDialogManager().dialogEventToTrigger.c_str(), "Choice_Ulana"))
			{

			}
		}

		return true;
	}

	bool DialogEventManager::Draw()
	{


		return false;
	}

	void DialogEventManager::OnDestroy()
	{

	}

	bool DialogEventManager::CleanUp()
	{


		return true;
	}
}