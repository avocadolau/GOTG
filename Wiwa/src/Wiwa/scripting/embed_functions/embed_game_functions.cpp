#include <wipch.h>
#include "embed_game_functions.h"

void SetRoomStateTriggerNext(bool value)
{
	Wiwa::GameStateManager::setPlayerTriggerNextRoom(value);
}
