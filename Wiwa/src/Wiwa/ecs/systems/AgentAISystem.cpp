#include <wipch.h>

#include "AgentAISystem.h"
#include "Wiwa/ecs/components/AgentAI.h"
#include <Wiwa/AI/AIPathFindingManager.h>
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
}

void Wiwa::AgentAISystem::OnUpdate()
{
	//
	GoToPosition({49,0,49});
	//
	m_DirectionPoint = Wiwa::AIPathFindingManager::MapToWorld(m_DirectionPoint.x, m_DirectionPoint.y);

	Wiwa::AIPathFindingManager::MapData& localMapData = Wiwa::AIPathFindingManager::GetMapData();
	m_DirectionPoint = { m_DirectionPoint.x + (localMapData.tileWidth*0.5f), m_DirectionPoint.y + (localMapData.tileHeight * 0.5f) };

	Wiwa::AgentAI* agent = GetComponentByIterator<Wiwa::AgentAI>(m_AgentAI);
	Wiwa::Transform3D* transform = GetComponentByIterator<Wiwa::Transform3D>(m_Transform);

	/*if (transform->position.x < m_DirectionPoint.x && transform->position.z < m_DirectionPoint.y) {
		agent->hasArrived = true; 
	}*/

	float dirX = m_DirectionPoint.x - transform->position.x;
	dirX /= abs(dirX);

	float dirY = m_DirectionPoint.y - transform->position.z;
	dirY /= abs(dirY);

	// Move to direction
	transform->localPosition.x += agent->speed * dirX;
	transform->localPosition.z += agent->speed * dirY;

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

	Wiwa::AIPathFindingManager::MapData mapdata = Wiwa::AIPathFindingManager::GetMapData();

	float d = mapdata.height * mapdata.width * 2;

	for (float i = -d; i <= d; i += 1.0f)
	{
		glVertex3f(i, 0.0f, -d);
		glVertex3f(i, 0.0f, d);
		glVertex3f(-d, 0.0f, i);
		glVertex3f(d, 0.0f, i);
	}


	glVertex3f(transform->localPosition.x, 0, transform->localPosition.z);

	glm::vec2 dest = Wiwa::AIPathFindingManager::WorldToMap(49, 49);

	glVertex3f(dest.x, 0, dest.y);

	glEnd();

	camera->frameBuffer->Unbind();
}

void Wiwa::AgentAISystem::OnDestroy()
{
}

void Wiwa::AgentAISystem::GoToPosition(glm::vec3 targetPos)
{
	Wiwa::AgentAI* agent = GetComponentByIterator<Wiwa::AgentAI>(m_AgentAI);
	Wiwa::Transform3D* transform = GetComponentByIterator<Wiwa::Transform3D>(m_Transform);

	// transform->position;
	
	m_DirectionPoint = Wiwa::AIPathFindingManager::WorldToMap(transform->position.x, transform->position.z);
	

	glm::ivec2 target = {targetPos.x, targetPos.z};
	//Wiwa::AIPathFindingManager::CreatePath(m_DirectionPoint,target);

	glm::ivec2 nextPos = target;

	const std::vector<glm::ivec2>* lastPath = Wiwa::AIPathFindingManager::GetLastPath();
	
	if (lastPath->size() > 1)
	{
		nextPos = { lastPath->at(1).x, lastPath->at(1).y };
	}

	m_DirectionPoint = nextPos;

	
}
