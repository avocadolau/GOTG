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
