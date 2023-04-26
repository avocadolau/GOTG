#include <wipch.h>
#include "BossUltronMovement.h"
#include <Wiwa/ecs/systems/game/enemy/Ultron/BossUltron.h>


namespace Wiwa
{
	std::vector<glm::vec2> BossUltronMovementState::m_PremadePositions;

	BossUltronMovementState::BossUltronMovementState()
	{
		m_PremadePositions.push_back({ 0,0});
		m_PremadePositions.push_back({ 2,2 });
		m_PremadePositions.push_back({ 3,1 });
		m_PremadePositions.push_back({ -2,-2 });
	}

	BossUltronMovementState::~BossUltronMovementState()
	{

	}

	void BossUltronMovementState::EnterState(BossUltron* enemy)
	{
		Wiwa::EntityManager& em = enemy->getScene().GetEntityManager();
		//Wiwa::AnimatorSystem* animator = em.GetSystem<Wiwa::AnimatorSystem>(enemy->GetEntity());
		//ParticleManager& pman = enemy->getScene().GetParticleManager();
		Wiwa::NavAgentSystem* navAgentPtr = em.GetSystem<Wiwa::NavAgentSystem>(enemy->GetEntity());

		//EntityId currentEnemy = enemy->GetEntity();


		//pman.EmitBatch(currentEnemy);

		//animator->PlayAnimation("move", true);

		glm::vec2 destination2D = RandomPremadePosition();

		glm::vec3 destination3D = {destination2D.x,0.0f,destination2D.y};

		if (navAgentPtr != nullptr)
		{
			navAgentPtr->SetDestination(destination3D);
			currentDestination = destination3D;
		}
	}

	void BossUltronMovementState::UpdateState(BossUltron* enemy)
	{
		Wiwa::EntityManager& em = enemy->getScene().GetEntityManager();
		//Wiwa::AnimatorSystem* animator = em.GetSystem<Wiwa::AnimatorSystem>(enemy->GetEntity());
		Transform3D* playerTr = (Transform3D*)em.GetComponentByIterator(enemy->m_PlayerTransformIt);
		Wiwa::NavAgentSystem* navAgentPtr = em.GetSystem<Wiwa::NavAgentSystem>(enemy->GetEntity());

		//if (animator->HasFinished())
		//enemy->SwitchState(enemy->m_ChasingState);
		

			if (Math::IsPointNear(navAgentPtr->GetCurrentPosition(), currentDestination, 2.0f)) // change it for a timer?
			{
				navAgentPtr->StopAgent();
				//agentPtr->StopMoving();
				std::srand(std::time(0));

				enemy->SwitchState(enemy->m_BulletStormAttackState);

				//int randomAction = Math::RandomRange(0, NUMBER_OF_RANDOM_ACTIONS);
				//switch (randomAction)
				//{
				//case 0:
				//{
				//	enemy->SwitchState(enemy->m_BulletStormAttackState);
				//}
				//break;
				//case 1:
				//{
				//	enemy->SwitchState(enemy->m_ClusterShotsAttackState);
				//}
				//break;
				//case 2:
				//{
				//	enemy->SwitchState(enemy->m_LaserBeamAttackState);
				//}
				//break;
				///*case 3:
				//{
				//	enemy->SwitchState(enemy->m_DashState);
				//}*/
				//}
			}
				
	}

	void BossUltronMovementState::ExitState(BossUltron* enemy)
	{

	}

	void BossUltronMovementState::OnCollisionEnter(BossUltron* enemy, const Object* body1, const Object* body2)
	{
	}

}