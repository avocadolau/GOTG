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
		SceneManager::getActiveScene()->GetLightManager().RemoveLight(m_EntityId);
	}
	else if (GetComponent<DirectionalLight>())
	{
		SceneManager::getActiveScene()->GetLightManager().SetDirectionalLight(-1);
	}
}

void Wiwa::LightSystem::OnEntitySet()
{
	if (GetComponent<PointLight>())
	{
		SceneManager::getActiveScene()->GetLightManager().AddLight(m_EntityId);
	}
	else if (GetComponent<DirectionalLight>())
	{
		SceneManager::getActiveScene()->GetLightManager().SetDirectionalLight(m_EntityId);
	}
}

