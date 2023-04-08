#include <wipch.h>
#include "VictoryShieldSystem.h"
#include "Wiwa/ecs/components/game/attack/VictoryShield.h"
#include "Wiwa/ecs/systems/PhysicsSystem.h"

namespace Wiwa
{
	VictoryShieldSystem::VictoryShieldSystem()
	{
		m_ShieldIt = { WI_INVALID_INDEX, WI_INVALID_INDEX };
		m_Timer = 0.0f;
	}

	VictoryShieldSystem::~VictoryShieldSystem()
	{

	}

	void VictoryShieldSystem::OnAwake()
	{
		m_ShieldIt = GetComponentIterator<VictoryShield>();
		m_ShieldTransfromIt = GetComponentIterator<Transform3D>();
	}

	void VictoryShieldSystem::OnInit()
	{
		VictoryShield* shield = GetComponentByIterator<VictoryShield>(m_ShieldIt);

		Wiwa::EntityManager& em = m_Scene->GetEntityManager();
		//Wiwa::PhysicsManager& physicsManager = m_Scene->GetPhysicsManager();

		EntityId player = GameStateManager::GetPlayerId();

		//Wiwa::Object* obj = em.GetSystem<Wiwa::PhysicsSystem>(m_EntityId)->getBody();
		
		m_PlayerTransformIt = GetComponentIterator<Transform3D>(player);

		Wiwa::Transform3D* tranformShield = GetComponentByIterator<Transform3D>(m_ShieldTransfromIt);
		Wiwa::Transform3D* transformPlayer = GetComponentByIterator<Transform3D>(m_PlayerTransformIt);

		tranformShield->position = transformPlayer->position - glm::vec3(2.0f, 0.0f, 0.0f);
		//physicsManager.SetVelocity(obj, glm::normalize(shield->direction) * shield->velocity);
		
	}

	void VictoryShieldSystem::OnUpdate()
	{
		if (!getAwake())
			System::Awake();
		if (!getInit())
			System::Init();

		VictoryShield* shield = GetComponentByIterator<VictoryShield>(m_ShieldIt);

		Wiwa::Transform3D* tranformShield = GetComponentByIterator<Transform3D>(m_ShieldTransfromIt);
		Wiwa::Transform3D* transformPlayer = GetComponentByIterator<Transform3D>(m_PlayerTransformIt);

		shield->rotationAxis = glm::normalize(glm::cross(tranformShield->localPosition - transformPlayer->localPosition, glm::vec3(0, 1, 0)));

		glm::mat4 translation = glm::translate(glm::mat4(1.0f), -transformPlayer->localPosition);
		glm::mat4 rotation = glm::rotate(glm::mat4(1.0f), shield->angle, shield->rotationAxis);
		glm::mat4 reverseTranslation = glm::translate(glm::mat4(1.0f), transformPlayer->localPosition);
		glm::mat4 transformation = reverseTranslation * rotation * translation;

		glm::vec4 objectPosition4 = glm::vec4(tranformShield->localPosition, 1.0f);
		glm::vec4 transformedPosition4 = transformation * objectPosition4;
		glm::vec3 transformedPosition = glm::vec3(transformedPosition4.x, transformedPosition4.y, transformedPosition4.z);

		tranformShield->localPosition = transformedPosition;
		m_Timer += Time::GetDeltaTimeSeconds();

		if (m_Timer >= shield->lifeTime)
		{
			Wiwa::EntityManager& em = m_Scene->GetEntityManager();
			em.DestroyEntity(m_EntityId);
		}

	}


	void VictoryShieldSystem::OnDestroy()
	{

	}

	void VictoryShieldSystem::OnCollisionEnter(Object* body1, Object* body2)
	{
		/*if (body1->id == m_EntityId && strcmp(body2->selfTagStr, "ENEMY_BULLET") == 0)
		{
			Wiwa::EntityManager& em = m_Scene->GetEntityManager();
			em.DestroyEntity(body2->id);
		}*/
	}

}
