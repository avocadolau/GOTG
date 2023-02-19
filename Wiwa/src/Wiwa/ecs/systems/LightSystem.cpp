#include <wipch.h>
#include "LightSystem.h"
#include <Wiwa\utilities\render\LightManager.h>
#include <Wiwa\scene\SceneManager.h>
#include <Wiwa\scene\Scene.h>
#include <Wiwa\utilities\render\LightManager.h>

Wiwa::LightSystem::LightSystem()
{
	
}

Wiwa::LightSystem::~LightSystem()
{
	if (GetComponent<PointLight>())
	{
		SceneManager::getActiveScene()->GetLightManager().RemovePointLight(m_EntityId);
	}
	else if (GetComponent<DirectionalLight>())
	{
		SceneManager::getActiveScene()->GetLightManager().SetDirectionalLight(-1);
	}
	else if (GetComponent<SpotLight>())
	{
		SceneManager::getActiveScene()->GetLightManager().RemoveSpotLight(m_EntityId);
	}
}

void Wiwa::LightSystem::OnEntitySet()
{
	if (GetComponent<PointLight>())
	{
		SceneManager::getActiveScene()->GetLightManager().AddPointLight(m_EntityId);
	}
	else if (GetComponent<DirectionalLight>())
	{
		SceneManager::getActiveScene()->GetLightManager().SetDirectionalLight(m_EntityId);
	}
	else if (GetComponent<SpotLight>())
	{
		SceneManager::getActiveScene()->GetLightManager().AddSpotLight(m_EntityId);
	}
}

