#include "wipch.h"
#include "GameMusicManager.h"
#include "Wiwa/audio/Audio.h"
#include "Wiwa/core/Input.h"
#include "Wiwa/core/KeyCodes.h"

namespace Wiwa
{
	void GameMusicManager::Update()
	{
	}
	void GameMusicManager::PlayMusic(const char* music_event_name)
	{
		Audio::StopAllEvents();
		Audio::PostEvent(music_event_name);
	}

	void GameMusicManager::OnSceneChage(size_t scene_id)
	{

		if (!Audio::LoadedProject())
		{
			WI_ERROR("Audio project not loaded");
			return;
		}

		switch (scene_id)
		{
		case 0:
			PlayMusic("music_intro");
			break;
		case 1:
			PlayMusic("music_main_menu");
			break;
		case 2:
			PlayMusic("music_hub");
			break;

		default: 
			PlayMusic("music_envirmoent");
			break;
		}

	}

}

