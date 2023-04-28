#include "PhylasQuantumSwordSystem.h"
#include "../../components/attack/PhylasQuantumSword.h"
#include "Wiwa/ecs/systems/PhysicsSystem.h"
#include <Wiwa/game/Items/ItemManager.h>
#include <Wiwa/ecs/systems/MeshRenderer.h>
namespace Wiwa
{
	PhylasQuantumSwordSystem::PhylasQuantumSwordSystem()
	{
		m_PhylasSwordIt = { WI_INVALID_INDEX, WI_INVALID_INDEX };
		m_Timer = 0.0f;
	}

	PhylasQuantumSwordSystem::~PhylasQuantumSwordSystem()
	{

	}

	void PhylasQuantumSwordSystem::OnAwake()
	{
		m_PhylasSwordIt = GetComponentIterator<PhylasQuantumSword>();
		m_PhylasSwordTransformIt = GetComponentIterator<Transform3D>();

	}

	void PhylasQuantumSwordSystem::OnInit()
	{
		PhylasQuantumSword* PhylasSword = GetComponentByIterator<PhylasQuantumSword>(m_PhylasSwordIt);
		Transform3D* PhylasSwordTransform = GetComponentByIterator<Transform3D>(m_PhylasSwordTransformIt);
		Wiwa::EntityManager& em = m_Scene->GetEntityManager();
		PhylasSword->damage = Wiwa::ItemManager::GetAbility("Phyla's Quantum Sword")->Damage;
		PhylasSword->lifeTime = 30.0f;
		PhylasSword->velocity = 30.0f;
		
		EntityId player = GameStateManager::GetPlayerId();

		EntityId child = em.GetChildByName(m_EntityId,"bullet");
		m_ChildTransformIt = em.GetComponentIterator<Transform3D>(child);

		m_PlayerTransformIt = em.GetComponentIterator<Transform3D>(player);
		Transform3D* playerTransform = GetComponentByIterator<Transform3D>(m_PlayerTransformIt);
		Transform3D* childTransform = GetComponentByIterator<Transform3D>(m_ChildTransformIt);

		PhylasSword->direction = CalculateForward(*playerTransform);
		childTransform->localRotation = playerTransform->localRotation;

		PhylasSwordTransform->localPosition.x = playerTransform->localPosition.x;
		PhylasSwordTransform->localPosition.z = playerTransform->localPosition.z;
		PhylasSwordTransform->localPosition.y = playerTransform->localPosition.y + 2.55;

		PhylasSwordTransform->localScale.x = 1.0f;
		PhylasSwordTransform->localScale.z = 1.0f;
		PhylasSwordTransform->localScale.y = 1.0f;

		
	}

	void PhylasQuantumSwordSystem::OnUpdate()
	{
		if (!getAwake())
			System::Awake();
		if (!getInit())
			System::Init();

		PhylasQuantumSword* PhylasSword = GetComponentByIterator<PhylasQuantumSword>(m_PhylasSwordIt);
		Transform3D* PhylasSwordTransform = GetComponentByIterator<Transform3D>(m_PhylasSwordTransformIt);

		PhylasSwordTransform->localPosition += PhylasSword->direction * PhylasSword->velocity * Time::GetDeltaTimeSeconds();

		m_Timer += Time::GetDeltaTimeSeconds();

		
		if (m_Timer >= PhylasSword->lifeTime)
		{
			Wiwa::EntityManager& em = m_Scene->GetEntityManager();
			em.DestroyEntity(m_EntityId);
		}

	}


	void PhylasQuantumSwordSystem::OnDestroy()
	{

	}

	void PhylasQuantumSwordSystem::OnCollisionEnter(Object* body1, Object* body2)
	{
		std::string tag_wall = "WALL";
		std::string tag_column = "COLUMN";
		if (body1->id == m_EntityId && (tag_wall == body2->selfTagStr || tag_column == body2->selfTagStr))
		{
			Wiwa::EntityManager& em = m_Scene->GetEntityManager();
			em.DestroyEntity(m_EntityId);
		}
	}


	glm::vec3 PhylasQuantumSwordSystem::CalculateForward(const Transform3D& t3d)
	{
		glm::vec3 rotrad = glm::radians(t3d.rotation);

		glm::vec3 forward;

		forward.x = glm::cos(rotrad.x) * glm::sin(rotrad.y);
		forward.y = -glm::sin(rotrad.x);
		forward.z = glm::cos(rotrad.x) * glm::cos(rotrad.y);

		forward = glm::degrees(forward);

		return glm::normalize(forward);
	}

}