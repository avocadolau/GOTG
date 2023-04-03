#include <wipch.h>

#include "AgentAISystem.h"
#include "Wiwa/ecs/components/AgentAI.h"
#include <Wiwa/AI/AIMapGeneration.h>
//#include "Wiwa/ecs/components/Transform3D.h"
#include <Wiwa/physics/PhysicsManager.h>
#include <glm/gtx/norm.hpp>
#include <glm/gtx/vector_angle.hpp> // for glm::angle()
#include "glew.h"

Wiwa::AgentAISystem::AgentAISystem()
{
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
	Wiwa::AgentAI* agent = GetComponentByIterator<Wiwa::AgentAI>(m_AgentAI);
	Wiwa::Transform3D* transform = GetComponentByIterator<Wiwa::Transform3D>(m_Transform);

	/*if (agent->hasPath == false)
	{
		if (CreatePath(agent->target))
		{
			agent->hasPath = true;
			agent->hasArrived = false;
		}
	}*/

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

	if (m_IsMoving)
	{
		glm::vec2 position = glm::vec2(transform->position.x, transform->position.z);
		// Calculate the distance between the current position and the target position
		float distance = glm::distance(position, m_DirectionPoint);

		// Calculate the time required to move the full distance at the given move speed
		float timeToMove = distance / agent->speed;
		

		// Calculate the interpolation factor based on the elapsed time and the time required to move
		float t = glm::clamp(Time::GetDeltaTimeSeconds() / timeToMove, 0.0f, 1.0f);
		
		// Interpolate the character's position between the current position and the target position using the interpolation factor
		glm::vec2 interpolatedPosition = glm::mix(position, m_DirectionPoint, t);

		// Interpolate the character's rotation to the target rotation using the interpolation factor
		
		/*if (targetRotation < 0.0f) {
			targetRotation += 2.0f * glm::pi<float>();
		}*/
		
		// Update the character's position and rotation to the interpolated position and rotation
		transform->localPosition.x = interpolatedPosition.x;
		transform->localPosition.z = interpolatedPosition.y;

		
		// Move to direction
		/*transform->localPosition.x += agent->speed * direction.x * Time::GetDeltaTimeSeconds();
		transform->localPosition.z += agent->speed * direction.y * Time::GetDeltaTimeSeconds();*/
		
		
		// Rotation Things:
		
		float timeToRotate = distance / agent->angularSpeed;
		
		float tRot = glm::clamp(Time::GetDeltaTimeSeconds() / timeToRotate, 0.0f, 1.0f);
		//
		//// Calculate the forward vector from the current position to the target position
		glm::vec2 forward = glm::normalize(m_DirectionPoint - position);

		//
		//// Calculate the angle between the current forward vector and the target forward vector
		float angle = glm::angle(forward, { 0.0f, 1.0f });
		if (forward.x < 0.0f) {
			angle = (-angle);
		}
		//
		float targetRotation = angle * 180 / glm::pi<float>();
		//
		float interpolatedRotation = glm::mix(transform->localRotation.y, targetRotation, tRot);

		//WI_INFO(" angle {}", angle);
		transform->localRotation.y = interpolatedRotation;
		//WI_INFO(" Interpolation Rotation {}", interpolatedRotation);
		//WI_INFO(" target Rotation {}", targetRotation);

		
	}

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
	
	int check = Wiwa::AIPathFindingManager::CreatePath(currentPositionMap, targetInMap);

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
