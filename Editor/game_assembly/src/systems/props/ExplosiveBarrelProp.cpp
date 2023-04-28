#include "ExplosiveBarrelProp.h"
#include "Wiwa/ecs/systems/PhysicsSystem.h"
#include <glm/gtx/quaternion.hpp>
#include "../attack/ExplosiveBarrelSystem.h"
#include <Wiwa/game/GameStateManager.h>

namespace Wiwa
{

	Wiwa::ExplosiveBarrelProp::ExplosiveBarrelProp()
	{

	}

	ExplosiveBarrelProp::~ExplosiveBarrelProp()
	{
	}

	void ExplosiveBarrelProp::OnAwake()
	{
	}

	void ExplosiveBarrelProp::OnInit()
	{

	}

	void ExplosiveBarrelProp::OnUpdate()
	{

	}

	void ExplosiveBarrelProp::OnDestroy()
	{
	}

	void ExplosiveBarrelProp::OnCollisionEnter(Object* body1, Object* body2)
	{
		std::string playerBulletStr = "PLAYER"; //Add PLAYER_BULLET when works again
		if (body1->id == m_EntityId && playerBulletStr == body2->selfTagStr)
		{
			Wiwa::Scene* _scene = (Wiwa::Scene*)m_Scene;
			Wiwa::EntityManager& em = _scene->GetEntityManager();
			Transform3D* selfTr = (Transform3D*)em.GetComponentByIterator(this->m_TransformIt);

			SpawnExplosiveBarrelExplosion(this, selfTr);

			//Destroy the barrel
			em.DestroyEntity(m_EntityId);
		}
	}

	void ExplosiveBarrelProp::SpawnExplosiveBarrelExplosion(ExplosiveBarrelProp* enemy, Wiwa::Transform3D* transform)
	{
		if (GameStateManager::s_PoolManager->s_ExplosiveBarrel->getCountDisabled() <= 0)
			return;

		Wiwa::EntityManager& entityManager = enemy->getScene().GetEntityManager();
		GameStateManager::s_PoolManager->SetScene(&enemy->getScene());
		EntityId newExplosionId = EntityManager::INVALID_INDEX;
		newExplosionId = GameStateManager::s_PoolManager->s_ExplosiveBarrel->GetFromPool();

		if (newExplosionId == EntityManager::INVALID_INDEX)
			return;

		ExplosiveBarrelSystem* explosionBarrelSys = entityManager.GetSystem<ExplosiveBarrelSystem>(newExplosionId);
		PhysicsSystem* physSys = entityManager.GetSystem<PhysicsSystem>(newExplosionId);
		physSys->DeleteBody();

		// Set intial positions
		//Transform3D* playerTr = (Transform3D*)entityManager.GetComponentByIterator(enemy->m_TransformIt); //Transform
		Transform3D* explosiveBarrelTr = (Transform3D*)entityManager.GetComponentByIterator(entityManager.GetComponentIterator<Transform3D>(newExplosionId));

		if (!explosiveBarrelTr)
			return;

		explosiveBarrelTr->localPosition = Math::GetWorldPosition(transform->worldMatrix);
		explosiveBarrelTr->localScale = transform->localScale;

		physSys->CreateBody();

		explosionBarrelSys->EnableExplosion();
	}
}