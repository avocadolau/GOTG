#include <wipch.h>
#include "EnemySystem.h"
#include <Wiwa/game/GameStateManager.h>
#include <Wiwa/ecs/systems/AgentAISystem.h>
#include <Wiwa/ecs/components/game/attack/PhylasQuantumSword.h>
namespace Wiwa
{
	struct BulletComponent
	{
		float Velocity;
		float LifeTime;
		int Damage;
		glm::vec3 Direction;
	};

	/*struct PhylasQuantumSword
	{
		float velocity;
		float lifeTime;
		int damage;
		glm::vec3 direction;
	};*/

	Wiwa::EnemySystem::EnemySystem()
	{
		m_EnemyIt = { WI_INVALID_INDEX, WI_INVALID_INDEX };
		m_StatsIt = { WI_INVALID_INDEX, WI_INVALID_INDEX };
		m_ColliderIt = { WI_INVALID_INDEX, WI_INVALID_INDEX };
		m_AgentIt = { WI_INVALID_INDEX, WI_INVALID_INDEX };
		m_TransformIt = { WI_INVALID_INDEX, WI_INVALID_INDEX };

		m_PlayerTransformIt = { WI_INVALID_INDEX, WI_INVALID_INDEX };
		m_PlayerStatsIt = { WI_INVALID_INDEX, WI_INVALID_INDEX };
		m_PlayerId = -1;
		m_WasSpawnedBySpawner = false;
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
		m_AgentIt = GetComponentIterator<AgentAI>();
		m_TransformIt = GetComponentIterator<Transform3D>();
		m_PlayerId = Wiwa::GameStateManager::s_PlayerId;
		m_PlayerTransformIt = GetComponentIterator<Transform3D>(m_PlayerId);
		m_PlayerStatsIt = GetComponentIterator<Character>(m_PlayerId);

		Enemy* self = GetComponentByIterator<Enemy>(m_EnemyIt);
		Transform3D* transform = GetComponentByIterator<Transform3D>(m_TransformIt);
		//transform->localPosition.y = 0.0f;

		self->currentRotation = transform->localRotation;
	}

	void EnemySystem::OnUpdate()
	{
		if (Input::IsKeyPressed(Wiwa::Key::M))
		{
			Character* stats = GetComponentByIterator<Character>(m_StatsIt);
			ReceiveDamage(100000);
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
			BulletComponent* bullet = em.GetComponent<BulletComponent>(body2->id);
			ReceiveDamage(bullet->Damage);
		}

		std::string phylasSword = "PHYLAS_QUANTUM_SWORD";
		if (body1->id == m_EntityId && phylasSword == body2->selfTagStr)
		{
			Wiwa::Scene* _scene = (Wiwa::Scene*)m_Scene;
			Wiwa::EntityManager& em = _scene->GetEntityManager();
			Wiwa::PhylasQuantumSword* phylasSword = em.GetComponent<Wiwa::PhylasQuantumSword>(body2->id);
			ReceiveDamage(phylasSword->damage);
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
			
			if (chances <= GameStateManager::s_EnemyDropChances)
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
		return true;
	}

	bool EnemySystem::OnDisabledFromPool()
	{
		Transform3D* transform = GetComponent<Transform3D>();
		if (transform)
		{
			transform->localPosition.y = 10000.0f;
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

	void EnemySystem::ChasePlayer()
	{
		Wiwa::EntityManager& em = m_Scene->GetEntityManager();

		Transform3D* playerTr = GetComponentByIterator<Transform3D>(m_PlayerTransformIt);
		Wiwa::AgentAISystem* agentPtr = em.GetSystem<Wiwa::AgentAISystem>(m_EntityId);

		if (agentPtr)
		{
			agentPtr->CreatePath(playerTr->localPosition);
		}
		//RotateTo(playerTr.Position,  enemy, entityId);                   
	}

	bool EnemySystem::GoToPosition(glm::vec3 targetedPosition)
	{
		bool ret = false;

		Wiwa::EntityManager& em = m_Scene->GetEntityManager();

		//Transform3D* playerTr = GetComponentByIterator<Transform3D>(m_PlayerTransformIt);
		Wiwa::AgentAISystem* agentPtr = em.GetSystem<Wiwa::AgentAISystem>(m_EntityId);

		if (agentPtr)
		{
			ret = agentPtr->CreatePath(targetedPosition);
		}
		//RotateTo(playerTr.Position,  enemy, entityId);        

		return ret;
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