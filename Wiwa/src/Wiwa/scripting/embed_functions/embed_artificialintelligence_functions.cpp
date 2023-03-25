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

void SendAIToPositionById(size_t entity_id, size_t target_id)
{
	Wiwa::EntityManager& em = Wiwa::SceneManager::getActiveScene()->GetEntityManager();

	Wiwa::AgentAISystem* agentPtr = em.GetSystem<Wiwa::AgentAISystem>(entity_id);

	if (agentPtr)
	{
		agentPtr->CreatePath(em.GetComponent<Wiwa::Transform3D>(target_id)->localPosition);
	}
}

float DistanceAgentTarget(size_t entity_id, glm::vec3 target_pos)
{
	Wiwa::EntityManager& em = Wiwa::SceneManager::getActiveScene()->GetEntityManager();

	return glm::distance(em.GetComponent<Wiwa::Transform3D>(entity_id)->localPosition, target_pos);
}

float DistanceAgentTargetById(size_t entity_id, size_t target_id)
{
	Wiwa::EntityManager& em = Wiwa::SceneManager::getActiveScene()->GetEntityManager();

	return glm::distance(em.GetComponent<Wiwa::Transform3D>(entity_id)->localPosition, em.GetComponent<Wiwa::Transform3D>(target_id)->localPosition);
}

void StopMoving(size_t entity_id)
{
	Wiwa::EntityManager& em = Wiwa::SceneManager::getActiveScene()->GetEntityManager();

	Wiwa::AgentAISystem* agentPtr = em.GetSystem<Wiwa::AgentAISystem>(entity_id);

	agentPtr->StopMoving();
}

bool HasPath(size_t entity_id)
{
	Wiwa::EntityManager& em = Wiwa::SceneManager::getActiveScene()->GetEntityManager();

	Wiwa::AgentAISystem* agentPtr = em.GetSystem<Wiwa::AgentAISystem>(entity_id);

	return agentPtr->HasPath();	
}
