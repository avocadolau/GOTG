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

#include <random>
//#include <glm/gtc/quaternion.hpp>
//#include <glm/gtx/matrix_decompose.hpp>
//#include <glm/gtc/type_ptr.hpp>
//#include <vector>
#include "glew.h"
//#include <Wiwa/utilities/json/JSONDocument.h>

Wiwa::AIMapGeneration::MapData Wiwa::AIMapGeneration::m_MapData = MapData();
unsigned char* Wiwa::AIMapGeneration::m_Map = nullptr;

bool Wiwa::AIMapGeneration::CreateWalkabilityMap(int width, int height, float tileWidth, float tileHeight, glm::vec3 startPos)
{
	bool ret = false;

	m_MapData.startingPosition = startPos;
	m_MapData.width = width;
	m_MapData.height = height;
	m_MapData.tileHeight = tileHeight;
	m_MapData.tileWidth = tileWidth;

	if (tileHeight < 1.f) tileHeight = 1.f;

	if (tileWidth < 1.f) tileWidth = 1.f;	

	delete[] m_Map;
	m_Map = nullptr;
	m_Map = new unsigned char[width * height]; // string that store tiles, this can vary as we want
	memset(m_Map, 1, width * height);
	AIPathFindingManager::SetMap(width, height, m_Map);
	ret = true;

	return ret;
}

void Wiwa::AIMapGeneration::BakeMap()
{
	Wiwa::EntityManager& em = Wiwa::SceneManager::getActiveScene()->GetEntityManager();
	std::vector<EntityId>* entityList = em.GetEntitiesAlive();
	int size = entityList->size();
	for (int i = 0; i < size; i++)
	{
		EntityId actualId = entityList->at(i);
		if (em.HasComponent<Wiwa::CollisionBody>(actualId))
		{
			Wiwa::CollisionBody* cb = em.GetComponent<Wiwa::CollisionBody>(actualId);
			
			if (!cb->isStatic)
				continue;

			if (cb)
			{
				if (em.HasComponent<Wiwa::ColliderCube>(actualId))
				{
					ColliderCube* cube = em.GetComponent<ColliderCube>(actualId);
					glm::vec3& pos = em.GetComponent<Wiwa::Transform3D>(actualId)->localPosition;

					glm::vec2 topLeft = glm::vec2(pos.x - cube->halfExtents.x, pos.z + cube->halfExtents.z);
					glm::vec2 bottomLeft = glm::vec2(pos.x - cube->halfExtents.x, pos.z - cube->halfExtents.z);
					glm::vec2 bottomRight = glm::vec2(pos.x + cube->halfExtents.x, pos.z - cube->halfExtents.z);
					glm::vec2 topRight = glm::vec2(pos.x + cube->halfExtents.x, pos.z + cube->halfExtents.z);

					glm::ivec2 topLeftMap = Wiwa::AIMapGeneration::WorldToMap(topLeft.x, topLeft.y);
					glm::ivec2 bottomLeftMap = Wiwa::AIMapGeneration::WorldToMap(bottomLeft.x, bottomLeft.y);
					glm::ivec2 bottomRightMap = Wiwa::AIMapGeneration::WorldToMap(bottomRight.x, bottomRight.y);
					glm::ivec2 topRightMap = Wiwa::AIMapGeneration::WorldToMap(topRight.x, topRight.y);

					for (int i = bottomLeftMap.y; i <= topLeftMap.y; i++)
					{
						for (int j = topLeftMap.x; j <= topRightMap.x; j++)
						{
							SetPositionUnWalkable(glm::ivec2(j, i));
						}
					}

				}
				else if (em.HasComponent<Wiwa::ColliderSphere>(actualId))
				{
					ColliderSphere* sphere = em.GetComponent<ColliderSphere>(actualId);
				}
				else if (em.HasComponent<Wiwa::ColliderCylinder>(actualId))
				{
					ColliderCylinder* cylinder = em.GetComponent<ColliderCylinder>(actualId);
				}
				else if (em.HasComponent<Wiwa::ColliderCapsule>(actualId))
				{
					ColliderCapsule* capsule = em.GetComponent<ColliderCapsule>(actualId);
				}
			}
		}
		else
			continue;
	}
	AIPathFindingManager::SetMap(m_MapData.width, m_MapData.height, m_Map);
}

void Wiwa::AIMapGeneration::SetPositionUnWalkable(glm::ivec2 vec)
{
	//m_Map[vec.x * m_MapData.width + vec.y] = INVALID_WALK_CODE;
	m_Map[(vec.y * m_MapData.width) + vec.x] = INVALID_WALK_CODE;
}

void Wiwa::AIMapGeneration::DebugDrawMap()
{
	Camera* camera = Wiwa::SceneManager::getActiveScene()->GetCameraManager().editorCamera;

	glViewport(0, 0, camera->frameBuffer->getWidth(), camera->frameBuffer->getHeight());
	camera->frameBuffer->Bind(false);

	glMatrixMode(GL_PROJECTION);
	glLoadMatrixf(glm::value_ptr(camera->getProjection()));
	glMatrixMode(GL_MODELVIEW);
	glLoadMatrixf(glm::value_ptr(camera->getView()));

	unsigned char* map = Wiwa::AIMapGeneration::GetMap();
	for (int i = 0; i < m_MapData.height; i++)
	{
		for (int j = 0; j < m_MapData.width; j++)
		{
			glm::vec2 vec = Wiwa::AIMapGeneration::MapToWorld(i, j);
			if (map[j * m_MapData.width + i] == 255)
			{
				glColor4f(1, 0, 0, 0.1f);
			}
			else
			{
				glColor4f(0, 0, 1, 0.1f);
			}
			glPolygonMode(GL_FRONT_AND_BACK, GL_FILL); // Set the polygon mode to wireframe
			glBegin(GL_QUADS); // Begin drawing the quad   
			glVertex3f(vec.x, 0.0f, vec.y + m_MapData.tileHeight); // Bottom-left vertex
			glVertex3f(vec.x + m_MapData.tileWidth, 0.0f, vec.y + m_MapData.tileHeight); // Bottom-right vertex
			glVertex3f(vec.x + m_MapData.tileWidth, 0.0f, vec.y); // Top-right vertex
			glVertex3f(vec.x, 0.0f, vec.y); // Top-left vertex
			glEnd();

			glColor3f(0.5, 0.5, .5);
			glPolygonMode(GL_FRONT_AND_BACK, GL_LINE); // Set the polygon mode to wireframe
			glBegin(GL_QUADS); // Begin drawing the quad   
			glVertex3f(vec.x, 0.0f, vec.y + m_MapData.tileHeight); // Bottom-left vertex
			glVertex3f(vec.x + m_MapData.tileWidth, 0.0f, vec.y + m_MapData.tileHeight); // Bottom-right vertex
			glVertex3f(vec.x + m_MapData.tileWidth, 0.0f, vec.y); // Top-right vertex
			glVertex3f(vec.x, 0.0f, vec.y); // Top-left vertex
			glEnd();
			glPolygonMode(GL_FRONT_AND_BACK, GL_FILL); // Set the polygon mode back to fill
		}
	}
	glEnd();
	camera->frameBuffer->Unbind();
}

glm::vec2 Wiwa::AIMapGeneration::MapToWorld(int x, int y)
{
	glm::vec2 ret;

	ret.x = (float)x * m_MapData.tileWidth;
	ret.y = (float)y * m_MapData.tileHeight;
	//WI_CORE_INFO("Map to World: x = {}, y = {}, ret.x = {}, ret.y = {}", x, y, ret.x, ret.y);
	return ret;
}

glm::ivec2 Wiwa::AIMapGeneration::WorldToMap(float x, float y)
{
	glm::ivec2 ret;

	ret.x = x / m_MapData.tileWidth;
	ret.y = y / m_MapData.tileHeight;
	//WI_CORE_INFO("World to Map: x = {}, y = {}, ret.x = {}, ret.y = {}", x,y,ret.x, ret.y);
	return ret;
}

bool Wiwa::AIMapGeneration::OnSave()
{
	Wiwa::EntityManager& em = Wiwa::SceneManager::getActiveScene()->GetEntityManager();

	// Check if entity already exists
	std::string name = "tiles_map_data";
	std::vector<EntityId>* ent = em.GetEntitiesAlive();
	size_t count = em.GetEntityCount();
	bool hasMapData = false;
	for (int i = 0; i < count; i++)
	{
		size_t id = ent->at(i);
		std::string entName(em.GetEntityName(id));
		if (name == entName)
		{
			SaveMapData(id);
			hasMapData = true;
			return true;
		}
	}

	if (!hasMapData)
	{
		CreateNewEntityAndSaveData();
		return true;
	}
	return false;
}

bool Wiwa::AIMapGeneration::OnLoad()
{
	Wiwa::EntityManager& em = Wiwa::SceneManager::getActiveScene()->GetEntityManager();

	// Check if entity already exists
	const char* name = "tiles_map_data";
	std::vector<EntityId>* ent = em.GetEntitiesAlive();
	size_t count = em.GetEntityCount();
	bool hasMapData = false;
	for (int i = 0; i < count; i++)
	{
		size_t id = ent->at(i);
		std::string entName(em.GetEntityName(id));
		if (name != entName)
			continue;

		if (em.HasComponent<Wiwa::MapAI>(id))
		{
			LoadMapData(id);
			hasMapData = true;
		}
		return true;
	}
	return false;
}

bool Wiwa::AIMapGeneration::ClearMap()
{
	CreateWalkabilityMap(m_MapData.width, m_MapData.height, m_MapData.tileWidth, m_MapData.tileHeight, m_MapData.startingPosition);
	return true;
}

void Wiwa::AIMapGeneration::LoadMapData(size_t id)
{
	Wiwa::EntityManager& em = Wiwa::SceneManager::getActiveScene()->GetEntityManager();
	Wiwa::MapAI* data = em.GetComponent<Wiwa::MapAI>(id);
	m_MapData.startingPosition = data->startingPosition;
	m_MapData.width = data->width;
	m_MapData.height = data->height;
	m_MapData.tileWidth = data->tileWidth;
	m_MapData.tileHeight = data->tileHeight;

	CreateWalkabilityMap(m_MapData.width, m_MapData.height, m_MapData.tileWidth, m_MapData.tileHeight, m_MapData.startingPosition);

	//strcpy(reinterpret_cast<char*>(m_Map), reinterpret_cast<const char*>(data->map)); // OK
	memcpy(m_Map, &data->map, m_MapData.width * m_MapData.height);
	AIPathFindingManager::SetMap(m_MapData.width, m_MapData.height, m_Map);
}

void Wiwa::AIMapGeneration::SaveMapData(size_t id)
{
	Wiwa::EntityManager& em = Wiwa::SceneManager::getActiveScene()->GetEntityManager();
	Wiwa::MapAI* data = em.GetComponent<Wiwa::MapAI>(id);
	data->startingPosition = m_MapData.startingPosition;
	data->width = m_MapData.width;
	data->height = m_MapData.height;
	data->tileWidth = m_MapData.tileWidth;
	data->tileHeight = m_MapData.tileHeight;
	//strcpy(reinterpret_cast<char*>(data->map), reinterpret_cast<const char*>(m_Map)); // OK
	memset(data->map, 1, MAP_TILES_MAX_SIZE);
	memcpy(data->map, m_Map, m_MapData.width * m_MapData.height);
	AIPathFindingManager::SetMap(m_MapData.width, m_MapData.height, m_Map);

}

void Wiwa::AIMapGeneration::CreateNewEntityAndSaveData()
{
	const char* name = "tiles_map_data";
	Wiwa::EntityManager& em = Wiwa::SceneManager::getActiveScene()->GetEntityManager();
	size_t newId = em.CreateEntity(name);
	MapAI newMap;
	newMap.startingPosition = m_MapData.startingPosition;
	newMap.width = m_MapData.width;
	newMap.height = m_MapData.height;
	newMap.tileWidth = m_MapData.tileWidth;
	newMap.tileHeight = m_MapData.tileHeight;
	memset(newMap.map, 1, MAP_TILES_MAX_SIZE);
	memcpy(newMap.map, m_Map, m_MapData.width * m_MapData.height);
	//strcpy(reinterpret_cast<char*>(newMap.map), reinterpret_cast<const char*>(m_Map)); // OK
	em.AddComponent<Wiwa::MapAI>(newId, newMap);
	em.AddComponent<Wiwa::Transform3D>(newId);
	AIPathFindingManager::SetMap(m_MapData.width, m_MapData.height, m_Map);

}
