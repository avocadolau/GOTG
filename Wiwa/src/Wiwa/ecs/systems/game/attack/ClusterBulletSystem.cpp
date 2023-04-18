#include <wipch.h>
#include "ClusterBulletSystem.h"
#include "Wiwa/ecs/components/game/attack/ClusterBullet.h"
#include "Wiwa/ecs/systems/PhysicsSystem.h"
#include <Wiwa/utilities/EntityPool.h>
#include "Wiwa/ecs/components/game/attack/SimpleBullet.h"

Wiwa::ClusterBulletSystem::ClusterBulletSystem()
{
	m_BulletIt = { WI_INVALID_INDEX, WI_INVALID_INDEX };
	m_Timer = 0.0f;
}

Wiwa::ClusterBulletSystem::~ClusterBulletSystem()
{
	m_BulletIt = GetComponentIterator<ClusterBullet>();
}

void Wiwa::ClusterBulletSystem::OnAwake()
{
	m_BulletIt = GetComponentIterator<ClusterBullet>();	
}

void Wiwa::ClusterBulletSystem::OnInit()
{
	ClusterBullet* bullet = GetComponentByIterator<ClusterBullet>(m_BulletIt);

	bullet->damage = CLUSTER_BULLET_DAMAGE;
	bullet->lifeTime = CLUSTER_BULLET_LIFETIME;

	Wiwa::EntityManager& em = m_Scene->GetEntityManager();
	Wiwa::PhysicsManager& physicsManager = m_Scene->GetPhysicsManager();

	Wiwa::Object* obj = em.GetSystem<Wiwa::PhysicsSystem>(m_EntityId)->getBody();

	physicsManager.SetVelocity(obj, glm::normalize(bullet->direction) * bullet->velocity);
}

void Wiwa::ClusterBulletSystem::OnUpdate()
{
	if (!getAwake())
		System::Awake();
	if (!getInit())
		System::Init();

	ClusterBullet* bullet = GetComponentByIterator<ClusterBullet>(m_BulletIt);

	m_Timer += Time::GetDeltaTimeSeconds();

	if (m_Timer >= bullet->lifeTime)
	{
		Wiwa::EntityManager& em = m_Scene->GetEntityManager();
		em.DestroyEntity(m_EntityId);
	}
}

void Wiwa::ClusterBulletSystem::OnDestroy()
{
}

void Wiwa::ClusterBulletSystem::OnCollisionEnter(Object* body1, Object* body2)
{
	if (body1->id == m_EntityId)
	{
		std::string playerStr = "PLAYER";
		if (playerStr == body2->selfTagStr)
		{
			ClusterBullet* bullet = GetComponentByIterator<ClusterBullet>(m_BulletIt);
			GameStateManager::DamagePlayer(bullet->damage);
		}

		Wiwa::EntityManager& em = m_Scene->GetEntityManager();
		//em.DestroyEntity(m_EntityId);

		Math::RandomRange(0,1);

		int randomValue = Math::RandomRange(0, 1);

		if (randomValue == 0)
		{
			BlowClusterBullet01(m_EntityId);
		}
		else
		{
			BlowClusterBullet02(m_EntityId);
		}

		GameStateManager::s_PoolManager->s_ClusterBulletsPool->ReturnToPool(m_EntityId);
	}
}

void Wiwa::ClusterBulletSystem::SpawnBullet(const glm::vec3& bull_dir, EntityId simpleBullet)
{
	Wiwa::EntityManager& entityManager = this->getScene().GetEntityManager();
	EntityId newBulletId = entityManager.LoadPrefab("assets\\enemy\\simple_bullet\\simple_bullet.wiprefab");
	//entityManager.RemoveSystem(newBulletId, physicsSystemHash);

	// Set intial positions
	//Transform3D* bulletTr = (Transform3D*)entityManager.GetComponentByIterator(entityManager.GetComponentIterator<Transform3D>(newBulletId));
	Transform3D* clusterBulletTr = (Transform3D*)entityManager.GetComponentByIterator(entityManager.GetComponentIterator<Transform3D>(simpleBullet));

	ParticleManager& pman = this->getScene().GetParticleManager();

	pman.EmitBatch(newBulletId);

	if (!clusterBulletTr)
		return;

	clusterBulletTr->localPosition = glm::normalize(clusterBulletTr->localPosition);
	clusterBulletTr->localRotation = glm::vec3(-90.0f, 0.0f, bull_dir.y + 90.0f);
	//bulletTr->localScale = transform->localScale;
	SimpleBullet* bullet = (SimpleBullet*)entityManager.GetComponentByIterator(entityManager.GetComponentIterator<SimpleBullet>(newBulletId));
	bullet->direction = bull_dir;
	bullet->damage = 10;
}

void Wiwa::ClusterBulletSystem::BlowClusterBullet01(EntityId bulletId)
{
	Wiwa::EntityManager& entityManager = this->getScene().GetEntityManager();

	Transform3D* bulletTr = (Transform3D*)entityManager.GetComponentByIterator(entityManager.GetComponentIterator<Transform3D>(bulletId));

	int numBullets = 10;
	float degreeStep = 360.0f / numBullets;

	for (int i = 0; i < numBullets; ++i) {
		float directionAngle = i * degreeStep;
		float radian = directionAngle * (PI / 180.0f); // Convert degree to radian
		float xDir = cos(radian);
		float yDir = sin(radian);

		glm::vec3 direction(xDir, 0.0f, yDir);
		this->SpawnBullet(direction, bulletId);
	}
}

void Wiwa::ClusterBulletSystem::BlowClusterBullet02(EntityId bulletId)
{
	Wiwa::EntityManager& entityManager = this->getScene().GetEntityManager();

	Transform3D* bulletTr = (Transform3D*)entityManager.GetComponentByIterator(entityManager.GetComponentIterator<Transform3D>(bulletId));

	int numGroups = 10;
	int numBulletsPerGroup = 3;
	float degreeStep = 360.0f / numGroups;
	float groupDegreeStep = 10.0f; // The angle between bullets in a group

	for (int i = 0; i < numGroups; ++i) {
		for (int j = 0; j < numBulletsPerGroup; ++j) {
			float directionAngle = i * degreeStep + j * groupDegreeStep;
			float radian = directionAngle * (PI / 180.0f); // Convert degree to radian
			float xDir = cos(radian);
			float yDir = sin(radian);

			glm::vec3 direction(xDir, yDir, 0.0f);
			SpawnBullet(direction, bulletId);
		}
	}
}
