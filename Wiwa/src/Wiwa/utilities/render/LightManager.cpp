#include <wipch.h>
#include "LightManager.h"

Wiwa::LightManager::LightManager()
{
}

Wiwa::LightManager::~LightManager()
{
}

void Wiwa::LightManager::AddPointLight(size_t light)
{
	m_PointLights.push_back(light);
}

void Wiwa::LightManager::RemovePointLight(size_t light)
{
	for (size_t i = 0; i < m_PointLights.size(); i++)
	{
		if (m_PointLights[i] == light)
		{
			m_PointLights.erase(m_PointLights.begin() + i);
			break;
		}
	}
}

void Wiwa::LightManager::AddSpotLight(size_t light)
{
	m_SpotLights.push_back(light);
}

void Wiwa::LightManager::RemoveSpotLight(size_t light)
{
	for (size_t i = 0; i < m_SpotLights.size(); i++)
	{
		if (m_SpotLights[i] == light)
		{
			m_SpotLights.erase(m_SpotLights.begin() + i);
			break;
		}
	}
}
