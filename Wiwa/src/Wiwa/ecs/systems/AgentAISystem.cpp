#include <wipch.h>

#include "AgentAISystem.h"
#include "Wiwa/ecs/components/AgentAI.h"
#include <Wiwa/AI/AIPathFindingManager.h>
//#include "Wiwa/ecs/components/Transform3D.h"

Wiwa::AgentAISystem::AgentAISystem()
{
}

Wiwa::AgentAISystem::~AgentAISystem()
{
}

void Wiwa::AgentAISystem::OnAwake()
{
	m_AgentAI = GetComponentIterator<Wiwa::AgentAI>();
	m_Transform = GetComponentIterator<Wiwa::Transform3D>();
}

void Wiwa::AgentAISystem::OnInit()
{
}

void Wiwa::AgentAISystem::OnUpdate()
{
	

	m_DirectionPoint = Wiwa::AIPathFindingManager::MapToWorld(m_DirectionPoint.x, m_DirectionPoint.y);

	Wiwa::AIPathFindingManager::MapData& localMapData = Wiwa::AIPathFindingManager::GetMapData();
	m_DirectionPoint = { m_DirectionPoint.x + (localMapData.tileWidth*0.5f), m_DirectionPoint.y + (localMapData.tileHeight * 0.5f) };

	Wiwa::AgentAI* agent = GetComponentByIterator<Wiwa::AgentAI>(m_AgentAI);

	if (m_DirectionPoint.x + (localMapData.tileWidth * 0.5f) < agent->target.x) // target position
	{

	}
	else if (m_DirectionPoint.x + (localMapData.tileWidth * 0.5f) > agent->target.x)
	{

	}

	if (m_DirectionPoint.y + (localMapData.tileHeight * 0.5f) < agent->target.z) // target position
	{

	}
	else if (m_DirectionPoint.y + (localMapData.tileHeight * 0.5f) > agent->target.z)
	{

	}

}

void Wiwa::AgentAISystem::OnDestroy()
{
}

void Wiwa::AgentAISystem::GoToPosition(glm::vec3 targetPos)
{
	Wiwa::AgentAI* agent = GetComponentByIterator<Wiwa::AgentAI>(m_AgentAI);
	Wiwa::Transform3D* transform = GetComponentByIterator<Wiwa::Transform3D>(m_Transform);

	// transform->position;
	
	m_DirectionPoint = Wiwa::AIPathFindingManager::WorldToMap(transform->position.x, transform->position.z);
	

	glm::ivec2 target = {targetPos.x, targetPos.z};
	Wiwa::AIPathFindingManager::CreatePath(m_DirectionPoint,target);

	glm::ivec2 nextPos;

	const std::vector<glm::ivec2>* lastPath = Wiwa::AIPathFindingManager::GetLastPath();
	
	if (lastPath->size() > 1)
	{
		nextPos = { lastPath->at(1).x, lastPath->at(1).y };
	}

	m_DirectionPoint = nextPos;

	
}
