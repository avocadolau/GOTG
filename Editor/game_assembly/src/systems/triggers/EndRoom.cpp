#include "EndRoom.h"

Wiwa::EndRoomTrigger::EndRoomTrigger()
{
}

Wiwa::EndRoomTrigger::~EndRoomTrigger()
{
}

void Wiwa::EndRoomTrigger::OnDestroy()
{
	GameStateManager::setPlayerTriggerNextRoom(false);
}

void Wiwa::EndRoomTrigger::OnCollisionEnter(Object* body1, Object* body2)
{
	std::string playerBulletStr = "PLAYER";
	if (body1->id == m_EntityId && playerBulletStr == body2->selfTagStr)
	{
		GameStateManager::setPlayerTriggerNextRoom(true);
	}
}

void Wiwa::EndRoomTrigger::OnCollision(Object* body1, Object* body2)
{
	/*std::string playerBulletStr = "PLAYER";
	if (body1->id == m_EntityId && playerBulletStr == body2->selfTagStr)
	{
		GameStateManager::setPlayerTriggerNextRoom(true);
	}*/
}

void Wiwa::EndRoomTrigger::OnCollisionExit(Object* body1, Object* body2)
{
	std::string playerBulletStr = "PLAYER";
	if (body1->id == m_EntityId && playerBulletStr == body2->selfTagStr)
	{
		GameStateManager::setPlayerTriggerNextRoom(false);
	}
}
