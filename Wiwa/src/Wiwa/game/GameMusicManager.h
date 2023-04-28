#pragma once

#include <Wiwa/core/Core.h>
#include <Wiwa/audio/Audio.h>
namespace Wiwa {

	class GameMusicManager
	{
	public:

		std::vector<std::string> m_MusicBanks;

		void static Update();
		void static PlayMusic(const char* music_event_name);

		void static OnSceneChage(size_t scene_id);
	};

}
