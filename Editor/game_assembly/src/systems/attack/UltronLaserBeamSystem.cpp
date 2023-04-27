#include <wipch.h>
#include "UltronLaserBeamSystem.h"
#include "../../components/attack/UltronLaserBeam.h"
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

void Wiwa::UltronLaserBeamSystem::InitLaserBeam()
{
	if (!getAwake())
		System::Awake();
	/*GameStateManager::s_PoolManager->SetScene(m_Scene);
	GameStateManager::s_PoolManager->s_SimpleBulletsPool->GetFromPool();*/

	//UltronLaserBeamSystem* bullet = GetComponentByIterator<UltronLaserBeamSystem>(m_LaserIt);
	//Wiwa::EntityManager& em = m_Scene->GetEntityManager();
	//Wiwa::Object* obj = em.GetSystem<Wiwa::PhysicsSystem>(m_EntityId)->getBody();
	//Wiwa::PhysicsManager& physicsManager = m_Scene->GetPhysicsManager();

	//physicsManager.SetVelocity(obj, glm::normalize(bullet->direction) * bullet->velocity);
	//Wiwa::EntityManager& em = m_Scene->GetEntityManager();

	//PhysicsSystem* physSystem = em.GetSystem<Wiwa::PhysicsSystem>(m_EntityId);

	//if (physSystem)
	//{
	//	physSystem->DeactivateBody();
	//}
}


bool Wiwa::UltronLaserBeamSystem::EnableLaser()
{
	UltronLaserBeamSystem* bullet = GetComponent<UltronLaserBeamSystem>();
	if (bullet)
	{
		bullet->InitLaserBeam();
	}
	m_Timer = 0.0f;
	return true;
}

bool Wiwa::UltronLaserBeamSystem::OnDisabledFromPool()
{
	Transform3D* transform = GetComponent<Transform3D>();
	if (transform)
	{
		transform->localPosition.y = 5000.0f;
	}

	//UltronLaserBeam* laserBeam = GetComponent<UltronLaserBeam>();

	Wiwa::EntityManager& em = m_Scene->GetEntityManager();

	PhysicsSystem* physSystem = em.GetSystem<Wiwa::PhysicsSystem>(m_EntityId);

	physSystem->DeleteBody();

	m_Timer = 0.0f;

	return true;
}
