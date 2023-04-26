#include "BulletVelocity.h"
#include "../../components/BulletData.h"
#include <Wiwa/ecs/systems/PhysicsSystem.h>

Wiwa::BulletVelocity::~BulletVelocity()
{
}

void Wiwa::BulletVelocity::OnAwake()
{
	m_BulletDataIt = GetComponentIterator<BulletData>();
	m_Timer = 0.f;
}
void Wiwa::BulletVelocity::OnInit()
{
	BulletData* bulletData = GetComponentByIterator<BulletData>(m_BulletDataIt);
	glm::vec3 velocity = Math::CalculateForward(GetTransform()) * bulletData->Velocity;
	m_Scene->GetEntityManager().GetSystem<Wiwa::PhysicsSystem>(m_EntityId)->getBody()->velocity = Math::ToBulletVector3(velocity);
}
void Wiwa::BulletVelocity::OnUpdate()
{
	BulletData* bulletData = GetComponentByIterator<BulletData>(m_BulletDataIt);
	m_Timer += Time::GetDeltaTimeSeconds();

	if (m_Timer >= bulletData->LifeTime)
	{
		m_Scene->GetEntityManager().DestroyEntity(m_EntityId);
	}

}
