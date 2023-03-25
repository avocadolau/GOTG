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

					for (int i = bottomLeftMap.y; i < topLeftMap.y; i++)
					{
						for (int j = topLeftMap.x; j < topRightMap.x; j++)
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
	AIPathFindingManager::SetMap(m_mapData.width, m_mapData.height, m_map);

}

void Wiwa::AIMapGeneration::SetPositionUnWalkable(glm::ivec2 vec)
{
	//m_map[vec.x * m_mapData.width + vec.y] = INVALID_WALK_CODE;
	m_map[(vec.y * m_mapData.width) + vec.x] = INVALID_WALK_CODE;
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
	for (int i = 0; i < m_mapData.height; i++)
	{
		for (int j = 0; j < m_mapData.width; j++)
		{
			glm::vec2 vec = Wiwa::AIMapGeneration::MapToWorld(i, j);
			if (map[j * m_mapData.width + i] == 255)
			{
				glColor4f(1, 0, 0, 0.1f);
			}
			else
			{
				glColor4f(0, 0, 1, 0.1f);
			}
			glPolygonMode(GL_FRONT_AND_BACK, GL_FILL); // Set the polygon mode to wireframe
			glBegin(GL_QUADS); // Begin drawing the quad   
			glVertex3f(vec.x, 0.0f, vec.y + m_mapData.tileHeight); // Bottom-left vertex
			glVertex3f(vec.x + m_mapData.tileWidth, 0.0f, vec.y + m_mapData.tileHeight); // Bottom-right vertex
			glVertex3f(vec.x + m_mapData.tileWidth, 0.0f, vec.y); // Top-right vertex
			glVertex3f(vec.x, 0.0f, vec.y); // Top-left vertex
			glEnd();

			glColor3f(0.5, 0.5, .5);
			glPolygonMode(GL_FRONT_AND_BACK, GL_LINE); // Set the polygon mode to wireframe
			glBegin(GL_QUADS); // Begin drawing the quad   
			glVertex3f(vec.x, 0.0f, vec.y + m_mapData.tileHeight); // Bottom-left vertex
			glVertex3f(vec.x + m_mapData.tileWidth, 0.0f, vec.y + m_mapData.tileHeight); // Bottom-right vertex
			glVertex3f(vec.x + m_mapData.tileWidth, 0.0f, vec.y); // Top-right vertex
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

	ret.x = (float)x * m_mapData.tileWidth;
	ret.y = (float)y * m_mapData.tileHeight;
	//WI_CORE_INFO("Map to World: x = {}, y = {}, ret.x = {}, ret.y = {}", x, y, ret.x, ret.y);
	return ret;
}

glm::ivec2 Wiwa::AIMapGeneration::WorldToMap(float x, float y)
{
	glm::ivec2 ret;

	ret.x = x / m_mapData.tileWidth;
	ret.y = y / m_mapData.tileHeight;
	//WI_CORE_INFO("World to Map: x = {}, y = {}, ret.x = {}, ret.y = {}", x,y,ret.x, ret.y);
	return ret;
}
