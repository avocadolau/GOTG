#include "CameraController.h"
#include <math.h>
#include <cmath>
#include "../../components/camera/PlayerCameraData.h"
#include "../../components/camera/CameraAgent.h"

namespace Wiwa
{
	CameraController::CameraController()
	{
	}

	CameraController::~CameraController()
	{
	}

	void CameraController::OnAwake()
	{
	}

	void CameraController::OnInit()
	{
		if (!getAwake())
			OnAwake();
		InitCamera();
		InitFOV();

		InitCameraAgents();
	}

	void CameraController::OnUpdate()
	{
		if (!getInit())
			OnInit();

		if (!m_Camera)
			return;
		if (m_PlayerId == WI_INVALID_INDEX)
		{
			m_PlayerId = GameStateManager::GetPlayerId();
			return;
		}	
		if (m_PlayerTransformIt.c_id == WI_INVALID_INDEX)
		{
			m_PlayerTransformIt = GetComponentIterator<Transform3D>(m_PlayerId);
			return;
		}		

		Wiwa::EntityManager& em = getScene().GetEntityManager();

		Transform3D* playerTr = (Transform3D*)em.GetComponentByIterator(m_PlayerTransformIt);

		glm::vec3 playerForward =  CalculateForward(*playerTr);
		glm::vec3 finalpos = playerTr->position + m_CameraOffset;
		m_Camera->setPosition(finalpos);


		UpdateCameraAgents();

		switch (GameStateManager::GetType())
		{
		case RoomType::ROOM_HUB:
		{
		}break;
		case RoomType::ROOM_REWARD:
			m_FOV = 40;
			break;
		case RoomType::ROOM_COMBAT:
		{
			BattleRoomFinished();
		}	break;
		case RoomType::ROOM_BOSS:
			m_FOV = 60;
			break;
		case RoomType::ROOM_SHOP:
			m_FOV = 60;
			break;
		default:
			m_FOV = 60;
			break;
		}
	}

	void CameraController::InitCameraAgents()
	{
		EntityManager& em = m_Scene->GetEntityManager();

		size_t size = 0;
		Wiwa::CameraAgent* cameraAgents = nullptr;
		cameraAgents = em.GetComponents<CameraAgent>(&size);

		if (cameraAgents) 
		{
			for (int i = 0; i < size; i++) 
			{
				if (em.IsComponentRemoved<CameraAgent>(i))
				{
				}
				else
				{
					Wiwa::CameraAgent* cameraAgent = &cameraAgents[i];
					if (cameraAgent)
					{
						m_CameraAgents.emplace_back(cameraAgent);
					}
				}
			}
		}
	}

	void CameraController::InitCamera()
	{
		Wiwa::CameraManager& cm = Wiwa::SceneManager::getActiveScene()->GetCameraManager();
		m_Camera = cm.getActiveCamera();
		m_PlayerId = GameStateManager::GetPlayerId();

		if (m_PlayerId != WI_INVALID_INDEX)
		{
			m_PlayerTransformIt = GetComponentIterator<Transform3D>(m_PlayerId);
		}
		m_Camera->setRotation(m_CameraRotation);
		m_CameraDataIt = GetComponentIterator<PlayerCameraData>();
	}

	void CameraController::InitFOV()
	{
		switch (GameStateManager::GetType())
		{
		case RoomType::ROOM_HUB:
		{
			
		}	break;
		case RoomType::ROOM_REWARD:
			m_FOV = 40;
			break;
		case RoomType::ROOM_COMBAT:
			m_FOV = 60;
			break;
		case RoomType::ROOM_BOSS:
			m_FOV = 60;
			break;
		case RoomType::ROOM_SHOP:
			m_FOV = 60;
			break;
		default:
			m_FOV = 60;
			break;
		}
		m_Camera->setFOV(m_FOV);
	}

	void CameraController::BattleRoomFinished()
	{
		if (GameStateManager::GetState() == RoomState::STATE_FINISHED)
		{
			PlayerCameraData* data = nullptr;
			if (m_CameraDataIt.c_id != WI_INVALID_INDEX)
				data = GetComponentByIterator<PlayerCameraData>(m_CameraDataIt);
			if (data)
			{
				if (m_FOV != data->MinCombatFOV)
				{
					m_FOV += data->CombatVelocity * Time::GetDeltaTimeSeconds();
					CLAMP(m_FOV, data->MinCombatFOV, data->MaxCombatFOV);
					m_Camera->setFOV(m_FOV);
				}
			}
			else
			{
				if (m_FOV != 40.f)
				{
					m_FOV += m_Velocity * Time::GetDeltaTimeSeconds();
					CLAMP(m_FOV, 40.f, 70.f);
					m_Camera->setFOV(m_FOV);
				}
			}
		}
	}

	void CameraController::UpdateCameraAgents()
	{
		Transform3D* playerT3D = GetComponentByIterator<Transform3D>(m_PlayerTransformIt);

		float nearDistance = 0;
		for (size_t i = 0; i < m_CameraAgents.size(); i++)
		{
			if (!m_CameraAgents[i])
				continue;
			if (!m_CameraAgents[i]->Active)
				continue;

			float distance = glm::distance(playerT3D->position, m_CameraAgents[i]->Position);

			if (i == 0)
				nearDistance = distance;
			else if (distance < nearDistance)
				nearDistance = distance;
		}

		PlayerCameraData* data = nullptr;
		if (m_CameraDataIt.c_id != WI_INVALID_INDEX)
			data = GetComponentByIterator<PlayerCameraData>(m_CameraDataIt);
		if (data)
		{
			m_FOV = mapValue(nearDistance, 0, 100.f, data->MinCombatFOV, data->MaxCombatFOV);
			CLAMP(m_FOV, data->MinCombatFOV, data->MaxCombatFOV);
		}
		else
		{
			m_FOV = mapValue(nearDistance, 0, 100.f, 40.f, 70.f);
			CLAMP(m_FOV, 40.f, 70.f);
		}
		m_Camera->setFOV(m_FOV);
	}

	void CameraController::OnDestroy()
	{
		m_Camera = nullptr;
	}

}