#include <wipch.h>
#include "RewardRoom.h"
#include <Wiwa/scene/SceneManager.h>

void Wiwa::RewardRoom::OnInit()
{
	uint32_t randomNum = RAND(0, 100);
	uint32_t counter = 0;
	Transform3D* t3d = SceneManager::getActiveScene()->GetEntityManager().GetComponent<Transform3D>(m_EntityId);
	if (IS_DROP_RATE(randomNum, counter, GameStateManager::s_ActiveSkillChances))
	{
		GameStateManager::SpawnRandomItem(t3d->position, 0);
	}
	counter += GameStateManager::s_ActiveSkillChances;
	if (IS_DROP_RATE(randomNum, counter, GameStateManager::s_PassiveSkillChances))
	{
		GameStateManager::SpawnRandomItem(t3d->position, 1);
	}
	counter += GameStateManager::s_PassiveSkillChances;
	if (IS_DROP_RATE(randomNum, counter, GameStateManager::s_BuffChances))
	{
		GameStateManager::SpawnRandomItem(t3d->position, 2);
	}
	counter += GameStateManager::s_BuffChances;
	if (IS_DROP_RATE(randomNum, counter, GameStateManager::s_NPCRoomChances))
	{
		GameStateManager::SpawnRandomItem(t3d->position, 1);
	}
	counter += GameStateManager::s_NPCRoomChances;
}
