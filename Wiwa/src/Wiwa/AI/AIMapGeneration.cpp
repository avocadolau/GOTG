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
#include <Wiwa/ecs/systems/PhysicsSystem.h>

Wiwa::AIMapGeneration::MapData Wiwa::AIMapGeneration::m_MapData = MapData();
//unsigned char* Wiwa::AIMapGeneration::m_Map = nullptr;
std::vector<unsigned char> Wiwa::AIMapGeneration::m_Map = std::vector<unsigned char>(MAP_TILES_MAX_SIZE);
glm::vec2 Wiwa::AIMapGeneration::topLeft = glm::vec2(0.0f);
glm::vec2 Wiwa::AIMapGeneration::bottomLeft = glm::vec2(0.0f);
glm::vec2 Wiwa::AIMapGeneration::bottomRight = glm::vec2(0.0f);
glm::vec2 Wiwa::AIMapGeneration::topRight = glm::vec2(0.0f);
float Wiwa::AIMapGeneration::minX = 0;
float Wiwa::AIMapGeneration::minY = 0;
float Wiwa::AIMapGeneration::maxX = 0;
float Wiwa::AIMapGeneration::maxY = 0;

bool Wiwa::AIMapGeneration::CreateWalkabilityMap(int width, int height, float tileWidth, float tileHeight, glm::vec2 startPos)
{
	bool ret = false;

	m_MapData.startingPosition = startPos;
	m_MapData.width = width;
	m_MapData.height = height;
	m_MapData.tileHeight = tileHeight;
	m_MapData.tileWidth = tileWidth;

	if (tileHeight < 1.f) tileHeight = 1.f;

	if (tileWidth < 1.f) tileWidth = 1.f;	

	m_Map.clear();
	m_Map.assign(width * height, DEFAULT_WALK_CODE);
	/*if (m_Map != nullptr)
	{
		delete[] m_Map;
		m_Map = nullptr;
	}*/
	//
	//m_Map = new unsigned char[width * height]; // string that store tiles, this can vary as we want
	//memset(m_Map, 1, width * height);
	AIPathFindingManager::SetMap(width, height, m_Map);
	ret = true;

	return ret;
}

glm::vec2 rotateVec2(glm::vec2 point, float angle, glm::vec2 origin)
{
	float s = sin(angle);
	float c = cos(angle);

	// translate point back to origin
	point -= origin;

	// rotate point
	glm::vec2 rotatedPoint;
	rotatedPoint.x = point.x * c - point.y * s;
	rotatedPoint.y = point.x * s + point.y * c;

	// translate point back to its original position
	rotatedPoint += origin;

	return rotatedPoint;
}

glm::vec2 scaleVec2(glm::vec2 point, glm::vec2 scaling, glm::vec2 origin)
{
	// translate point back to origin
	point -= origin;

	// apply scaling
	point.x *= scaling.x;
	point.y *= scaling.y;

	// translate point back to its original position
	point += origin;

	return point;
}


glm::vec2 scaleRectVertex(glm::vec2 rectPos, glm::vec2 halfExtents, glm::vec2 scaling, glm::vec2 localPos) {
	glm::vec2 scaledPos = glm::vec2(rectPos.x + localPos.x * halfExtents.x * scaling.x,
		rectPos.y + localPos.y * halfExtents.y * scaling.y);
	return scaledPos;
}

glm::vec2 rotateRectVertex(const glm::vec2& rectPos, const glm::vec2& halfExtents, const glm::vec2& scaling, float rotation, const glm::vec2& vertex)
{
	glm::vec2 scaledVertex = vertex * scaling;
	glm::vec2 rotatedVertex;
	rotatedVertex.x = scaledVertex.x * cos(rotation) - scaledVertex.y * sin(rotation);
	rotatedVertex.y = scaledVertex.x * sin(rotation) + scaledVertex.y * cos(rotation);
	glm::vec2 worldVertex = rectPos + rotatedVertex * halfExtents;
	return worldVertex;
}


glm::vec2 rotate_and_scale_vertex(const glm::vec2& vertex, const glm::vec2& rect_pos, const glm::vec2& rect_half_extents, const glm::vec2& rect_scale, float rect_rotation)
{
	// Translate the vertex to the rect position.
	glm::vec2 translated_vertex = vertex - rect_pos;

	// Apply the rect rotation.
	float cos_theta = glm::cos(glm::radians(rect_rotation));
	float sin_theta = glm::sin(glm::radians(rect_rotation));
	glm::mat2 rotation_matrix(cos_theta, -sin_theta, sin_theta, cos_theta);
	glm::vec2 rotated_vertex = rotation_matrix * translated_vertex;

	// Scale the vertex based on the rect scale.
	glm::vec2 scaled_vertex = rotated_vertex / rect_scale;

	// Apply the rect half extents to the vertex to keep it within the rect bounds.
	scaled_vertex.x = glm::clamp(scaled_vertex.x, -rect_half_extents.x, rect_half_extents.x);
	scaled_vertex.y = glm::clamp(scaled_vertex.y, -rect_half_extents.y, rect_half_extents.y);

	// Translate the vertex back to its original position.
	glm::vec2 final_vertex = scaled_vertex + rect_pos;

	return final_vertex;
}


bool pointInRotatedRect(const glm::vec2& tileCenter, const glm::vec2& rectPos, const glm::vec2& halfExtents, const glm::vec2& scaling, float rotation)
{
	//// First, we need to rotate the tile center by the negative angle of the rectangle
	//glm::vec2 rotatedTileCenter = rotateVec2(tileCenter, -rotation, rectPos);

	//// Next, we need to scale the rotated tile center based on the scaling vector
	//glm::vec2 scaledTileCenter = scaleVec2(rotatedTileCenter, scaling, rectPos);

	glm::vec2 tileC = rotate_and_scale_vertex(tileCenter, rectPos, halfExtents, scaling, rotation);

	// Calculate the minimum and maximum x and y coordinates that the rectangle encompasses
	float minX = rectPos.x - halfExtents.x;
	float maxX = rectPos.x + halfExtents.x;
	float minY = rectPos.y - halfExtents.y;
	float maxY = rectPos.y + halfExtents.y;

	// Check if the scaled and rotated tile center is inside the rectangle
	bool isInsideX = tileC.x >= minX && tileC.x <= maxX;
	bool isInsideY = tileC.y >= minY && tileC.y <= maxY;

	return isInsideX && isInsideY;
}

bool Wiwa::AIMapGeneration::BakeMap()
{
	if (m_Map.empty())
		return false;

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
					BakeCube(*cb, *cube, actualId, em);
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
	return true;
}

bool Wiwa::AIMapGeneration::BakeCube(const CollisionBody& body, const ColliderCube& cube, EntityId actualId, EntityManager& em)
{
	glm::vec3& pos = em.GetComponent<Wiwa::Transform3D>(actualId)->localPosition;
	glm::vec3& rot = em.GetComponent<Wiwa::Transform3D>(actualId)->localRotation;
	btTransform& t = em.GetSystem<Wiwa::PhysicsSystem>(actualId)->getBody()->collisionObject->getWorldTransform();

	glm::mat4 mat(1.0f);
	t.getOpenGLMatrix(glm::value_ptr(mat));

	glm::vec2 rectPos = { pos.x,pos.z };
	glm::vec2 halfExtents = { cube.halfExtents.x, cube.halfExtents.z };
	glm::vec2 scaling = { body.scalingOffset.x, body.scalingOffset.z };

	//topLeft = rectPos - halfExtents;
	//bottomLeft = rectPos + glm::vec2(-halfExtents.x, halfExtents.y);
	//bottomRight = rectPos + halfExtents;
	//topRight = rectPos + glm::vec2(halfExtents.x, -halfExtents.y);

	/*topLeft = scaleRectVertex(rectPos, halfExtents, scaling, glm::vec2(-1, 1));
	bottomLeft = scaleRectVertex(rectPos, halfExtents, scaling, glm::vec2(-1, -1));
	bottomRight = scaleRectVertex(rectPos, halfExtents, scaling, glm::vec2(1, -1));
	topRight = scaleRectVertex(rectPos, halfExtents, scaling, glm::vec2(1, 1));*/
	
	// Calculate the corners of the rotated rectangle in world space
	topLeft = rotateRectVertex(rectPos, halfExtents, scaling, glm::radians(rot.y), glm::vec2(-1, 1));
	bottomLeft = rotateRectVertex(rectPos, halfExtents, scaling, glm::radians(rot.y), glm::vec2(-1, -1));
	bottomRight = rotateRectVertex(rectPos, halfExtents, scaling, glm::radians(rot.y), glm::vec2(1, -1));
	topRight = rotateRectVertex(rectPos, halfExtents, scaling, glm::radians(rot.y), glm::vec2(1, 1));

	//glm::vec2 topLeft = glm::vec2(pos.x - cube.halfExtents.x, pos.z + cube.halfExtents.z);
	//glm::vec2 bottomLeft = glm::vec2(pos.x - cube.halfExtents.x, pos.z - cube.halfExtents.z);
	//glm::vec2 bottomRight = glm::vec2(pos.x + cube.halfExtents.x, pos.z - cube.halfExtents.z);
	//glm::vec2 topRight = glm::vec2(pos.x + cube.halfExtents.x, pos.z + cube.halfExtents.z);

	glm::ivec2 topLeftMap = Wiwa::AIMapGeneration::WorldToMap(topLeft.x, topLeft.y);
	glm::ivec2 bottomLeftMap = Wiwa::AIMapGeneration::WorldToMap(bottomLeft.x, bottomLeft.y);
	glm::ivec2 bottomRightMap = Wiwa::AIMapGeneration::WorldToMap(bottomRight.x, bottomRight.y);
	glm::ivec2 topRightMap = Wiwa::AIMapGeneration::WorldToMap(topRight.x, topRight.y);

	//DrawRect(topLeft, bottomLeft, bottomRight, topRight);

	//for (float i = bottomLeft.y; i <= topLeft.y; i++)
	//{
	//	for (float j = topLeft.x; j <= topRight.x; j++)
	//	{
	//		SetPositionUnWalkable(Wiwa::AIMapGeneration::WorldToMap(i, j));
	//	}
	//}

	for (int i = bottomLeftMap.y; i <= topLeftMap.y; i++)
	{
		for (int j = topLeftMap.x; j <= topRightMap.x; j++)
		{
			SetPositionUnWalkable(glm::ivec2(i, j));
		}
	}
	//// Calculate the corners of the rotated rectangle in world space
	//glm::vec2 topLeft = rotateRectVertex(rectPos, halfExtents, scaling, glm::radians(rot.y), glm::vec2(-1, 1));
	//glm::vec2 bottomLeft = rotateRectVertex(rectPos, halfExtents, scaling, glm::radians(rot.y), glm::vec2(-1, -1));
	//glm::vec2 bottomRight = rotateRectVertex(rectPos, halfExtents, scaling, glm::radians(rot.y), glm::vec2(1, -1));
	//glm::vec2 topRight = rotateRectVertex(rectPos, halfExtents, scaling, glm::radians(rot.y), glm::vec2(1, 1));

	//// Convert the corners to tile coordinates
	//glm::ivec2 topLeftTile = Wiwa::AIMapGeneration::WorldToMap(topLeft.x, topLeft.y);
	//glm::ivec2 bottomLeftTile = Wiwa::AIMapGeneration::WorldToMap(bottomLeft.x, bottomLeft.y);
	//glm::ivec2 bottomRightTile = Wiwa::AIMapGeneration::WorldToMap(bottomRight.x, bottomRight.y);
	//glm::ivec2 topRightTile = Wiwa::AIMapGeneration::WorldToMap(topRight.x, topRight.y);

	//// Determine the minimum and maximum x and y tile indices
	//minX = std::min({ topLeft.x, bottomLeft.x, bottomRight.x, topRight.x });
	//minY = std::min({ topLeft.y, bottomLeft.y, bottomRight.y, topRight.y });
	//maxX = std::max({ topLeft.x, bottomLeft.x, bottomRight.x, topRight.x });
	//maxY = std::max({ topLeft.y, bottomLeft.y, bottomRight.y, topRight.y });
	//// Loop over all tiles within the bounding box
	//for (float y = minY; y <= maxY; y++) {
	//	for (float x = minX; x <= maxX; x++) {
	//		SetPositionUnWalkable(WorldToMap(x,y));
	//		//// Calculate the center of the current tile in world space
	//		//glm::vec2 tileCenter = Wiwa::AIMapGeneration::MapToWorld(x, y);
	//		////glm::vec2 tileCenter = Wiwa::AIMapGeneration::MapToWorld(x, y) + glm::vec2(m_MapData.tileWidth / 2, m_MapData.tileHeight / 2);
	//		//SetPositionUnWalkable(glm::ivec2(x, y));

	//		//// Check if the tile is contained within the rotated rectangle
	//		//if (pointInRotatedRect(tileCenter, rectPos, halfExtents, scaling, glm::radians(rot.y))) {
	//		//	// Do something with the tile
	//		//	// ...

	//		//}
	//	}
	//}

	return false;
}

void Wiwa::AIMapGeneration::DrawRect()
{
	Camera* camera = Wiwa::SceneManager::getActiveScene()->GetCameraManager().editorCamera;

	glViewport(0, 0, camera->frameBuffer->getWidth(), camera->frameBuffer->getHeight());
	camera->frameBuffer->Bind(false);

	glMatrixMode(GL_PROJECTION);
	glLoadMatrixf(glm::value_ptr(camera->getProjection()));
	glMatrixMode(GL_MODELVIEW);
	glLoadMatrixf(glm::value_ptr(camera->getView()));

	glColor4f(0, 1, 0, 1);
	
	glPolygonMode(GL_FRONT_AND_BACK, GL_FILL); // Set the polygon mode to wireframe
	glBegin(GL_QUADS); // Begin drawing the quad   
	glVertex3f(bottomLeft.x, 2, bottomLeft.y); // Bottom-left vertex
	glVertex3f(bottomRight.x, 2, bottomRight.y); // Bottom-right vertex
	glVertex3f(topRight.x, 2, topRight.y); // Top-right vertex
	glVertex3f(topLeft.x, 2, topLeft.y); // Top-left vertex
	
	glEnd();
	camera->frameBuffer->Unbind();
}

void Wiwa::AIMapGeneration::DrawMinMaxRect()
{
	Camera* camera = Wiwa::SceneManager::getActiveScene()->GetCameraManager().editorCamera;

	glViewport(0, 0, camera->frameBuffer->getWidth(), camera->frameBuffer->getHeight());
	camera->frameBuffer->Bind(false);

	glMatrixMode(GL_PROJECTION);
	glLoadMatrixf(glm::value_ptr(camera->getProjection()));
	glMatrixMode(GL_MODELVIEW);
	glLoadMatrixf(glm::value_ptr(camera->getView()));

	glColor4f(1, 1, 0, 1);

	glPolygonMode(GL_FRONT_AND_BACK, GL_FILL); // Set the polygon mode to wireframe
	glBegin(GL_QUADS); // Begin drawing the quad   
	glVertex3f(minX, 4, minY); // Bottom-left vertex
	glVertex3f(maxX, 4, minY); // Bottom-right vertex
	glVertex3f(maxX, 4, maxY); // Top-right vertex
	glVertex3f(minX, 4, maxY); // Top-left vertex

	glEnd();
	camera->frameBuffer->Unbind();
}

void Wiwa::AIMapGeneration::SetPositionUnWalkable(glm::ivec2 vec)
{
	int index = vec.y * m_MapData.width + vec.x;
	if (index >= 0 && index < m_Map.size() && AIPathFindingManager::CheckBoundaries(vec))
	{
		m_Map[index] = INVALID_WALK_CODE;
	/*	WI_INFO("Index i: {}", index);
		WI_INFO("UnWalkable x: {} , y: {}", vec.x, vec.y);*/
	}
}

bool Wiwa::AIMapGeneration::DebugDrawMap()
{
	if (m_Map.empty())
		return false;

	Camera* camera = Wiwa::SceneManager::getActiveScene()->GetCameraManager().editorCamera;

	glViewport(0, 0, camera->frameBuffer->getWidth(), camera->frameBuffer->getHeight());
	camera->frameBuffer->Bind(false);

	glMatrixMode(GL_PROJECTION);
	glLoadMatrixf(glm::value_ptr(camera->getProjection()));
	glMatrixMode(GL_MODELVIEW);
	glLoadMatrixf(glm::value_ptr(camera->getView()));

	for (int i = 0; i < m_MapData.height; i++)
	{
		for (int j = 0; j < m_MapData.width; j++)
		{
			glm::vec2 vec = Wiwa::AIMapGeneration::MapToWorld(i, j);
			if (m_Map[i * m_MapData.width + j] == 255)
			{
				glColor4f(1, 0, 0, 0.1f);
			}
			else
			{
				glColor4f(0, 0, 1, 0.1f);
			}
			glPolygonMode(GL_FRONT_AND_BACK, GL_FILL); // Set the polygon mode to wireframe
			glBegin(GL_QUADS); // Begin drawing the quad   
			glVertex3f(vec.x, 0.5f, vec.y + m_MapData.tileHeight); // Bottom-left vertex
			glVertex3f(vec.x + m_MapData.tileWidth, 0.5f, vec.y + m_MapData.tileHeight); // Bottom-right vertex
			glVertex3f(vec.x + m_MapData.tileWidth, 0.5f, vec.y); // Top-right vertex
			glVertex3f(vec.x, 0.5f, vec.y); // Top-left vertex
			glEnd();

			glColor3f(0.5, 0.5, 0.5);
			glPolygonMode(GL_FRONT_AND_BACK, GL_LINE); // Set the polygon mode to wireframe
			glBegin(GL_QUADS); // Begin drawing the quad   
			glVertex3f(vec.x, 0.5f, vec.y + m_MapData.tileHeight); // Bottom-left vertex
			glVertex3f(vec.x + m_MapData.tileWidth, 0.5f, vec.y + m_MapData.tileHeight); // Bottom-right vertex
			glVertex3f(vec.x + m_MapData.tileWidth, 0.5f, vec.y); // Top-right vertex
			glVertex3f(vec.x, 0.5f, vec.y); // Top-left vertex
			glEnd();
			glPolygonMode(GL_FRONT_AND_BACK, GL_FILL); // Set the polygon mode back to fill
		}
	}
	glEnd();
	camera->frameBuffer->Unbind();

	return true;
}

glm::vec2 Wiwa::AIMapGeneration::MapToWorld(int x, int y)
{
	glm::vec2 ret;

	ret.x = (float)(x + m_MapData.startingPosition.x) * m_MapData.tileWidth;
	ret.y = (float)(y + m_MapData.startingPosition.y) * m_MapData.tileHeight;
	//WI_CORE_INFO("Map to World: x = {}, y = {}, ret.x = {}, ret.y = {}", x, y, ret.x, ret.y);
	return ret;
}

glm::ivec2 Wiwa::AIMapGeneration::WorldToMap(float x, float y)
{
	glm::ivec2 ret;

	ret.x = (x / m_MapData.tileWidth) - m_MapData.startingPosition.x;
	ret.y = (y / m_MapData.tileHeight) - m_MapData.startingPosition.y;
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

	if (!m_Map.empty() && !hasMapData && (m_Map[0] != '\0'))
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
	//CreateWalkabilityMap(m_MapData.width, m_MapData.height, m_MapData.tileWidth, m_MapData.tileHeight, m_MapData.startingPosition);
	/*if (m_Map != nullptr)
	{
		delete[] m_Map;
		m_Map = nullptr;
	}*/
	m_Map.clear();
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
	//memcpy(m_Map, &data->map, m_MapData.width * m_MapData.height * sizeof(unsigned char));
	//m_Map.assign(data->map, data->map + sizeof(data->map));
	std::copy(data->map, data->map + data->width * data->height, m_Map.begin());
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
	//memcpy(data->map, m_Map, m_MapData.width * m_MapData.height * sizeof(unsigned char));
	memset(data->map, 1, MAP_TILES_MAX_SIZE);
	std::copy(m_Map.begin(), m_Map.end(), data->map);
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
	//memset(newMap.map, 1, MAP_TILES_MAX_SIZE);
	//memcpy(newMap.map, m_Map, m_MapData.width * m_MapData.height * sizeof(unsigned char));
	//strcpy(reinterpret_cast<char*>(newMap.map), reinterpret_cast<const char*>(m_Map)); // OK
	memset(newMap.map, 1, MAP_TILES_MAX_SIZE);
	std::copy(m_Map.begin(), m_Map.end(), newMap.map);
	em.AddComponent<Wiwa::MapAI>(newId, newMap);
	em.AddComponent<Wiwa::Transform3D>(newId);
	AIPathFindingManager::SetMap(m_MapData.width, m_MapData.height, m_Map);

}
