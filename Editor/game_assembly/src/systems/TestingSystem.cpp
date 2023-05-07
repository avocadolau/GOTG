#include "TestingSystem.h"
#include <Wiwa/ecs/systems/PhysicsSystem.h>

namespace Wiwa
{
	TestingSystem::TestingSystem()
	{
		m_SpawnOnce = false;
	}

	TestingSystem::~TestingSystem()
	{
	}

	void TestingSystem::OnAwake()
	{
	}

	void TestingSystem::OnInit()
	{
		SetTransform();
	}

	void TestingSystem::OnUpdate()
	{
		if (Input::IsKeyPressed(Key::P) && !m_SpawnOnce)
		{
			EntityManager& em = GetEntityManager();
			EntityId id = em.LoadPrefab("assets\\Prefabs\\YondusFin\\YundusFin_01.wiprefab");
			Transform3D* tr = em.GetComponent<Transform3D>(id);
			PhysicsSystem* physSys = em.GetSystem<PhysicsSystem>(id);
			physSys->DeleteBody();
			*tr = *GetTransform();
			tr->localPosition.y = 2.3f;
			tr->position = tr->localPosition;
			physSys->CreateBody();
			m_SpawnOnce = true;
		}
	}
}