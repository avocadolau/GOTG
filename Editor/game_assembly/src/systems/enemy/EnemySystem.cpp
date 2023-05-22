#include <wipch.h>
#include "EnemySystem.h"
#include <Wiwa/game/GameStateManager.h>
#include <Wiwa/ecs/components/game/enemy/Enemy.h>

#include <Wiwa/ecs/components/game/wave/WaveSpawner.h>
#include <Wiwa/ecs/components/game/Health.h>
#include <Wiwa/ecs/components/game/enemy/PhalanxMelee.h>
#include <Wiwa/ecs/components/game/enemy/PhalanxRanged.h>
#include <Wiwa/ecs/components/game/enemy/Sentinel.h>
#include <Wiwa/ecs/components/game/enemy/Subjugator.h>
#include <Wiwa/ecs/components/game/enemy/Ultron.h>

#include "../../components/attack/PhylasQuantumSword.h"
#include "../../components/attack/GrootSeeds.h"
#include "../../components/attack/StarhawkBlast.h"

#include <Wiwa/ecs/systems/OzzAnimationSystem.h>
#include <Wiwa/ecs/systems/AudioSystem.h>
#include <Wiwa/ecs/systems/ai/NavAgentSystem.h>
#include "../../components/attack/Attack.h"
#include "../../components/attack/SimpleBullet.h"
#include "../../components/attack/Explosion.h"
#include <Wiwa/ecs/components/game/Character.h>
#include "MeleePhalanx/EnemyMeleePhalanx.h"
#include "../../components/attack/YondusFin.h"

namespace Wiwa
{
	Wiwa::EnemySystem::EnemySystem()
	{
		m_EnemyStateIt = { WI_INVALID_INDEX, WI_INVALID_INDEX };
		m_StatsIt = { WI_INVALID_INDEX, WI_INVALID_INDEX };
		m_Health = { WI_INVALID_INDEX, WI_INVALID_INDEX };

		m_ColliderIt = { WI_INVALID_INDEX, WI_INVALID_INDEX };
		m_AgentIt = { WI_INVALID_INDEX, WI_INVALID_INDEX };
		m_NavAgentIt = { WI_INVALID_INDEX, WI_INVALID_INDEX };
		m_TransformIt = { WI_INVALID_INDEX, WI_INVALID_INDEX };
		m_AudioSourceIt = { WI_INVALID_INDEX, WI_INVALID_INDEX };

		m_PlayerTransformIt = { WI_INVALID_INDEX, WI_INVALID_INDEX };
		m_PlayerStatsIt = { WI_INVALID_INDEX, WI_INVALID_INDEX };
		m_PlayerId = -1;
		m_WaveId = -1;
		m_WasSpawnedBySpawner = false;

		m_PhysicsSystemHash = FNV1A_HASH("PhysicsSystem");

		m_NavAgentSys = nullptr;
		m_AnimatorSys = nullptr;
		m_AudioSys = nullptr;

		m_TimerSlow = 0.0f;
		m_PreviousSpeed = 0.0f;
		m_SlowPercentage = 1.0f;
	}

	EnemySystem::~EnemySystem()
	{
		m_NavAgentSys = nullptr;
		m_AnimatorSys = nullptr;
		m_AudioSys = nullptr;
	}

	void EnemySystem::OnAwake()
	{
	}

	void EnemySystem::OnInit()
	{
		m_EnemyStateIt = GetComponentIterator<EnemyState>();
		m_StatsIt = GetComponentIterator<EnemyData>();
		m_Health = GetComponentIterator<Health>();

		m_ColliderIt = GetComponentIterator<CollisionBody>();
		m_NavAgentIt = GetComponentIterator<NavAgent>();
		m_TransformIt = GetComponentIterator<Transform3D>();
		m_AudioSourceIt = GetComponentIterator<AudioSource>();
		m_PlayerId = Wiwa::GameStateManager::s_PlayerId;
		m_PlayerTransformIt = GetComponentIterator<Transform3D>(m_PlayerId);
		m_PlayerStatsIt = GetComponentIterator<Character>(m_PlayerId);

		SetStatsFromTable();

		EnemyState* self = GetComponentByIterator<EnemyState>(m_EnemyStateIt);
		Transform3D* transform = GetComponentByIterator<Transform3D>(m_TransformIt);
		AudioSource* audio = GetComponentByIterator<AudioSource>(m_AudioSourceIt);

		Wiwa::EntityManager& em = m_Scene->GetEntityManager();

		m_NavAgentSys = em.GetSystem<Wiwa::NavAgentSystem>(m_EntityId);
		m_AnimatorSys = em.GetSystem<Wiwa::OzzAnimationSystem>(m_EntityId);
		m_AudioSys = em.GetSystem<Wiwa::AudioSystem>(m_EntityId);

		self->currentRotation = transform->localRotation;

		NavAgent* agent = GetComponentByIterator<NavAgent>(m_NavAgentIt);
		if (agent)
			m_PreviousSpeed = agent->maxSpeed;
	}

	void EnemySystem::OnUpdate()
	{
		EnemyData* stats = GetComponentByIterator<EnemyData>(m_StatsIt);
		NavAgent* agent = GetComponentByIterator<NavAgent>(m_NavAgentIt);

		if (Input::IsKeyPressed(Wiwa::Key::M))
		{
			ReceiveDamage(100000);
		}

		if (m_TimerSlow > 0.0f)
		{
			// Slowed if has timer
			if (agent)
				agent->maxSpeed = m_PreviousSpeed * m_SlowPercentage;

			m_TimerSlow -= Time::GetDeltaTimeSeconds();
		}
		else
		{
			// Normal speed if no slow debuff
			if (agent)
				agent->maxSpeed = m_PreviousSpeed;
		}
	}

	void EnemySystem::OnDestroy()
	{
	}

	void EnemySystem::OnCollisionEnter(Object* body1, Object* body2)
	{
		std::string playerBulletStr = "PLAYER_ATTACK";
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
			
			Attack* attack = GetComponentByIterator<Attack>(em.GetComponentIterator<Attack>(body2->id));
			if (attack)
			{
				std::string attackStr = attack->attackType;

				std::string starlordBullet = "STARLORD_BULLET";
				if (starlordBullet == attackStr)
				{
					SimpleBullet* bullet = GetComponentByIterator<SimpleBullet>(em.GetComponentIterator<SimpleBullet>(body2->id));
					if (!bullet)
						return;
					ReceiveDamage(bullet->damage);
				}

				std::string yondusFinStr = "YONDUS_FIN";
				if (yondusFinStr == attackStr)
				{
					YondusFin* yondusFin = GetComponentByIterator<YondusFin>(em.GetComponentIterator<YondusFin>(body2->id));
					if (!yondusFin)
						return;
					ReceiveDamage(yondusFin->damage);
				}
			}
		}

		std::string sentinelExplosionStr = "SENTINEL_EXPLOSION";
		if (body1->id == m_EntityId && sentinelExplosionStr == body2->selfTagStr)
		{
			Wiwa::Scene* _scene = (Wiwa::Scene*)m_Scene;
			Wiwa::EntityManager& em = _scene->GetEntityManager();
			Wiwa::Explosion* explosion = em.GetComponent<Wiwa::Explosion>(body2->id);
			ReceiveDamage(explosion->damage);
		}

		std::string phylasSword = "PHYLAS_QUANTUM_SWORD";
		if (body1->id == m_EntityId && phylasSword == body2->selfTagStr)
		{
			Wiwa::Scene* _scene = (Wiwa::Scene*)m_Scene;
			Wiwa::EntityManager& em = _scene->GetEntityManager();
			Wiwa::PhylasQuantumSword* phylasSword = nullptr;
			phylasSword = em.GetComponent<Wiwa::PhylasQuantumSword>(body2->id);
			if (phylasSword != nullptr)
			{
				ReceiveDamage(phylasSword->damage);
			}
		}

		std::string groot_seeds = "GROOTS_SEEDS";
		if (body1->id == m_EntityId && groot_seeds == body2->selfTagStr)
		{
			Wiwa::Scene* _scene = (Wiwa::Scene*)m_Scene;
			Wiwa::EntityManager& em = _scene->GetEntityManager();
			Wiwa::GrootSeeds* grootSeeds = nullptr;
			grootSeeds = em.GetComponent<Wiwa::GrootSeeds>(body2->id);
			if (grootSeeds != nullptr)
			{
				EnemyData* statsSelf = GetComponentByIterator<EnemyData>(m_StatsIt);
				//statsSelf->Speed -= 2.0f;
				ReceiveDamage(grootSeeds->damage);
			}
		}

		std::string starhawks_blast = "STARHAWKS_BLAST";
		if (body1->id == m_EntityId && starhawks_blast == body2->selfTagStr)
		{
			Wiwa::Scene* _scene = (Wiwa::Scene*)m_Scene;
			Wiwa::EntityManager& em = _scene->GetEntityManager();
			Wiwa::StarhawksBlast* starhawks = nullptr;
			starhawks = em.GetComponent<Wiwa::StarhawksBlast>(body2->id);
			if (starhawks != nullptr)
			{
				EnemyData* statsSelf = GetComponentByIterator<EnemyData>(m_StatsIt);
				ReceiveDamage(starhawks->damage);
			}
		}
	}

	void EnemySystem::ReceiveDamage(int damage)
	{
		WI_INFO("Enemy hit by: {} damage", damage);
		Health* health = GetComponentByIterator<Health>(m_Health);
		EnemyData* statsSelf = GetComponentByIterator<EnemyData>(m_StatsIt);

		health->health = health->health - damage;
		if (health->health <= 0)
		{
			// Notify the player and spawn an item
			// TODO: Modify depending on the enemy
			/*switch (self->enemyType)
			{
			default:
				break;
			}*/
			GameStateManager::GetPlayerInventory().AddTokens(statsSelf->creditsDrop);
			Character* player = GameStateManager::GetCurrentScene()->GetEntityManager().GetComponent<Character>(GameStateManager::GetPlayerId());
			// As in the GDD for each enemy the player kills the shield regenerates
			// TODO: Change with a stats
			if (player)
			{
				player->Shield += player->ShieldRegeration;
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
					GameStateManager::SpawnItem(m_TransformIt, 3, "Healing Pills");
				}
				else if (IN_BETWEEN(chances, 51, 75))
				{
					GameStateManager::SpawnItem(m_TransformIt, 3, "Medkit");
				}
				else if (IN_BETWEEN(chances, 76, 85))
				{
					GameStateManager::SpawnItem(m_TransformIt, 3, "Shield Booster");
				}
				else if (IN_BETWEEN(chances, 86, 95))
				{
					GameStateManager::SpawnItem(m_TransformIt, 3, "First Aid Kit");
				}
				else if (IN_BETWEEN(chances, 96, 100))
				{
					GameStateManager::SpawnItem(m_TransformIt, 3, "Ego's Help");
				}
			}
		}
	}

	bool EnemySystem::OnEnabledFromPool()
	{
		Health* health = GetComponent<Health>();
		EnemyData* stats = GetComponent<EnemyData>();
		if (health)
		{
			health->health = stats->health;
		}

		EnemyState* self = GetComponent<EnemyState>();
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

		Health* health = GetComponent<Health>();
		EnemyData* stats = GetComponent<EnemyData>();
		if (health)
		{
			health->health = stats->health;
		}

		EnemyState* self = GetComponent<EnemyState>();
		if (self)
		{
			self->hasFinished = true;
		}
		return true;
	}

	void EnemySystem::LookAt(const glm::vec3& target_look, float rotation_speed)
	{
		Transform3D* transform = GetComponentByIterator<Transform3D>(m_TransformIt);

		float distance = glm::distance(transform->localPosition, target_look);

		float timeToRotate = distance / rotation_speed;
		float tRot = glm::clamp(Time::GetDeltaTimeSeconds() / timeToRotate, 0.0f, 1.0f);

		glm::vec3 forward = glm::normalize(target_look - transform->localPosition);

		float angle = glm::angle(forward, glm::vec3(0.0f, 0.0f, 1.0f));

		glm::vec3 crossProduct = glm::cross(forward, glm::vec3(0.0f, 0.0f, 1.0f));
		if (crossProduct.y > 0.0f) {
			angle = -angle;
		}

		float targetRotation = angle * 180 / glm::pi<float>();

		float rotationDifference = targetRotation - transform->localRotation.y;

		while (rotationDifference > 180.0f) {
			rotationDifference -= 360.0f;
		}
		while (rotationDifference < -180.0f) {
			rotationDifference += 360.0f;
		}

		float interpolatedRotation = transform->localRotation.y + rotationDifference * tRot;

		transform->localRotation.y = interpolatedRotation;
	}

	void EnemySystem::LookAtWithoutInterpolation(const glm::vec3& target_look)
	{
		Transform3D* transform = GetComponentByIterator<Transform3D>(m_TransformIt);

		glm::vec3 forward = glm::normalize(target_look - transform->localPosition);

		float angle = glm::angle(forward, glm::vec3(0.0f, 0.0f, 1.0f));

		glm::vec3 crossProduct = glm::cross(forward, glm::vec3(0.0f, 0.0f, 1.0f));
		if (crossProduct.y > 0.0f) {
			angle = -angle;
		}

		float targetRotation = angle * 180 / glm::pi<float>();

		float rotationDifference = targetRotation - transform->localRotation.y;

		while (rotationDifference > 180.0f) {
			rotationDifference -= 360.0f;
		}
		while (rotationDifference < -180.0f) {
			rotationDifference += 360.0f;
		}

		transform->localRotation.y = transform->localRotation.y + rotationDifference;
	}

	void EnemySystem::RotateTo(const glm::vec3& target)
	{
		float angle = glm::degrees(std::atan2(target.x, target.z));
		
		EnemyState* self = GetComponentByIterator<EnemyState>(m_EnemyStateIt);

		self->currentRotation.y = Wiwa::Math::LerpAngle(self->currentRotation.y, angle, 10);

		if (self->currentRotation.y >= 360.0f)
			self->currentRotation.y = 0.0f;

		Transform3D* transform = GetComponentByIterator<Transform3D>(m_TransformIt);

		transform->localRotation.y = self->currentRotation.y;
	}

	void EnemySystem::SetStatsFromTable()
	{
		Wiwa::EntityManager& entityManager = m_Scene->GetEntityManager();
		EnemyManager& enemyManager = Wiwa::GameStateManager::GetEnemyManager();
		auto& data = enemyManager.GetData();
		m_StatsIt = GetComponentIterator<EnemyData>();
		EnemyData* stats = GetComponentByIterator<EnemyData>(m_StatsIt);
		EnemyState* state = GetComponentByIterator<EnemyState>(m_EnemyStateIt);
		NavAgent* agent = GetComponentByIterator<NavAgent>(m_NavAgentIt);

		std::string nameType = typeid(this).name();

		PhalanxMelee* Pm = GetComponent<Wiwa::PhalanxMelee>();
		PhalanxRanged* Pr = GetComponent<Wiwa::PhalanxRanged>();
		Sentinel* Sent = GetComponent<Wiwa::Sentinel>();
		Subjugator* Sub = GetComponent<Wiwa::Subjugator>();
		Ultron* ultron = GetComponent<Wiwa::Ultron>();

		if (Pm)
		{
			if (Pm->variant == 1)
			{
				*stats = data[{enemyManager.m_CurrentRunLevel, "MELEE_PHALANX_REDVARIANT"}];
			}
			else if (Pm->variant == 2)
			{
				*stats = data[{enemyManager.m_CurrentRunLevel, "MELEE_PHALANX_BLUEVARIANT"}];
			}
			else
			{
				*stats = data[{enemyManager.m_CurrentRunLevel, "MELEE_PHALANX_GENERIC"}];
			}
		}
		else if (Pr)
		{
			if (Pr->variant == 1)
			{
				*stats = data[{enemyManager.m_CurrentRunLevel, "RANGED_PHALANX_REDVARIANT"}];
			}
			else if (Pr->variant == 2)
			{
				*stats = data[{enemyManager.m_CurrentRunLevel, "RANGED_PHALANX_BLUEVARIANT"}];
			}
			else
			{
				*stats = data[{enemyManager.m_CurrentRunLevel, "RANGED_PHALANX_GENERIC"}];
			}
		}
		else if (Sent)
		{
			*stats = data[{enemyManager.m_CurrentRunLevel, "SENTINEL"}];
		}
		else if (Sub)
		{
			if (Sub->variant == 1)
			{
				*stats = data[{enemyManager.m_CurrentRunLevel, "SUBJUGATOR_CHIEF"}];
			}
			else
			{
				*stats = data[{enemyManager.m_CurrentRunLevel, "SUBJUGATOR"}];
			}
		}
		else if (ultron)
		{
			*stats = data[{enemyManager.m_CurrentRunLevel, "ULTRON"}];
		}

		agent->maxAcceleration = stats->maxAcceleration;
		agent->maxSpeed = stats->maxVelocity;
		agent->stoppingDistance = stats->stoppingDistance;

		Health* health = GetComponent<Health>();
		if (health)
		{
			health->health = stats->health;
		}
	}

	// Time in seconds and percentage in 0 and 1 scale
	void EnemySystem::SlowForTime(float time, int percentage)
	{
		m_TimerSlow = time;
		m_SlowPercentage = percentage;
	}
}