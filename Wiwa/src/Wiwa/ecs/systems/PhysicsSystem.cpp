#include <wipch.h>
#include "PhysicsSystem.h"
#include "Wiwa/physics/PhysicsManager.h"
#include "Wiwa/core/Resources.h"
#include "Wiwa/utilities/render/Model.h"
#include "Wiwa/ecs/components/Mesh.h"

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
		//PhysicsManager& physicsManager = Wiwa::SceneManager::getActiveScene()->GetPhysicsManager();

		Wiwa::EntityManager& entityManager = Wiwa::SceneManager::getActiveScene()->GetEntityManager();
		const char* e_name = entityManager.GetEntityName(m_EntityId);
		WI_INFO("Updating physics of --> {}", e_name);
	}

	void PhysicsSystem::OnDestroy()
	{
		DeleteBody();
	}

	void PhysicsSystem::OnSystemAdded()
	{
		Wiwa::EntityManager& entityManager = m_Scene->GetEntityManager();
		const char* e_name = entityManager.GetEntityName(m_EntityId);
		WI_INFO("Init physics of --> {}", e_name);

		Rigidbody* rb = GetComponent<Rigidbody>();
		ColliderCube* cube = GetComponent<ColliderCube>();
		Transform3D* transform = GetComponent<Transform3D>();
		Mesh* mesh = GetComponent<Mesh>();

		PhysicsManager& physicsManager = m_Scene->GetPhysicsManager();

		if (rb && cube)
		{
			if (mesh)
			{
				Model* root_mod = Wiwa::Resources::GetResourceById<Wiwa::Model>(mesh->meshId);
				cube->halfExtents = root_mod->boundingBox.HalfSize();
			}

			physicsManager.AddBodyCube(m_EntityId, *cube, *transform, *rb);
			//physicsManager.UpdateEngineToPhysics();
		}
	}
	void PhysicsSystem::OnSystemRemoved()
	{
		DeleteBody();
	}

	void PhysicsSystem::DeleteBody()
	{
		PhysicsManager& physicsManager = m_Scene->GetPhysicsManager();

		btRigidBody* body = physicsManager.FindByEntityId(m_EntityId);

		if (body != nullptr)
			physicsManager.DeleteBody(body);
	}
}