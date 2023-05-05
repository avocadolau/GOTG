#include "wipch.h"
#include "GameMusicManager.h"
#include "Wiwa/audio/Audio.h"
#include "Wiwa/core/Input.h"
#include "Wiwa/core/KeyCodes.h"

namespace Wiwa
{
	void GameMusicManager::OnSceneChage(size_t scene_id)
	{

		if (!Audio::LoadedProject())
		{
			WI_ERROR("Audio project not loaded");
			return;
		}
	

		Audio::ChangeStateContainer("music_states", "menus");
		Audio::ChangeStateContainer("menu_scene", "main_menu");
		Audio::PostEvent("play_music");
		
		switch (scene_id)
		{
		case 0:
			Audio::PostEvent("play_music");
			Audio::ChangeStateContainer("music_states", "menus");
			Audio::ChangeStateContainer("menu_scene", "logo");

			break;
		
		 case 1:	
			Audio::PostEvent("play_music");
		    Audio::ChangeStateContainer("music_states", "menus");
			Audio::ChangeStateContainer("menu_scene", "intro");
			break;
		case 2:
			Audio::ChangeStateContainer("music_states", "menus");
			Audio::ChangeStateContainer("menu_scene", "main_menu");
			break;
		default: 
			Audio::PostEvent("play_music");
			Audio::ChangeStateContainer("music_states", "game_play");
			Audio::ChangeStateContainer("music_game_play", "test");
			break;
		}

	}

}

