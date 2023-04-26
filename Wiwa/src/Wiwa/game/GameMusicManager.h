#pragma once

#include <Wiwa/core/Core.h>
#include <Wiwa/audio/Audio.h>


namespace Wiwa {

	struct SceneMusic {
		const char* m_SongName;
		uint32_t scene;
	};

	class GameMusicManager
	{

		void static PlayMusic(const char* music_name);
	};

}
