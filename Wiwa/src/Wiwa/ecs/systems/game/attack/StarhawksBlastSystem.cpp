#include <wipch.h>
#include "StarhawksBlastSystem.h"
#include "Wiwa/ecs/components/game/attack/StarhawkBlast.h"
#include "Wiwa/ecs/systems/PhysicsSystem.h"
#include <Wiwa/game/Items/ItemManager.h>
#include <Wiwa/ecs/systems/MeshRenderer.h>

namespace Wiwa
{
	StarhawksBlastSystem::StarhawksBlastSystem()
	{
		m_StarHawksIt = { WI_INVALID_INDEX, WI_INVALID_INDEX };
		m_Timer = 0.0f;
	}

	StarhawksBlastSystem::~StarhawksBlastSystem()
	{

	}

	void StarhawksBlastSystem::OnAwake()
	{
		m_StarHawksIt = GetComponentIterator<StarhawksBlast>();
		m_StarHawksTransfromIt = GetComponentIterator<Transform3D>();
	}

	void StarhawksBlastSystem::OnInit()
	{
		Wiwa::EntityManager& em = m_Scene->GetEntityManager();
		StarhawksBlast* starhawksBlast = GetComponentByIterator<StarhawksBlast>(m_StarHawksIt);
		starhawksBlast->velocity = 200.f;
		starhawksBlast->lifeTime = Wiwa::ItemManager::GetBuff("Starhawk's Time Blast")->Duration;
		EntityId player = GameStateManager::GetPlayerId();

		m_PlayerTransformIt = em.GetComponentIterator<Transform3D>(player);
		EntityId child = em.GetChildByName(m_EntityId, "Colider");
		m_ColliderTransformIt = em.GetComponentIterator<Transform3D>(child);
		Mesh* mesh = em.AddComponent<Mesh>(child);

		mesh->meshId = Resources::Load<Model>("assets/prefabs/victoryshield/planebullet.fbx");
		mesh->materialId = Resources::Load<Material>("assets/prefabs/victoryshield/defaultmaterial.wimaterial");
		em.ApplySystem<MeshRenderer>(child);
	}

	void StarhawksBlastSystem::OnUpdate()
	{
		if (!getAwake())
			System::Awake();
		if (!getInit())
			System::Init();

		StarhawksBlast* starhawksBlast = GetComponentByIterator<StarhawksBlast>(m_StarHawksIt);

		Wiwa::Transform3D* transfromStarhawksBlast = GetComponentByIterator<Transform3D>(m_StarHawksTransfromIt);
		Wiwa::Transform3D* transformPlayer = GetComponentByIterator<Transform3D>(m_PlayerTransformIt);
		Wiwa::Transform3D* transformCol = GetComponentByIterator<Transform3D>(m_ColliderTransformIt);

		transfromStarhawksBlast->localPosition.x = transformPlayer->localPosition.x;
		transfromStarhawksBlast->localPosition.z = transformPlayer->localPosition.z;
		transfromStarhawksBlast->localRotation.y += starhawksBlast->velocity * Time::GetDeltaTimeSeconds();
		transformCol->localPosition.y = 0.f;
		m_Timer += Time::GetDeltaTimeSeconds();
		if (m_Timer >= starhawksBlast->lifeTime)
		{
			Wiwa::EntityManager& em = m_Scene->GetEntityManager();
			em.DestroyEntity(m_EntityId);
		}

	}


	void StarhawksBlastSystem::OnDestroy()
	{

	}

	void StarhawksBlastSystem::OnCollisionEnter(Object* body1, Object* body2)
	{
		/*if (body1->id == m_EntityId && strcmp(body2->selfTagStr, "ENEMY_BULLET") == 0)
		{
			Wiwa::EntityManager& em = m_Scene->GetEntityManager();
			em.DestroyEntity(body2->id);
		}*/
	}

}