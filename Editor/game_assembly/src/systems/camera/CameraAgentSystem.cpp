#include "CameraAgentSystem.h"
#include "../../components/camera/CameraAgent.h"

void Wiwa::CameraAgentSystem::OnAwake()
{
	m_DataIt.c_id = WI_INVALID_INDEX;
}
void Wiwa::CameraAgentSystem::OnInit()
{
	m_DataIt = GetComponentIterator<CameraAgent>();
}
void Wiwa::CameraAgentSystem::OnUpdate()
{
	if (m_DataIt.c_id != WI_INVALID_INDEX)
	{
		m_DataIt = GetComponentIterator<CameraAgent>();
	}
	CameraAgent* data = nullptr;
	data = GetComponentByIterator<CameraAgent>(m_DataIt);

	if (data)
	{
		data->Position = Math::GetWorldPosition(GetTransform()->worldMatrix);
	}
}

bool Wiwa::CameraAgentSystem::OnEnabledFromPool()
{
	if (m_DataIt.c_id != WI_INVALID_INDEX)
	{
		m_DataIt = GetComponentIterator<CameraAgent>();
	}
	CameraAgent* data = nullptr;
	data = GetComponentByIterator<CameraAgent>(m_DataIt);

	if (data)
	{
		data->Active = true;
	}

	return true;
}

bool Wiwa::CameraAgentSystem::OnDisabledFromPool()
{
	if (m_DataIt.c_id != WI_INVALID_INDEX)
	{
		m_DataIt = GetComponentIterator<CameraAgent>();
	}
	CameraAgent* data = nullptr;
	data = GetComponentByIterator<CameraAgent>(m_DataIt);

	if (data)
	{
		data->Active = false;
	}

	return true;
}

