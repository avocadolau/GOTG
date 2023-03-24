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
	CreatePath({ 49,0,49 });
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
	//
	//
	//glBegin(GL_LINES);
	//glColor3f(1, 1, 1);
	//glLineWidth(10.0f);

	Wiwa::AIMapGeneration::MapData mapdata = Wiwa::AIMapGeneration::GetMapData();

	//float d = mapdata.height * mapdata.width * 2;

	//for (float i = -d; i <= d; i += mapdata.tileHeight)
	//{
	//	glVertex3f(i, 0.0f, -d);
	//	glVertex3f(i, 0.0f, d);
	//	//glVertex3f(-d, 0.0f, i);
	//	//glVertex3f(d, 0.0f, i);		
	//}

	//for (float j = -d; j <= d; j += mapdata.tileWidth)
	//{
	//	glVertex3f(-d, 0.0f, j);
	//	glVertex3f(d, 0.0f, j);
	//}
	//PhysicsManager& physicsManager = m_Scene->GetPhysicsManager();
	//
	//physicsManager.m_Debug_draw->lineDisplayShader->Bind();
	//physicsManager.m_Debug_draw->lineDisplayShader->setUniformMat4(physicsManager.m_Debug_draw->lineDisplayShaderUniforms.Model, glm::mat4(1.0f));
	//physicsManager.m_Debug_draw->lineDisplayShader->setUniformMat4(physicsManager.m_Debug_draw->lineDisplayShaderUniforms.View, camera->getView());
	//physicsManager.m_Debug_draw->lineDisplayShader->setUniformMat4(physicsManager.m_Debug_draw->lineDisplayShaderUniforms.Projection, camera->getProjection());
	//physicsManager.m_Debug_draw->lineDisplayShader->setUniformVec4(physicsManager.m_Debug_draw->lineDisplayShader->getUniformLocation("u_Color"), glm::vec4(1.0, 0.0f, 0.0f, 1.0f));

	unsigned char* map = Wiwa::AIMapGeneration::GetMap();
	for (int i = 0; i < mapdata.height; i++)
	{
		for (int j = 0; j < mapdata.width; j++)
		{
			glm::vec2 vec = Wiwa::AIMapGeneration::MapToWorld(i, j);
			//btVector3 topLeft, topRight, bottomLeft, bottomRight;
			//topLeft = btVector3(vec.x, 0, vec.y);
			//topRight = btVector3(vec.x + mapdata.tileWidth, 0, vec.y);
			//bottomLeft = btVector3(vec.x, 0, vec.y - mapdata.tileHeight);
			//bottomRight = btVector3(vec.x + mapdata.tileWidth, 0, vec.y - mapdata.tileHeight);
			
			//physicsManager.m_Debug_draw->drawLine(topLeft,bottomLeft, btVector4(0,0,1,1));
			//physicsManager.m_Debug_draw->drawLine(bottomLeft, bottomRight, btVector4(0, 0, 1, 1));
			//physicsManager.m_Debug_draw->drawLine(bottomRight, topRight, btVector4(0, 0, 1, 1));
			//physicsManager.m_Debug_draw->drawLine(topRight,topLeft, btVector4(0, 0, 1, 1));
			//

			if (map[j * mapdata.width + i] == 255)
			{			
				glColor3f( (j + i)*0.01f, 0, 0);
			}
			else
			{
				glColor3f(0, 0,  (j + i) * 0.01f);
			}
			//glColor3f(1, 0, 0);
			glPolygonMode(GL_FRONT_AND_BACK, GL_3D); // Set the polygon mode to wireframe
			glBegin(GL_QUADS); // Begin drawing the quad   
			//glVertex3f(vec.x,0, vec.y - mapdata.tileHeight ); // Bottom-left vertex   
			//glVertex3f(vec.x,0, vec.y); // Top-left vertex   
			//glVertex3f(vec.x + mapdata.tileWidth,0, vec.y); // Top-right vertex   
			//glVertex3f(vec.x + mapdata.tileWidth,0, vec.y - mapdata.tileHeight); // Bottom-right vertex
			glVertex3f(vec.x, 0.0f, vec.y+1.0f); // Bottom-left vertex
			glVertex3f(vec.x +1.0f, 0.0f, vec.y+1.0f); // Bottom-right vertex
			glVertex3f(vec.x+1.0f, 0.0f, vec.y); // Top-right vertex
			glVertex3f(vec.x, 0.0f, vec.y); // Top-left vertex
			
			//
			glEnd();
			glPolygonMode(GL_FRONT_AND_BACK, GL_FILL); // Set the polygon mode back to fill
		}
	}


	//glVertex3f(transform->localPosition.x, 0, transform->localPosition.z);

	//glm::vec2 dest = Wiwa::AIMapGeneration::WorldToMap(20, 20);

	//glVertex3f(dest.x, 0, dest.y);

	glEnd();
	//physicsManager.m_Debug_draw->lineDisplayShader->UnBind();
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
	//m_DirectionPoint = { nextPos.x , nextPos.y };

	m_IsMoving = true;
}


bool Wiwa::AgentAISystem::HasArrivedNextPosition(const glm::ivec2& next_position, const glm::ivec2& current_position)
{
	return (next_position == current_position);
}
