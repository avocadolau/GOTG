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
}

