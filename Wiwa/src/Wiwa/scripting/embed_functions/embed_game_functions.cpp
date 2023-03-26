#include <wipch.h>
#include "embed_game_functions.h"

void SetRoomStateTriggerNext(bool value)
{
	Wiwa::GameStateManager::setPlayerTriggerNextRoom(value);
}

void InitHub()
{
	Wiwa::GameStateManager::InitHub();
}

void StartRun()
{
	Wiwa::GameStateManager::StartRun();
}

void EndRun()
{
	Wiwa::GameStateManager::EndRun();
}

void SetPlayer(size_t id, void* scene)
{
	Wiwa::Scene* _scene = (Wiwa::Scene*)scene;
	Wiwa::GameStateManager::SetPlayerId(id, _scene);
}

void SavePlayerProgression()
{
	Wiwa::GameStateManager::SaveProgression();
}

void LoadPlayerProgression()
{
	Wiwa::GameStateManager::LoadProgression();
}

float GetControllerDeadzone()
{
	return Wiwa::GameStateManager::s_GamepadDeadzone;
}

void Die()
{
	Wiwa::GameStateManager::Die();
}
