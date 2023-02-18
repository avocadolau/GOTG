#include <wipch.h>
#include "PhysicsSystem.h"
#include "Wiwa/physics/PhysicsManager.h"

namespace Wiwa {
	PhysicsSystem::PhysicsSystem()
	{

	}
	PhysicsSystem::~PhysicsSystem()
	{

	}

	void PhysicsSystem::OnAwake()
	{
	}

	void PhysicsSystem::OnInit()
	{
		PhysicsManager::UpdateEngineToPhysics();
	}

	void PhysicsSystem::OnUpdate()
	{
		PhysicsManager::UpdateWorld();

		/*if (rb) {
			WI_INFO("{}", rb->x);
		}*/
	}
	void PhysicsSystem::OnDestroy()
	{
	}
	void PhysicsSystem::OnSystemAdded()
	{
	/*	Rigidbody* rb = GetComponent<Rigidbody>();
		if (rb)
		{
			Wiwa::EntityManager& entityManager = Wiwa::SceneManager::getActiveScene()->GetEntityManager();
			const char* e_name = entityManager.GetEntityName(m_EntityId);
			WI_INFO("{}", e_name);
		}*/
	}
	void PhysicsSystem::OnSystemRemoved()
	{
	}
}