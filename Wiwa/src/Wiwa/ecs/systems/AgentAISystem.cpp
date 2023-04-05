#include <wipch.h>

#include "AgentAISystem.h"
#include "Wiwa/ecs/components/AgentAI.h"
#include <Wiwa/AI/AIMapGeneration.h>
//#include "Wiwa/ecs/components/Transform3D.h"
#include <Wiwa/physics/PhysicsManager.h>
#include <glm/gtx/norm.hpp>
#include <glm/gtx/vector_angle.hpp> // for glm::angle()
#include "glew.h"
#include "PhysicsSystem.h"

Wiwa::AgentAISystem::AgentAISystem()
{
	m_IsRotatingByTile = true;
}

Wiwa::AgentAISystem::~AgentAISystem()
{
}

void Wiwa::AgentAISystem::OnAwake()
{
	m_AgentAI = GetComponentIterator<Wiwa::AgentAI>();
	m_Transform = GetComponentIterator<Wiwa::Transform3D>();
}

void Wiwa::AgentAISystem::OnInit()
{
	Wiwa::AgentAI* agent = GetComponentByIterator<Wiwa::AgentAI>(m_AgentAI);
	if (agent)
	{
		agent->hasArrived = false;
		agent->hasPath = false;
	}
}

void Wiwa::AgentAISystem::OnUpdate()
{
	if (!getAwake())
		System::Awake();
	if (!getInit())
		System::Init();
	Wiwa::AgentAI* agent = GetComponentByIterator<Wiwa::AgentAI>(m_AgentAI);
	Wiwa::Transform3D* transform = GetComponentByIterator<Wiwa::Transform3D>(m_Transform);

	if (lastPath.empty() == false && m_IsMoving == false)
	{
		GoToNextPosition();

		if (lastPath.empty())
		{
			agent->hasPath = false;
			agent->hasArrived = true;
		}
	}

	Wiwa::AIMapGeneration::MapData& localMapData = Wiwa::AIMapGeneration::GetMapData();
	float threshold = localMapData.tileWidth; // example threshold distance
	if (isNear(glm::vec2(transform->localPosition.x, transform->localPosition.z), m_DirectionPoint, threshold))
	{
		m_IsMoving = false;
	}

	glm::vec2 position = glm::vec2(transform->position.x, transform->position.z);
	
	float distance = glm::distance(position, m_DirectionPoint);

	if (m_IsMoving)
	{	
		// Calculate the time required to move the full distance at the given move speed
		float timeToMove = distance / agent->speed;

		// Calculate the interpolation factor based on the elapsed time and the time required to move
		float t = glm::clamp(Time::GetDeltaTimeSeconds() / timeToMove, 0.0f, 1.0f);
		
		// Interpolate the character's position between the current position and the target position using the interpolation factor
		glm::vec2 interpolatedPosition = glm::mix(position, m_DirectionPoint, t);

		transform->localPosition.x = interpolatedPosition.x;
		transform->localPosition.z = interpolatedPosition.y;

		if (m_IsRotatingByTile)
			LookAtPosition(m_DirectionPoint);
	}
	
	
	RotateAgent(distance, *agent, position, transform);
	

	Camera* camera = Wiwa::SceneManager::getActiveScene()->GetCameraManager().editorCamera;

	glViewport(0, 0, camera->frameBuffer->getWidth(), camera->frameBuffer->getHeight());
	camera->frameBuffer->Bind(false);

	glMatrixMode(GL_PROJECTION);
	glLoadMatrixf(glm::value_ptr(camera->getProjection()));
	glMatrixMode(GL_MODELVIEW);
	glLoadMatrixf(glm::value_ptr(camera->getView()));
	
	Wiwa::AIMapGeneration::MapData& data = Wiwa::AIMapGeneration::GetMapData();

	for (int i = 0; i < lastPath.size(); i++)
	{
		glm::vec2 vec = lastPath.at(i);
		glColor3f(0, 1, 0);
		glPolygonMode(GL_FRONT_AND_BACK, GL_3D); // Set the polygon mode to wireframe
		glBegin(GL_QUADS); // Begin drawing the quad
		glVertex3f(vec.x, 0.5f, vec.y + data.tileHeight); // Bottom-left vertex
		glVertex3f(vec.x + data.tileWidth, 0.5f, vec.y + data.tileHeight); // Bottom-right vertex
		glVertex3f(vec.x + data.tileWidth, 0.5f, vec.y); // Top-right vertex
		glVertex3f(vec.x, 0.5f, vec.y); // Top-left vertex
		
		glEnd();
		glPolygonMode(GL_FRONT_AND_BACK, GL_FILL); // Set the polygon mode back to fill
	}
	
	camera->frameBuffer->Unbind();
}

void Wiwa::AgentAISystem::OnDestroy()
{
}

bool Wiwa::AgentAISystem::CreatePath(const glm::vec3& targetPos)
{
	Wiwa::Transform3D* transform = GetComponentByIterator<Wiwa::Transform3D>(m_Transform);
	Wiwa::AgentAI* agent = GetComponentByIterator<Wiwa::AgentAI>(m_AgentAI);

	glm::ivec2 currentPositionMap = Wiwa::AIMapGeneration::WorldToMap(transform->position.x, transform->position.z);
	glm::ivec2 targetInMap = Wiwa::AIMapGeneration::WorldToMap(targetPos.x, targetPos.z);
	
	auto start_time = std::chrono::high_resolution_clock::now();
	int check = Wiwa::AIPathFindingManager::CreatePath(currentPositionMap, targetInMap);
	auto end_time = std::chrono::high_resolution_clock::now();
	// Calculate the elapsed time
	auto elapsed_time = std::chrono::duration_cast<std::chrono::milliseconds>(end_time - start_time).count();
	WI_INFO("Pathfinding time: {}", elapsed_time);

	if (check != -1)
	{
		lastPath = Wiwa::AIPathFindingManager::GetLastPath();
		return true;
	}
	agent->hasPath = true;
	agent->hasArrived = false;
	return false;
}

void Wiwa::AgentAISystem::GoToNextPosition()
{
	nextPos = lastPath.back();
	lastPath.pop_back();

	Wiwa::AIMapGeneration::MapData& localMapData = Wiwa::AIMapGeneration::GetMapData();
	m_DirectionPoint = { nextPos.x + (localMapData.tileWidth * 0.5f), nextPos.y + (localMapData.tileHeight * 0.5f) };

	m_IsMoving = true;
}


bool Wiwa::AgentAISystem::HasArrivedNextPosition(const glm::vec2& next_position, const glm::vec2& current_position)
{
	return (next_position == current_position);
}

bool Wiwa::AgentAISystem::isNear(glm::vec2 point1, glm::vec2 point2, float threshold)
{
	return glm::distance2(point1, point2) < threshold * threshold;
}

void Wiwa::AgentAISystem::StopMoving()
{
	lastPath.clear();
}

bool Wiwa::AgentAISystem::HasPath()
{
	if(lastPath.empty())
	{
		return false;
	}
	else
	{
		return true;
	}	
}

void Wiwa::AgentAISystem::LookAtPosition(glm::vec2 direction_point)
{
	m_RotDirectionPoint = direction_point;
}

void Wiwa::AgentAISystem::RotateAgent(const float distance, const AgentAI& agent, const glm::vec2& position, Transform3D* transform)
{
	float timeToRotate = distance / agent.angularSpeed;
	float tRot = glm::clamp(Time::GetDeltaTimeSeconds() / timeToRotate, 0.0f, 1.0f);
	// Calculate the forward vector from the current position to the target position
	glm::vec2 forward = glm::normalize(m_RotDirectionPoint - position);
	// Calculate the angle between the current forward vector and the target forward vector
	float angle = glm::angle(forward, { 0.0f, 1.0f });
	if (forward.x < 0.0f) {
		angle = (-angle);
	}

	float targetRotation = angle * 180 / glm::pi<float>();
	float interpolatedRotation = glm::mix(transform->localRotation.y, targetRotation, tRot);

	transform->localRotation.y = interpolatedRotation;
}
