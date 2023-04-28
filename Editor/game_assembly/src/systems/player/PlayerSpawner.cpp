#include "PlayerSpawner.h"

namespace Wiwa
{
	Wiwa::PlayerSpawner::PlayerSpawner()
	{
		m_PlayerSet = false;
	}

	PlayerSpawner::~PlayerSpawner()
	{
	}

	void PlayerSpawner::OnInit()
	{
		Wiwa::EntityManager& em = m_Scene->GetEntityManager();

		Transform3D* tr = GetComponent<Transform3D>();

		playerId = em.LoadPrefab("assets\\Player\\Final\\Player.wiprefab");
		Transform3D* playerTr = em.GetComponent<Transform3D>(playerId);

		playerTr->localPosition.x = tr->worldMatrix[3].x;
		playerTr->localPosition.y = tr->worldMatrix[3].y;
		playerTr->localPosition.z = tr->worldMatrix[3].z;

		/*playerTr->position.x = tr->worldMatrix[3].x;
		playerTr->position.y = tr->worldMatrix[3].y;
		playerTr->position.z = tr->worldMatrix[3].z;*/
	}

	void PlayerSpawner::OnUpdate()
	{
		if (!m_PlayerSet)
		{
			Wiwa::EntityManager& em = m_Scene->GetEntityManager();
			Transform3D* tr = GetComponent<Transform3D>();
			Transform3D* playerTr = em.GetComponent<Transform3D>(playerId);

			playerTr->localPosition.x = tr->worldMatrix[3].x;
			playerTr->localPosition.y = tr->worldMatrix[3].y;
			playerTr->localPosition.z = tr->worldMatrix[3].z;
			m_PlayerSet = true;
		}
	}

}