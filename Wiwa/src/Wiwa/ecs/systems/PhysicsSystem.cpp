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
	}

	void PhysicsSystem::OnUpdate()
	{
		PhysicsManager* physicsManager = &Wiwa::SceneManager::getActiveScene()->GetPhysicsManager();

		if (Time::IsPlaying())
		{
			Wiwa::EntityManager& entityManager = Wiwa::SceneManager::getActiveScene()->GetEntityManager();
			const char* e_name = entityManager.GetEntityName(m_EntityId);
			WI_INFO("Updating physics of --> {}", e_name);

		}
	}

	void PhysicsSystem::OnDestroy()
	{
		DeleteBody();
	}

	void PhysicsSystem::OnSystemAdded()
	{
		Wiwa::EntityManager& entityManager = Wiwa::SceneManager::getActiveScene()->GetEntityManager();
		const char* e_name = entityManager.GetEntityName(m_EntityId);
		WI_INFO("Init physics of --> {}", e_name);

		PhysicsManager* physicsManager = &Wiwa::SceneManager::getActiveScene()->GetPhysicsManager();

		Rigidbody* rb = GetComponent<Rigidbody>();
		ColliderCube* cube = GetComponent<ColliderCube>();
		Transform3D* transform = GetComponent<Transform3D>();

		if (rb && cube)
		{
			physicsManager->AddBodyCube(m_EntityId, *cube, *transform, *rb);
			physicsManager->UpdateEngineToPhysics();
		}
	}
	void PhysicsSystem::OnSystemRemoved()
	{
		DeleteBody();
	}

	void PhysicsSystem::DeleteBody()
	{
		Wiwa::EntityManager& entityManager = Wiwa::SceneManager::getActiveScene()->GetEntityManager();
		const char* e_name = entityManager.GetEntityName(m_EntityId);
		WI_INFO("Deleting physics of --> {}", e_name);

		PhysicsManager* physicsManager = &Wiwa::SceneManager::getActiveScene()->GetPhysicsManager();

		if (physicsManager != NULL)
		{
			btRigidBody* body = physicsManager->FindByEntityId(m_EntityId);

			if (body != nullptr)
				physicsManager->DeleteBody(body);
		}
	}
}