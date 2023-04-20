#include <wipch.h>
#include "UltronLaserBeamSystem.h"
#include "Wiwa/ecs/components/game/attack/UltronLaserBeam.h"
#include "Wiwa/ecs/systems/PhysicsSystem.h"
#include <Wiwa/utilities/EntityPool.h>
//#include "Wiwa/ecs/components/game/attack/SimpleBullet.h"

Wiwa::UltronLaserBeamSystem::UltronLaserBeamSystem()
{
	m_LaserIt = { WI_INVALID_INDEX, WI_INVALID_INDEX };
	m_Timer = 0.0f;
}

Wiwa::UltronLaserBeamSystem::~UltronLaserBeamSystem()
{
}

void Wiwa::UltronLaserBeamSystem::OnAwake()
{
	m_LaserIt = GetComponentIterator<UltronLaserBeam>();
}

void Wiwa::UltronLaserBeamSystem::OnInit()
{
}

void Wiwa::UltronLaserBeamSystem::OnUpdate()
{
	if (!getAwake())
		System::Awake();
	if (!getInit())
		System::Init();

	UltronLaserBeam* bullet = GetComponentByIterator<UltronLaserBeam>(m_LaserIt);
	

	if (m_Timer >= bullet->lifeTime)
	{
		GameStateManager::s_PoolManager->s_UltronLaserBeamPool->ReturnToPool(m_EntityId);
	}

	m_Timer += Time::GetDeltaTimeSeconds();
}

void Wiwa::UltronLaserBeamSystem::OnDestroy()
{
}

void Wiwa::UltronLaserBeamSystem::OnCollisionEnter(Object* body1, Object* body2)
{
	std::string playerStr = "PLAYER";

	if (body1->id == m_EntityId && playerStr == body2->selfTagStr)
	{		
		UltronLaserBeam* bullet = GetComponentByIterator<UltronLaserBeam>(m_LaserIt);
		GameStateManager::DamagePlayer(bullet->damagePerSecond * Time::GetDeltaTime());
	}
}
