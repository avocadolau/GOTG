#include <wipch.h>
#include "EnemySystem.h"
#include <Wiwa/game/GameStateManager.h>
#include <Wiwa/ecs/components/game/attack/PhylasQuantumSword.h>
#include <Wiwa/ecs/components/game/attack/GrootSeeds.h>
#include <Wiwa/ecs/components/game/attack/StarhawkBlast.h>
#include "Wiwa/ecs/components/game/wave/WaveSpawner.h"
#include <Wiwa/ecs/components/game/enemy/Enemy.h>
#include <Wiwa/ecs/systems/AnimatorSystem.h>
#include <Wiwa/ecs/systems/ai/NavAgentSystem.h>

namespace Wiwa
{
	Wiwa::EnemySystem::EnemySystem()
	{
		m_EnemyIt = { WI_INVALID_INDEX, WI_INVALID_INDEX };
		m_StatsIt = { WI_INVALID_INDEX, WI_INVALID_INDEX };
		m_ColliderIt = { WI_INVALID_INDEX, WI_INVALID_INDEX };
		m_AgentIt = { WI_INVALID_INDEX, WI_INVALID_INDEX };
		m_NavAgentIt = { WI_INVALID_INDEX, WI_INVALID_INDEX };
		m_TransformIt = { WI_INVALID_INDEX, WI_INVALID_INDEX };

		m_PlayerTransformIt = { WI_INVALID_INDEX, WI_INVALID_INDEX };
		m_PlayerStatsIt = { WI_INVALID_INDEX, WI_INVALID_INDEX };
		m_PlayerId = -1;
		m_WaveId = -1;
		m_WasSpawnedBySpawner = false;

		m_PhysicsSystemHash = FNV1A_HASH("PhysicsSystem");
	}

	EnemySystem::~EnemySystem()
	{
	}

	void EnemySystem::OnAwake()
	{
	}

	void EnemySystem::OnInit()
	{
		m_EnemyIt = GetComponentIterator<Enemy>();
		m_StatsIt = GetComponentIterator<Character>();
		m_ColliderIt = GetComponentIterator<CollisionBody>();
		m_NavAgentIt = GetComponentIterator<NavAgent>();
		m_TransformIt = GetComponentIterator<Transform3D>();
		m_PlayerId = Wiwa::GameStateManager::s_PlayerId;
		m_PlayerTransformIt = GetComponentIterator<Transform3D>(m_PlayerId);
		m_PlayerStatsIt = GetComponentIterator<Character>(m_PlayerId);

		Enemy* self = GetComponentByIterator<Enemy>(m_EnemyIt);
		Transform3D* transform = GetComponentByIterator<Transform3D>(m_TransformIt);
		Character* stats = GetComponentByIterator<Character>(m_StatsIt);
		//transform->localPosition.y = 0.0f;
		stats->Slowed = false;
		stats->CounterSlowed = 0.0f;
		self->currentRotation = transform->localRotation;
	}

	void EnemySystem::OnUpdate()
	{
		Character* stats = GetComponentByIterator<Character>(m_StatsIt);
		if (Input::IsKeyPressed(Wiwa::Key::M))
		{
			ReceiveDamage(100000);
		}
		Transform3D* transform = GetComponentByIterator<Transform3D>(m_TransformIt);
		transform->localPosition.y = 0.0f;
		
		//SLOWED LOGIC
		if (stats->Slowed)
		{
			stats->CounterSlowed += Time::GetDeltaTimeSeconds();
		}

		if (stats->CounterSlowed >= timerSlow && stats->Slowed)
		{
			stats->Slowed = false;
			stats->CounterSlowed = 0.0f;
			/*AgentAI* statsSelf = GetComponentByIterator<AgentAI>(m_AgentIt);
			statsSelf->speed = previousSpeed;*/
		}
	}

	void EnemySystem::OnDestroy()
	{
	}

	void EnemySystem::OnCollisionEnter(Object* body1, Object* body2)
	{
		std::string playerBulletStr = "PLAYER_BULLET";
		if (body1->id == m_EntityId && playerBulletStr == body2->selfTagStr)
		{
			Wiwa::Scene* _scene = (Wiwa::Scene*)m_Scene;
			Wiwa::EntityManager& em = _scene->GetEntityManager();
			//BulletComponent* bullet = em.GetComponent<BulletComponent>(body2->id);

			//MARTINEX THERMOKINESIS
			Character* stats = GetComponentByIterator<Character>(m_StatsIt);
			Inventory& player = Wiwa::GameStateManager::GetPlayerInventory();
			Buff** listBuffs = player.GetBuffs();
			for (int i = 0; i < 2; i++)
			{
				if (listBuffs[i] != nullptr)
				{
					if (listBuffs[i]->buffType == BuffType::MARTINEX_THERMOKINESIS && listBuffs[i]->IsActive && !stats->Slowed)
					{
						/*AgentAI* statsSelf = GetComponentByIterator<AgentAI>(m_AgentIt);
						const float buffPercent = ((float)listBuffs[i]->BuffPercent / 100.f);
						previousSpeed = statsSelf->speed;
						timerSlow = listBuffs[i]->Duration;
						statsSelf->speed = statsSelf->speed * buffPercent;
						stats->Slowed = true;*/
						break;
					}
				}
			}
		
			//ReceiveDamage(bullet->Damage);
		}

		std::string phylasSword = "PHYLAS_QUANTUM_SWORD";
		if (body1->id == m_EntityId && phylasSword == body2->selfTagStr)
		{
			Wiwa::Scene* _scene = (Wiwa::Scene*)m_Scene;
			Wiwa::EntityManager& em = _scene->GetEntityManager();
			Wiwa::PhylasQuantumSword* phylasSword = em.GetComponent<Wiwa::PhylasQuantumSword>(body2->id);
			ReceiveDamage(phylasSword->damage);
		}

		std::string groot_seeds = "GROOTS_SEEDS";
		if (body1->id == m_EntityId && groot_seeds == body2->selfTagStr)
		{
			Wiwa::Scene* _scene = (Wiwa::Scene*)m_Scene;
			Wiwa::EntityManager& em = _scene->GetEntityManager();
			Wiwa::GrootSeeds* grootSeeds = em.GetComponent<Wiwa::GrootSeeds>(body2->id);
			Character* statsSelf = GetComponentByIterator<Character>(m_StatsIt);
			statsSelf->Speed -= 2.0f;
			ReceiveDamage(grootSeeds->damage);
		}

		std::string starhawks_blast = "STARHAWKS_BLAST";
		if (body1->id == m_EntityId && starhawks_blast == body2->selfTagStr)
		{
			Wiwa::Scene* _scene = (Wiwa::Scene*)m_Scene;
			Wiwa::EntityManager& em = _scene->GetEntityManager();
			Wiwa::StarhawksBlast* starhawks = em.GetComponent<Wiwa::StarhawksBlast>(body2->id);
			Character* statsSelf = GetComponentByIterator<Character>(m_StatsIt);
			ReceiveDamage(starhawks->damage);
		}
	}

	void EnemySystem::ReceiveDamage(int damage)
	{
		WI_INFO("Enemy hit by: {} damage", damage);
		Character* statsSelf = GetComponentByIterator<Character>(m_StatsIt);
		Enemy* self = GetComponentByIterator<Enemy>(m_EnemyIt);

		statsSelf->Health = statsSelf->Health - damage;
		if (statsSelf->Health <= 0)
		{
			// Notify the player and spawn an item
			// TODO: Modify depending on the enemy
			/*switch (self->enemyType)
			{
			default:
				break;
			}*/
			GameStateManager::GetPlayerInventory().AddTokens(15);
			Character* player = GameStateManager::GetCurrentScene()->GetEntityManager().GetComponent<Character>(GameStateManager::GetPlayerId());
			// As in the GDD for each enemy the player kills the shield regenerates
			// TODO: Change with a stats
			if (player)
			{
				player->Shield += 2;
				if (player->Shield >= player->MaxShield)
					player->Shield = player->MaxShield;
			}
			// Spawn an item
			uint32_t chances = RAND(0, 100);
			
			if (chances <= (uint)GameStateManager::s_EnemyDropChances)
			{
				// Chances 09/04/2023 (all inclusive)
				// Healing pills 50% = 1 - 50
				// Medkit 25% = 51 - 75
				// Shield booster 10% = 76 - 85
				// First aid kit 10% = 86 - 95
				// Ego's help 5% = 96 - 100
				chances = RAND(1, 100);
				Transform3D* t3d = GetComponentByIterator<Transform3D>(m_TransformIt);
				
				// Healing pills
				if (chances <= 50)
				{
					GameStateManager::SpawnItem(t3d->localPosition, 3, "Healing Pills");
				}
				else if (IN_BETWEEN(chances, 51, 75))
				{
					GameStateManager::SpawnItem(t3d->localPosition, 3, "Medkit");
				}
				else if (IN_BETWEEN(chances, 76, 85))
				{
					GameStateManager::SpawnItem(t3d->localPosition, 3, "Shield Booster");
				}
				else if (IN_BETWEEN(chances, 86, 95))
				{
					GameStateManager::SpawnItem(t3d->localPosition, 3, "First Aid Kit");
				}
				else if (IN_BETWEEN(chances, 96, 100))
				{
					GameStateManager::SpawnItem(t3d->localPosition, 3, "Ego's Help");
				}
			}
		}
	}

	bool EnemySystem::OnEnabledFromPool()
	{
		Character* stats = GetComponent<Character>();
		if (stats)
		{
			stats->Health = stats->MaxHealth;
		}

		Enemy* self = GetComponent<Enemy>();
		if (self)
		{
			self->hasFinished = false;
		}

		Transform3D* transform = GetComponent<Transform3D>();
		if (transform)
		{
			transform->localPosition.y = 0.0f;
		}
		return true;
	}

	bool EnemySystem::OnDisabledFromPool()
	{
		Transform3D* transform = GetComponent<Transform3D>();
		if (transform)
		{
			transform->localPosition.y = 1000.0f;
		}

		Character* stats = GetComponent<Character>();
		if (stats)
		{
			stats->Health = stats->MaxHealth;
		}

		Enemy* self = GetComponent<Enemy>();
		if (self)
		{
			self->hasFinished = true;
		}
		return true;
	}

	void EnemySystem::LookAt(const glm::vec3& target_look)
	{
		Transform3D* transform = GetComponentByIterator<Transform3D>(m_TransformIt);

		float distance = glm::distance(transform->localPosition, target_look);

		float timeToRotate = distance / 30.0f;
		float tRot = glm::clamp(Time::GetDeltaTimeSeconds() / timeToRotate, 0.0f, 1.0f);

		// Calculate the forward vector from the current position to the target position
		glm::vec3 forward = glm::normalize(target_look - transform->localPosition);

		// Calculate the angle between the forward vector and the world forward vector (0, 0, 1)
		float angle = glm::angle(forward, glm::vec3(0.0f, 0.0f, 1.0f));

		// Determine the sign of the angle based on the cross product between the forward vector and the world forward vector (0, 0, 1)
		glm::vec3 crossProduct = glm::cross(forward, glm::vec3(0.0f, 0.0f, 1.0f));
		if (crossProduct.y > 0.0f) {
			angle = -angle;
		}

		float targetRotation = angle * 180 / glm::pi<float>();

		// Calculate the difference between the current rotation and target rotation
		float rotationDifference = targetRotation - transform->localRotation.y;

		// Adjust the rotation difference to be within the range of -180 to 180 degrees
		while (rotationDifference > 180.0f) {
			rotationDifference -= 360.0f;
		}
		while (rotationDifference < -180.0f) {
			rotationDifference += 360.0f;
		}

		// Calculate the new interpolated rotation
		float interpolatedRotation = transform->localRotation.y + rotationDifference * tRot;

		transform->localRotation.y = interpolatedRotation;
	}

	void EnemySystem::RotateTo(const glm::vec3& target)
	{
		float angle = glm::degrees(std::atan2(target.x, target.z));
		
		Enemy* self = GetComponentByIterator<Enemy>(m_EnemyIt);

		self->currentRotation.y = Wiwa::Math::LerpAngle(self->currentRotation.y, angle, 10);

		if (self->currentRotation.y >= 360.0f)
			self->currentRotation.y = 0.0f;

		Transform3D* transform = GetComponentByIterator<Transform3D>(m_TransformIt);

		transform->localRotation.y = self->currentRotation.y;
	}
}