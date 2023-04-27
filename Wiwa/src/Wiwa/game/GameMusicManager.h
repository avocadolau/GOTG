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
	public:
		void static Update();
		void static PlayMusic(const char* music_name);
	};

}
