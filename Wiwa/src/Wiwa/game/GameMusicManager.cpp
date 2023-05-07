#include "wipch.h"
#include "GameMusicManager.h"
#include "Wiwa/audio/Audio.h"
#include "Wiwa/core/Input.h"
#include "Wiwa/core/KeyCodes.h"
#include "Wiwa/scene/SceneManager.h"
#include "Wiwa/core/ProjectManager.h"

namespace Wiwa
{
	uint64_t GameMusicManager::m_PlayEventId;
	bool GameMusicManager::m_Playing = false;

	void GameMusicManager::Init()
	{
		Logo();	

		Audio::PostEventDefaultListener("play_music", GameMusicManager::m_PlayEventId);
		m_Playing = true;
	}
	void GameMusicManager::UpdateCombatIntesity(int n_enemies)
	{
		Audio::SetRTPCValue("MusicIntensity", n_enemies);
	}
	void GameMusicManager::StopMusic()
	{
		Audio::SetState("MusicState", "None");
		Audio::StopEvent("play_music");
	}
	void GameMusicManager::Logo()
	{
		Audio::SetState("MenuScene", "Logo");
		Audio::SetState("MusicState", "Menus");
	}
	void GameMusicManager::Intro()
	{
		Audio::SetState("MenuScene", "Intro");
		Audio::SetState("MusicState", "Menus");
	}
	void GameMusicManager::Outro()
	{
		Audio::SetState("MenuScene", "Outro");
		Audio::SetState("MusicState", "Menus");
	}
	void GameMusicManager::MainMenu()
	{
		Audio::SetState("MenuScene", "MainMenu");
		Audio::SetState("MusicState", "Menus");
	}
	void GameMusicManager::GameOver()
	{
		Audio::SetState("MenuScene", "GameOver");
		Audio::SetState("MusicState", "Menus");
	}
	void GameMusicManager::MainHub()
	{
		Audio::SetState("RoomType", "Hub");
		Audio::SetState("MusicState", "GamePlay");
	}

	void GameMusicManager::Combat()
	{
		Audio::SetState("CombatIntensity", "Low");
		Audio::SetState("RoomType", "Combat");
		Audio::SetState("MusicState", "GamePlay");
	}
	void GameMusicManager::Rewad()
	{
		Audio::SetState("RoomType", "Reward");
		Audio::SetState("MusicState", "GamePlay");
	}
	void GameMusicManager::Shop()
	{
		Audio::SetState("RoomType", "Shop");
		Audio::SetState("MusicState", "GamePlay");
	}
	void GameMusicManager::OnSceneChage(const char* scene_path)
	{
		uint32_t id = Wiwa::ProjectManager::getSceneIndexByPath(scene_path);

		switch (id)
		{
		case 0:
			Logo();
			break;
		case 1: 
			Intro();
			break;
		case 2:
			MainMenu();
			break;
		case 3:
		//	MainHub();
			break;

		default:
			Combat();
			break;
		}
		

		if (!m_Playing)
		{
			Audio::PostEventDefaultListener("play_music", GameMusicManager::m_PlayEventId);
			m_Playing = true;
		}
	}
	
}

