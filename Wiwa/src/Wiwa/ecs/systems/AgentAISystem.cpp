#include <wipch.h>

#include "AgentAISystem.h"
#include "Wiwa/ecs/components/AgentAI.h"
#include <Wiwa/AI/AIMapGeneration.h>
//#include "Wiwa/ecs/components/Transform3D.h"
#include <Wiwa/physics/PhysicsManager.h>

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

	if (agent->hasPath == false)
	{
		CreatePath(agent->target);
		agent->hasPath = true;
		agent->hasArrived = false;
	}

	if (lastPath.empty() == false && m_IsMoving == false)
	{
		GoToNextPosition();

		if (lastPath.empty())
		{
			agent->hasPath = false;
			agent->hasArrived = true;
		}
	}

	if (HasArrivedNextPosition(glm::ivec2(transform->localPosition.x, transform->localPosition.z), m_DirectionPoint))
	{
		m_IsMoving = false;
	}

	if (m_IsMoving)
	{
		float dirX = 0.0f;

		if (m_DirectionPoint.x != transform->position.x)
		{
			dirX = m_DirectionPoint.x - transform->position.x;
			dirX /= abs(dirX);
		}
		
		float dirY = 0.0f;

		if (m_DirectionPoint.y != transform->position.z)
		{
			dirY = m_DirectionPoint.y - transform->position.z;
			dirY /= abs(dirY);
		}

		// Move to direction
		transform->localPosition.x += agent->speed * dirX * Time::GetDeltaTimeSeconds();
		transform->localPosition.z += agent->speed * dirY * Time::GetDeltaTimeSeconds();
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

	//if (!lastPath.empty())
	//{
	//	for (int i = 0; i < lastPath.size() - 1; i++)
	//	{
	//		glBegin(GL_LINE); // Begin drawing the quad   
	//		glVertex3f(lastPath[i].x, 0, lastPath[i].y);
	//		glVertex3f(lastPath[i + 1].x, 0, lastPath[i + 1].y);
	//		glEnd();
	//	}

	//}
	
	camera->frameBuffer->Unbind();
}

void Wiwa::AgentAISystem::OnDestroy()
{
}

void Wiwa::AgentAISystem::CreatePath(const glm::vec3& targetPos)
{
	Wiwa::Transform3D* transform = GetComponentByIterator<Wiwa::Transform3D>(m_Transform);
	
	glm::ivec2 currentPositionMap = Wiwa::AIMapGeneration::WorldToMap(transform->position.x, transform->position.z);
	glm::ivec2 targetInMap = Wiwa::AIMapGeneration::WorldToMap(targetPos.x, targetPos.z);
	
	Wiwa::AIPathFindingManager::CreatePath(currentPositionMap, targetInMap);

	lastPath = *Wiwa::AIPathFindingManager::GetLastPath();
}

void Wiwa::AgentAISystem::GoToNextPosition()
{
	nextPos = lastPath.back();
	lastPath.pop_back();

	Wiwa::AIMapGeneration::MapData& localMapData = Wiwa::AIMapGeneration::GetMapData();
	m_DirectionPoint = { nextPos.x + (localMapData.tileWidth * 0.5f), nextPos.y + (localMapData.tileHeight * 0.5f) };
	//m_DirectionPoint = { nextPos.x , nextPos.y };

	m_IsMoving = true;
}


bool Wiwa::AgentAISystem::HasArrivedNextPosition(const glm::ivec2& next_position, const glm::ivec2& current_position)
{
	return (next_position == current_position);
}
