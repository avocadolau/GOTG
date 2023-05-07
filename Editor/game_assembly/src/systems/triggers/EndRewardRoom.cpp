#include "EndRewardRoom.h"
#include "../../components/trigger/RewardRoomData.h"

void Wiwa::EndRewardRoom::OnAwake()
{
}
void Wiwa::EndRewardRoom::OnInit()
{
	m_DataIt = GetComponentIterator<RewardRoomData>();
	RewardRoomData* data = GetComponentByIterator<RewardRoomData>(m_DataIt);
	if (data)
	{
		if (data->isLeft)
		{
			data->num = GameStateManager::s_DoorsReward[0];
		}
		else
		{
			data->num = GameStateManager::s_DoorsReward[1];
		}
	}
}
void Wiwa::EndRewardRoom::OnUpdate()
{
}
void Wiwa::EndRewardRoom::OnCollisionEnter(Object* body1, Object* body2)
{
	std::string playerBulletStr = "PLAYER";
	if (body1->id == m_EntityId && playerBulletStr == body2->selfTagStr)
	{
		RewardRoomData* data = GetComponentByIterator<RewardRoomData>(m_DataIt);
		GameStateManager::setPlayerTriggerNextRoom(true);
		GameStateManager::s_NextRewardRoomReward = data->num;
	}
}

void Wiwa::EndRewardRoom::OnCollision(Object* body1, Object* body2)
{
	std::string playerBulletStr = "PLAYER";
	if (body1->id == m_EntityId && playerBulletStr == body2->selfTagStr)
	{
		RewardRoomData* data = GetComponentByIterator<RewardRoomData>(m_DataIt);
		GameStateManager::setPlayerTriggerNextRoom(true);
		GameStateManager::s_NextRewardRoomReward = data->num;
	}
}

void Wiwa::EndRewardRoom::OnCollisionExit(Object* body1, Object* body2)
{
	std::string playerBulletStr = "PLAYER";
	if (body1->id == m_EntityId && playerBulletStr == body2->selfTagStr)
	{
		RewardRoomData* data = GetComponentByIterator<RewardRoomData>(m_DataIt);
		GameStateManager::setPlayerTriggerNextRoom(false);
		GameStateManager::s_NextRewardRoomReward = data->num;
	}
}