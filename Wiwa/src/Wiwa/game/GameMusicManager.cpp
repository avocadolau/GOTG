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
			Audio::ChangeSwitchContainer("music", "logo");
			break;
		case 1:
			Audio::ChangeSwitchContainer("music", "intro");
			break;
		case 2:
			Audio::ChangeSwitchContainer("music", "hub");
			break;
		default: 
			Audio::ChangeSwitchContainer("music_game_play", "test");
			break;
		}

	}

}

