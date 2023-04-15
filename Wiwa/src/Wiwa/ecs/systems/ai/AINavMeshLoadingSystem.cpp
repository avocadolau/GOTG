#include <wipch.h>
#include "AINavMeshLoadingSystem.h"
#include <Wiwa/AI/AI_RecastManager.h>
namespace Wiwa
{
	AINavMeshLoadingSystem::AINavMeshLoadingSystem()
	{
	}

	AINavMeshLoadingSystem::~AINavMeshLoadingSystem()
	{
	}

	void AINavMeshLoadingSystem::OnSystemAdded()
	{
		RecastManager::Load();
	}
}
