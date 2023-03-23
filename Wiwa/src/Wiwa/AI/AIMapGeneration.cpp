#include <wipch.h>
#include "AIMapGeneration.h"
#include "AIPathFindingManager.h"
//#include "Wiwa/ecs/systems/System.h"
//#include "Wiwa/ecs/EntityManager.h"
//#include "Wiwa/scene/SceneManager.h"
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
	m_mapData.tileHeight = (int)tileHeight;
	m_mapData.tileWidth = (int)tileWidth;

	m_map = new unsigned char[width * height]; // string that store tiles, this can vary as we want
	memset(m_map, 1, width * height);
	AIPathFindingManager::SetMap(width, height, m_map);

	for (int z = 0; z < m_mapData.height; z++)
	{
		for (int x = 0; x < m_mapData.width; x++)
		{
			// if collision then invalid walking code

			// if not collision then default walking code
			m_map[x * m_mapData.width + z] = DEFAULT_WALK_CODE;
		}
	}

	ret = true;

	return ret;
}

glm::ivec2 Wiwa::AIMapGeneration::MapToWorld(int x, int y)
{
	glm::ivec2 ret;

	ret.x = x * m_mapData.tileWidth;
	ret.y = y * m_mapData.tileHeight;

	return ret;
}

glm::ivec2 Wiwa::AIMapGeneration::WorldToMap(int x, int y)
{
	glm::ivec2 ret;

	ret.x = x / m_mapData.tileWidth;
	ret.y = y / m_mapData.tileHeight;

	return ret;
}
