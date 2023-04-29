#include "YondusFinSystem.h"
#include "../../components/attack/YondusFin.h"
#include "Wiwa/ecs/systems/PhysicsSystem.h"
#include <Wiwa/game/Items/ItemManager.h>
#include <Wiwa/ecs/systems/MeshRenderer.h>
namespace Wiwa
{
	YondusFinSystem::YondusFinSystem()
	{
		m_YondusFinIt = { WI_INVALID_INDEX, WI_INVALID_INDEX };
		m_Timer = 0.0f;
	}

	YondusFinSystem::~YondusFinSystem()
	{

	}

	void YondusFinSystem::OnAwake()
	{
		m_YondusFinIt = GetComponentIterator<YondusFin>();
		m_YondusFinTransformIt = GetComponentIterator<Transform3D>();

	}

	void YondusFinSystem::OnInit()
	{
		YondusFin* yondus_fin = GetComponentByIterator<YondusFin>(m_YondusFinIt);
		Transform3D* yondus_fin_transform = GetComponentByIterator<Transform3D>(m_YondusFinTransformIt);
		Wiwa::EntityManager& em = m_Scene->GetEntityManager();
		yondus_fin->damage = Wiwa::ItemManager::GetAbility("Yondu's Fin")->Damage;
		yondus_fin->lifeTime = 5.0f;

		EntityId player = GameStateManager::GetPlayerId();

		m_PlayerTransformIt = em.GetComponentIterator<Transform3D>(player);
		Transform3D* playerTransform = GetComponentByIterator<Transform3D>(m_PlayerTransformIt);

		//std::vector<EntityId>* enemies = em.GetEntitiesAlive(m_EntityId);

		for (size_t i = 0; i < 10; i++)
		{
			//m_EnemiesTransformIt.push_back(em.GetComponentIterator<Transform3D>(children->at(i)));
		}
		

	}

	void YondusFinSystem::OnUpdate()
	{
		if (!getAwake())
			System::Awake();
		if (!getInit())
			System::Init();

		YondusFin* yondusFin = GetComponentByIterator<YondusFin>(m_YondusFinIt);

		Transform3D* playerTransform = GetComponentByIterator<Transform3D>(m_PlayerTransformIt);
		Transform3D* grootSeedsTransform = GetComponentByIterator<Transform3D>(m_YondusFinTransformIt);

		m_Timer += Time::GetDeltaTimeSeconds();


		if (m_Timer >= yondusFin->lifeTime)
		{
			Wiwa::EntityManager& em = m_Scene->GetEntityManager();
			em.DestroyEntity(m_EntityId);
		}

	}


	void YondusFinSystem::OnDestroy()
	{

	}

	void YondusFinSystem::OnCollisionEnter(Object* body1, Object* body2)
	{
	}


}