#include "wipch.h"
#include "CameraController.h"
#include <math.h>
#include <cmath>

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
		Wiwa::CameraManager& cm = Wiwa::SceneManager::getActiveScene()->GetCameraManager();
		m_Camera = cm.getActiveCamera();
		m_PlayerId = GameStateManager::GetPlayerId();

		if (m_PlayerId != WI_INVALID_INDEX)
		{
			m_PlayerTransformIt = GetComponentIterator<Transform3D>(m_PlayerId);
		}
		m_Camera->setRotation(m_CameraRotation);
	}

	void CameraController::OnUpdate()
	{
		if (!m_Camera)
			return;
		if (m_PlayerId == -1)
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

				
	}

	void CameraController::OnDestroy()
	{
		m_Camera = nullptr;
	}

}