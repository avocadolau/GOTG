#include "UltronLaserBeamSystem.h"
#include "../../components/attack/UltronLaserBeam.h"
#include "../../systems/enemy/Ultron/StateMachine/BossUltronLaserBeamAttack.h"
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
	Wiwa::EntityManager& em = m_Scene->GetEntityManager();

	m_LaserIt = GetComponentIterator<UltronLaserBeam>();
	EntityId boss = em.GetEntityByName("Ultron_01"); // TODO: Change this to get the boss entity ID from the GameStatemanager

	if (boss != WI_INVALID_INDEX)
	{
		m_BossTransformIt = em.GetComponentIterator<Transform3D>(boss);
	}
	
	EntityId player = GameStateManager::GetPlayerId();
	m_PlayerTransformIt = em.GetComponentIterator<Transform3D>(player);
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
	Transform3D* laserTr = GetComponent<Transform3D>();
	Transform3D* enemyTr = GetComponentByIterator<Transform3D>(m_BossTransformIt);
	Transform3D* playerTr = GetComponentByIterator<Transform3D>(m_PlayerTransformIt);

	if (m_Timer >= bullet->lifeTime)
	{
		GameStateManager::s_PoolManager->s_UltronLaserBeamPool->ReturnToPool(m_EntityId);
	}

	// Update the laser's position to be in front of the boss
	float distanceInFront = 6.0f;
	float laserHalfLength = 26.5f; // Adjust this value based on the actual half length of the laser
	glm::vec3 bossForward = Math::CalculateForward(enemyTr);
	laserTr->localPosition = enemyTr->localPosition + (bossForward * (distanceInFront + laserHalfLength));
	laserTr->localPosition.y += 2.5f;

	// Update the boss's rotation to face the player
	glm::vec3 directionToPlayer = glm::normalize(Math::CalculateForward(enemyTr) - enemyTr->localPosition);
	float angleToPlayer = glm::degrees(atan2(directionToPlayer.x, directionToPlayer.z));
	enemyTr->localRotation.y = angleToPlayer;
	
	// Update the laser's rotation to match the boss's rotation
	laserTr->localRotation = glm::vec3(-90.0f, angleToPlayer, 0.0f);

	///////////////////////////////////////////

	//// Update the laser's position to be in front of the boss
	//float distanceInFront = 3.0f;
	//float laserHalfLength = 2.5f; // Adjust this value based on the actual half length of the laser
	//glm::vec3 bossForward = Math::CalculateForward(enemyTr);
	//laserTr->localPosition = enemyTr->localPosition + (bossForward * (distanceInFront + laserHalfLength));
	//
	//// Calculate the target rotation for the boss to face the player
	//glm::vec3 directionToPlayer = glm::normalize(playerTr->localPosition - enemyTr->localPosition);
	//float targetAngle = glm::degrees(atan2(directionToPlayer.x, directionToPlayer.z));
	//
	//// Interpolate the boss's rotation towards the target rotation based on rotation speed and delta time
	//float currentAngle = enemyTr->localRotation.y;
	//float newAngle = glm::mix(currentAngle, targetAngle, 0.5f);
	//enemyTr->localRotation.y = newAngle;
	//
	//// Update the laser's rotation to match the boss's rotation
	//laserTr->localRotation = glm::vec3(-90.0f, newAngle, 0.0f);


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
