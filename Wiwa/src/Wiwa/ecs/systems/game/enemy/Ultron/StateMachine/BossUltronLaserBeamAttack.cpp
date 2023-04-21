#include <wipch.h>
#include "BossUltronLaserBeamAttack.h"
#include <Wiwa/ecs/systems/game/enemy/Ultron/BossUltron.h>
#include "Wiwa/ecs/components/game/attack/UltronLaserBeam.h"

namespace Wiwa
{
	BossUltronLaserBeamAttackState::BossUltronLaserBeamAttackState()
	{

	}

	BossUltronLaserBeamAttackState::~BossUltronLaserBeamAttackState()
	{

	}

	void BossUltronLaserBeamAttackState::EnterState(BossUltron* enemy)
	{
		m_Timer = 0.0f;		
		shootLaser = true;
	}

	void BossUltronLaserBeamAttackState::UpdateState(BossUltron* enemy)
	{
		Wiwa::EntityManager& em = enemy->getScene().GetEntityManager();
		Transform3D* selfTr = (Transform3D*)em.GetComponentByIterator(enemy->m_TransformIt);

		if (m_Timer >= 2.f && shootLaser == true)
		{
			SpawnLaserBeam(enemy, CalculateForward(*selfTr));

			shootLaser = false;
		}
		
		if (m_Timer >= 8.f)
		{
			enemy->SwitchState(enemy->m_MovementState);
		}		

		m_Timer += Time::GetDeltaTimeSeconds();
	}

	void BossUltronLaserBeamAttackState::ExitState(BossUltron* enemy)
	{
	}

	void BossUltronLaserBeamAttackState::OnCollisionEnter(BossUltron* enemy, const Object* body1, const Object* body2)
	{
	}

	EntityId* BossUltronLaserBeamAttackState::SpawnLaserBeam(BossUltron* enemy, const glm::vec3& bull_dir)
	{
		Wiwa::EntityManager& entityManager = enemy->getScene().GetEntityManager();
		EntityId newBulletId = entityManager.LoadPrefab("assets\\enemy\\ultron_laser_beam\\ultron_laser_beam.wiprefab");
		//entityManager.RemoveSystem(newBulletId, physicsSystemHash);

		// Set intial positions
		Transform3D* bulletTr = (Transform3D*)entityManager.GetComponentByIterator(entityManager.GetComponentIterator<Transform3D>(newBulletId));
		Transform3D* enemyTr = (Transform3D*)entityManager.GetComponentByIterator(entityManager.GetComponentIterator<Transform3D>(enemy->GetEntity()));


		if (!bulletTr || !enemyTr)
			return &newBulletId;

		bulletTr->localPosition = glm::normalize(enemyTr->localPosition);
		bulletTr->localRotation = glm::vec3(-90.0f, 0.0f, bull_dir.y + 90.0f);
		//bulletTr->localScale = transform->localScale;
		UltronLaserBeam* bullet = (UltronLaserBeam*)entityManager.GetComponentByIterator(entityManager.GetComponentIterator<UltronLaserBeam>(newBulletId));
		bullet->direction = bull_dir;

		return &newBulletId;
	}

	glm::vec3 BossUltronLaserBeamAttackState::CalculateForward(const Transform3D& t3d)
	{
		/*glm::vec4 forward = glm::vec4(0.0f, 0.0f, -1.0f, 0.0f);
		glm::vec4 transformed = t3d.worldMatrix * forward;
		return glm::normalize(glm::vec3(transformed));*/
		glm::vec3 rotrad = glm::radians(t3d.rotation);

		glm::vec3 forward;

		forward.x = glm::cos(rotrad.x) * glm::sin(rotrad.y);
		forward.y = -glm::sin(rotrad.x);
		forward.z = glm::cos(rotrad.x) * glm::cos(rotrad.y);

		forward = glm::degrees(forward);

		return glm::normalize(forward);
	}
}