#include <wipch.h>
#include "AIMapGeneration.h"
#include "AIPathFindingManager.h"
//#include "Wiwa/ecs/systems/System.h"
#include "Wiwa/ecs/EntityManager.h"
#include "Wiwa/scene/SceneManager.h"
//#include "Wiwa/utilities/render/CameraManager.h"
//#include <glew.h>

#include <Wiwa/utilities/Log.h>
//#include "Wiwa/utilities/time/Time.h"

//#include <glm/gtc/quaternion.hpp>
//#include <glm/gtx/matrix_decompose.hpp>
//#include <glm/gtc/type_ptr.hpp>
//#include <vector>

//#include <Wiwa/utilities/json/JSONDocument.h>

Wiwa::AIMapGeneration::MapData Wiwa::AIMapGeneration::m_mapData = MapData();
unsigned char* Wiwa::AIMapGeneration::m_map = nullptr;

bool Wiwa::AIMapGeneration::CreateWalkabilityMap(int width, int height, float tileWidth, float tileHeight, float startPos)
{
	bool ret = false;

	m_mapData.startingPosition = startPos;
	m_mapData.width = width;
	m_mapData.height = height;
	m_mapData.tileHeight = tileHeight;
	m_mapData.tileWidth = tileWidth;

	if (tileHeight < 1.f) tileHeight = 1.f;

	if (tileWidth < 1.f) tileWidth = 1.f;	


	m_map = new unsigned char[width * height]; // string that store tiles, this can vary as we want
	memset(m_map, 1, width * height);
	AIPathFindingManager::SetMap(width, height, m_map);

	Wiwa::EntityManager& em = Wiwa::SceneManager::getActiveScene()->GetEntityManager();
	std::vector<EntityId>* entities = em.GetEntitiesAlive();
	int entityCount = em.GetEntityCount();
	
	for (int z = 0; z < m_mapData.height; z++)
	{
		for (int x = 0; x < m_mapData.width; x++)
		{
			for (int i = 0; i < entityCount; i++)
			{
				EntityId actualId = entities->at(i);

				
				if (em.GetComponent<Wiwa::CollisionBody>(actualId)->isStatic)
				{
					em.GetComponent<Wiwa::Transform3D>(actualId)->localPosition;
				}
			}

			// if collision then invalid walking code
			
			// if not collision then default walking code
			m_map[x * m_mapData.width + z] = DEFAULT_WALK_CODE;
		}
	}

	ret = true;

	return ret;
}

glm::vec2 Wiwa::AIMapGeneration::MapToWorld(int x, int y)
{
	glm::vec2 ret;

	ret.x = (float)x * m_mapData.tileWidth;
	ret.y = (float)y * m_mapData.tileHeight;

	return ret;
}

glm::ivec2 Wiwa::AIMapGeneration::WorldToMap(float x, float y)
{
	glm::ivec2 ret;

	ret.x = x / m_mapData.tileWidth;
	ret.y = y / m_mapData.tileHeight;

	return ret;
}
