#include "wipch.h"
#include "embed_artificialintelligence_functions.h"
#include "Wiwa/ecs/systems/AgentAISystem.h"


void SendAIToPosition(size_t entity_id,glm::vec3 target_pos)
{
	Wiwa::EntityManager& em = Wiwa::SceneManager::getActiveScene()->GetEntityManager();

	Wiwa::AgentAISystem* agentPtr = em.GetSystem<Wiwa::AgentAISystem>(entity_id);

	if (agentPtr)
	{
		agentPtr->CreatePath(target_pos);
	}
}
