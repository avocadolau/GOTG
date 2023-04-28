#include "StartRun.h"
#include "../../components/trigger/Trigger.h"
#include "EndRoom.h"

Wiwa::StartRunTrigger::StartRunTrigger()
{
    //triggerIt = { WI_INVALID_INDEX, WI_INVALID_INDEX };

}

Wiwa::StartRunTrigger::~StartRunTrigger()
{
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
        GameStateManager::StartRun();
    }
}
