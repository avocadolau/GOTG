#include "ZigZagSystem.h"
#include "../../components/attack/ZigZagBullet.h"
#include "Wiwa/ecs/systems/PhysicsSystem.h"
#include <Wiwa/utilities/EntityPool.h>
#include "../../components/attack/Attack.h"

namespace Wiwa
{
	ZigZagBulletSystem::ZigZagBulletSystem()
	{
		m_BulletIt = { WI_INVALID_INDEX, WI_INVALID_INDEX };
		m_AttackIt = { WI_INVALID_INDEX, WI_INVALID_INDEX };
		m_Timer = 0.0f;
		m_ZigZagTimer = 0.0f;
	}

	ZigZagBulletSystem::~ZigZagBulletSystem()
	{

	}

	void ZigZagBulletSystem::OnAwake()
	{
		m_AttackIt = GetComponentIterator<Attack>();
		m_BulletIt = GetComponentIterator<ZigZagBullet>();

		Attack* attack = GetComponentByIterator<Attack>(m_AttackIt);
		if (attack)
		{
			std::string atStr = "ZIG_ZAG_BULLET";
			strcpy(attack->attackType, atStr.c_str());
			attack->isPlayerAttack = false;
			attack->isEnemyAttack = true;
		}
	}

	void ZigZagBulletSystem::OnInit()
	{

	}

	void ZigZagBulletSystem::InitBullet()
	{
		if (!getAwake())
			System::Awake();
		
		ZigZagBullet* bullet = GetComponentByIterator<ZigZagBullet>(m_BulletIt);
		Wiwa::EntityManager& em = m_Scene->GetEntityManager();
		Wiwa::Object* obj = em.GetSystem<Wiwa::PhysicsSystem>(m_EntityId)->getBody();
		Wiwa::PhysicsManager& physicsManager = m_Scene->GetPhysicsManager();

		physicsManager.SetVelocity(obj, glm::normalize(bullet->direction) * bullet->velocity);
	}

	void ZigZagBulletSystem::OnUpdate()
	{
		if (!getAwake())
			System::Awake();
		if (!getInit())
			System::Init();

		Wiwa::EntityManager& em = m_Scene->GetEntityManager();
		ZigZagBullet* bullet = GetComponentByIterator<ZigZagBullet>(m_BulletIt);
		Transform3D* transform = (Transform3D*)em.GetComponentByIterator(em.GetComponentIterator<Transform3D>(m_EntityId));
		

		m_Timer += Time::GetDeltaTimeSeconds();
		m_ZigZagTimer += Time::GetDeltaTimeSeconds();

		glm::vec3 updatePosition = ZigZagBulletMotion(transform->localPosition, 2, 1.0f, 20, m_ZigZagTimer, bullet->direction);

		//WI_INFO(" Update Position ZIGZAG: x {} y {} z {}", updatePosition.x, updatePosition.y, updatePosition.z);

		transform->localPosition = updatePosition;
		em.GetSystem<Wiwa::PhysicsSystem>(m_EntityId)->ForceSetPosition(updatePosition);

		if (m_Timer >= bullet->lifeTime)
		{
			GameStateManager::s_PoolManager->s_ZigZagBulletPool->ReturnToPool(m_EntityId);
		}
	}

	void ZigZagBulletSystem::OnDestroy()
	{

	}

	void ZigZagBulletSystem::OnCollisionEnter(Object* body1, Object* body2)
	{
		if (body1->id == m_EntityId)
		{
			std::string playerStr = "PLAYER";
			if (playerStr == body2->selfTagStr)
			{
				ZigZagBullet* bullet = GetComponentByIterator<ZigZagBullet>(m_BulletIt);
				GameStateManager::DamagePlayer(bullet->damage);
			}

			GameStateManager::s_PoolManager->s_ZigZagBulletPool->ReturnToPool(m_EntityId);
		}
	}

	bool ZigZagBulletSystem::EnableBullet()
	{
		ZigZagBullet* bullet = GetComponent<ZigZagBullet>();
		if (bullet)
		{
			InitBullet();
		}
		m_Timer = 0.0f;
		m_ZigZagTimer = 0.0f;
		return true;
	}

	bool ZigZagBulletSystem::OnDisabledFromPool()
	{
		Transform3D* transform = GetComponent<Transform3D>();
		if (transform)
		{
			transform->localPosition.y = 3000.0f;
		}

		Wiwa::EntityManager& em = m_Scene->GetEntityManager();

		PhysicsSystem* physSystem = em.GetSystem<Wiwa::PhysicsSystem>(m_EntityId);

		physSystem->DeleteBody();

		m_Timer = 0.0f;
		m_ZigZagTimer = 0.0f;

		return true;
	}

	glm::vec3 ZigZagBulletSystem::ZigZagBulletMotion(glm::vec3 current_position, float speed, float amplitude, float frequency, float delta_time, glm::vec3 forward_direction)
	{
		// Normalize the forward_direction
		forward_direction = glm::normalize(forward_direction);
		
		// Calculate the local right and up vectors
		glm::vec3 up(0.0f, 0.0f, 1.0f);
		glm::vec3 right = glm::cross(forward_direction, up);
		
		// Calculate the zig-zag motion along the local X and Y axis
		float zigzag_x = amplitude * sin(frequency * delta_time);
		float zigzag_y = amplitude * cos(frequency * delta_time);
		
		// Transform the local zigzag motion to the global coordinate system
		glm::vec3 zigzag_global = zigzag_x * right + zigzag_y * up;
		
		// Calculate the new position along the forward direction, including the zigzag motion
		glm::vec3 new_position = current_position + forward_direction * speed * delta_time + zigzag_global;
		//WI_INFO(" currentPos: {} {} {}, speed: {}, delta time: {}, new_position: {} {} {}", current_position.x, current_position.y, current_position.z, speed, delta_time, new_position.x, new_position.y, new_position.z);
		
		return new_position;

		//// Normalize the forward_direction
		//forward_direction = glm::normalize(forward_direction);
		//
		//// Calculate the local right and up vectors
		//glm::vec3 world_up(0.0f, 0.0f, 1.0f);
		//glm::vec3 right = glm::normalize(glm::cross(forward_direction, world_up));
		//glm::vec3 new_up = glm::cross(right, forward_direction);
		//
		//// Calculate the zig-zag motion along the local X and Z axis
		//float zigzag_x = amplitude * sin(frequency * delta_time);
		//float zigzag_z = amplitude * cos(frequency * delta_time);
		//
		//// Transform the local zigzag motion to the global coordinate system
		//glm::vec3 zigzag_global = zigzag_x * right + zigzag_z * new_up;
		//
		//// Calculate the new position along the forward direction, including the zigzag motion
		//glm::vec3 new_position = current_position + forward_direction * speed * delta_time + zigzag_global;
		//
		//return new_position;
		
	}
}