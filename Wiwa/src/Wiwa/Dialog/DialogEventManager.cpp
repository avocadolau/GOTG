#include <wipch.h>

#include "DialogEventManager.h"

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