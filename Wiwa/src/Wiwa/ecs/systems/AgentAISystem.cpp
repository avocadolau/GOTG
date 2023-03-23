#include <wipch.h>

#include "AgentAISystem.h"
#include "Wiwa/ecs/components/AgentAI.h"
#include <Wiwa/AI/AIMapGeneration.h>
//#include "Wiwa/ecs/components/Transform3D.h"

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
	CreatePath({ 20,0,20 });
}

void Wiwa::AgentAISystem::OnUpdate()
{
	Wiwa::AgentAI* agent = GetComponentByIterator<Wiwa::AgentAI>(m_AgentAI);
	Wiwa::Transform3D* transform = GetComponentByIterator<Wiwa::Transform3D>(m_Transform);

	/*if (transform->position.x < m_DirectionPoint.x && transform->position.z < m_DirectionPoint.y) {
		agent->hasArrived = true; 
	}*/

	if (lastPath.empty() == false && m_IsMoving == false)
	{
		GoToNextPosition();
	}

	if (HasArrivedNextPosition(glm::ivec2(transform->localPosition.x, transform->localPosition.z), m_DirectionPoint))
	{
		m_IsMoving = false;
	}

	if (m_IsMoving)
	{
		float dirX = m_DirectionPoint.x - transform->position.x;
		dirX /= abs(dirX);

		float dirY = m_DirectionPoint.y - transform->position.z;
		dirY /= abs(dirY);

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


	glBegin(GL_LINES);
	glColor3f(1, 1, 1);
	glLineWidth(10.0f);

	Wiwa::AIMapGeneration::MapData mapdata = Wiwa::AIMapGeneration::GetMapData();

	float d = mapdata.height * mapdata.width * 2;

	for (float i = -d; i <= d; i += 1.0f)
	{
		glVertex3f(i, 0.0f, -d);
		glVertex3f(i, 0.0f, d);
		glVertex3f(-d, 0.0f, i);
		glVertex3f(d, 0.0f, i);
	}


	glVertex3f(transform->localPosition.x, 0, transform->localPosition.z);

	glm::vec2 dest = Wiwa::AIMapGeneration::WorldToMap(49, 49);

	glVertex3f(dest.x, 0, dest.y);

	glEnd();

	camera->frameBuffer->Unbind();
}

void Wiwa::AgentAISystem::OnDestroy()
{
}

void Wiwa::AgentAISystem::CreatePath(const glm::vec3& targetPos)
{
	Wiwa::AgentAI* agent = GetComponentByIterator<Wiwa::AgentAI>(m_AgentAI);
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

	m_IsMoving = true;
}


bool Wiwa::AgentAISystem::HasArrivedNextPosition(const glm::ivec2& next_position, const glm::ivec2& current_position)
{
	return (next_position == current_position);
}
