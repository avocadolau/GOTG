#include "StartRun.h"
#include "../../components/trigger/Trigger.h"
#include "EndRoom.h"
#include "../../components/trigger/RewardRoomData.h"

Wiwa::StartRunTrigger::StartRunTrigger()
{
    //triggerIt = { WI_INVALID_INDEX, WI_INVALID_INDEX };

}

Wiwa::StartRunTrigger::~StartRunTrigger()
{
}

void Wiwa::StartRunTrigger::OnInit()
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

void Wiwa::StartRunTrigger::OnAwake()
{
    //triggerIt = GetComponentIterator<Trigger>();
}

void Wiwa::StartRunTrigger::OnCollisionEnter(Object* body1, Object* body2)
{
    WI_INFO("-----------OnCollision start room -----------");
    if (body1->id == m_EntityId)
    {
        WI_INFO("----------- Starting a new run -----------");
		RewardRoomData* data = GetComponentByIterator<RewardRoomData>(m_DataIt);
		GameStateManager::s_NextRewardRoomReward = data->num;
        GameStateManager::StartRun();
    }
}
