#include <wipch.h>
#include "NavAgentSystem.h"
#include "Wiwa/ecs/components/ai/NavAgent.h"
#include <glew.h>
#include <Wiwa/ecs/systems/PhysicsSystem.h>

namespace Wiwa
{
    NavAgentSystem::NavAgentSystem()
    {
        m_agentIndex = -1;
      
        //m_agentParams.radius = ;
        //m_agentParams.height = m_height;
        //m_agentParams.maxSpeed = m_maxSpeed;
        //m_agentParams.maxAcceleration = m_maxAcceleration;
        //m_agentParams.collisionQueryRange = m_radius * 12.f;
        //m_agentParams.pathOptimizationRange = m_radius * 30.f;
        //m_agentParams.updateFlags = DT_CROWD_ANTICIPATE_TURNS | DT_CROWD_OPTIMIZE_VIS | DT_CROWD_OPTIMIZE_TOPO | DT_CROWD_OBSTACLE_AVOIDANCE;
        //m_agentParams.obstacleAvoidanceType = 3;
        //m_agentParams.queryFilterType = 0;
        //m_agentParams.separationWeight = 1;
    }

    NavAgentSystem::~NavAgentSystem()
    {
        if (m_agentIndex != -1) {
            Crowd::getInstance().RemoveAgent(m_agentIndex);
        }
    }

    void NavAgentSystem::OnInit()
    {
        Transform3D* tr = GetComponent<Transform3D>();
        m_currentPos = tr->localPosition;
        m_NavAgentIt = GetComponentIterator<Wiwa::NavAgent>();
        RefreshParamters();
        RegisterWithCrowd();
    }

    // Returns a random number [0..1]
    static float frand()
    {
        //	return ((float)(rand() & 0xffff)/(float)0xffff);
        return (float)rand() / (float)RAND_MAX;
    }

    void NavAgentSystem::OnUpdate()
    {
        RefreshParamters();
        const dtQueryFilter* filter= Crowd::getInstance().getCrowd().getFilter(m_agentIndex);
        const dtNavMeshQuery* query = Crowd::getInstance().getCrowd().getNavMeshQuery();
        dtPolyRef startRef;
        float m_spos[3];
        dtStatus status = query->findRandomPoint(filter, frand, &startRef, m_spos);
        if (dtStatusSucceed(status))
        {
            SetDestination(glm::vec3(m_spos[0], m_spos[1], m_spos[2]));
        }

        if (m_agentIndex != -1) {
            const dtCrowdAgent* agent = Crowd::getInstance().getCrowd().getAgent(m_agentIndex);
            if (agent)
            {
                m_currentPos.x = agent->npos[0];
                m_currentPos.y = agent->npos[1];
                m_currentPos.z = agent->npos[2];
                //WI_INFO("X: {}, Y: {}, Z: {}", m_currentPos.x, m_currentPos.y, m_currentPos.z);
            }
            Crowd::getInstance().SetAgentMaxSpeed(m_agentIndex, m_agentParams.maxSpeed);
            Crowd::getInstance().SetAgentMaxAcceleration(m_agentIndex, m_agentParams.maxAcceleration);
            Crowd::getInstance().SetAgentParameters(m_agentIndex, m_agentParams);
            /* Crowd::getInstance().SetAgentPosition(m_agentIndex, &m_currentPos[0]);
             Crowd::getInstance().SetAgentMaxSpeed(m_agentIndex, m_maxSpeed);
             Crowd::getInstance().SetAgentMaxAcceleration(m_agentIndex, m_maxAcceleration);*/
            Render();

            Wiwa::EntityManager& em = m_Scene->GetEntityManager();
            Wiwa::PhysicsSystem* physSys = em.GetSystem<PhysicsSystem>(m_EntityId);
            // Check if entity has collision body
            if (em.HasComponent<Wiwa::CollisionBody>(m_EntityId))
            {
                physSys->getBody()->velocity = btVector3(agent->vel[0], agent->vel[1], agent->vel[2]);
            }
            else
            {
                Transform3D* tr = GetComponent<Transform3D>();
                tr->localPosition = m_currentPos;
            }
        }
    }

    void NavAgentSystem::SetDestination(const glm::vec3& target)
    {
        if (m_agentIndex != -1) {
            Crowd::getInstance().SetAgentTarget(m_agentIndex, &target[0]);
        }
    }

    void NavAgentSystem::SetPosition(const glm::vec3& position)
    {
        if (m_agentIndex != -1) {
            dtCrowdAgent* agent = Crowd::getInstance().getCrowd().getEditableAgent(m_agentIndex);
            if (agent)
            {
                agent->npos[0] = position.x;
                agent->npos[1] = position.y;
                agent->npos[2] = position.z;
            }
        }
    }

    void NavAgentSystem::SetMaxSpeed(float maxSpeed)
    {
        m_agentParams.maxSpeed = maxSpeed;
    }

    void NavAgentSystem::SetMaxAcceleration(float maxAcceleration)
    {
        m_agentParams.maxAcceleration = maxAcceleration;
    }

    const glm::vec3& NavAgentSystem::GetCurrentPosition() const
    {
        return m_currentPos;
    }

    const glm::vec3& NavAgentSystem::GetCurrentVelocity() const
    {
        return m_currentVel;
    }

    float NavAgentSystem::GetMaxSpeed() const
    {
        return m_agentParams.maxSpeed;
    }

    float NavAgentSystem::GetMaxAcceleration() const
    {
        return m_agentParams.maxAcceleration;
    }

    void NavAgentSystem::RegisterWithCrowd()
    {
        m_agentIndex = Crowd::getInstance().AddAgent(&m_currentPos[0], &m_agentParams);
        Crowd::getInstance().SetAgentParameters(m_agentIndex, m_agentParams);
    }

    void NavAgentSystem::RefreshParamters()
    {
        Wiwa::NavAgent* agent = GetComponentByIterator<Wiwa::NavAgent>(m_NavAgentIt);
        if (agent)
        {
            m_agentParams.radius = agent->radius;
            m_agentParams.height = agent->height;
            m_agentParams.maxSpeed = agent->maxSpeed;
            m_agentParams.maxAcceleration = agent->maxAcceleration;
            m_agentParams.collisionQueryRange = agent->radius * 12.f;
            m_agentParams.pathOptimizationRange = agent->radius * 30.f;
            m_agentParams.updateFlags = DT_CROWD_ANTICIPATE_TURNS | DT_CROWD_OPTIMIZE_VIS | DT_CROWD_OPTIMIZE_TOPO | DT_CROWD_OBSTACLE_AVOIDANCE;
            m_agentParams.obstacleAvoidanceType = 3;
            m_agentParams.queryFilterType = 0;
            m_agentParams.separationWeight = 1;
        }
    }

    void NavAgentSystem::Render()
    {
        Wiwa::Camera* camera = Wiwa::SceneManager::getActiveScene()->GetCameraManager().editorCamera;

        if (!camera->drawBoundingBoxes)
            return;

        glViewport(0, 0, camera->frameBuffer->getWidth(), camera->frameBuffer->getHeight());
        camera->frameBuffer->Bind(false);

        glMatrixMode(GL_PROJECTION);
        glLoadMatrixf(glm::value_ptr(camera->getProjection()));
        glMatrixMode(GL_MODELVIEW);
        glLoadMatrixf(glm::value_ptr(camera->getView()));

        DrawAgent();

        DrawPath();

        camera->frameBuffer->Unbind();
    }

    void NavAgentSystem::DrawAgent()
    {
        const unsigned int startCol = duRGBA(128, 25, 0, 192);
        duDebugDraw& dd = Wiwa::RecastManager::m_RecastMesh->getDebugDraw();

        dd.depthMask(false);

        // Agent dimensions.	
        duDebugDrawCylinderWire(&dd, m_currentPos.x - m_agentParams.radius, m_currentPos.y + 0.02f, m_currentPos.z - m_agentParams.radius, m_currentPos.x + m_agentParams.radius, m_currentPos.y + m_agentParams.height, m_currentPos.z + m_agentParams.radius, startCol, 2.0f);

        duDebugDrawCircle(&dd, m_currentPos.x, m_currentPos.y + 1, m_currentPos.z, m_agentParams.radius, duRGBA(0, 0, 0, 64), 1.0f);

        unsigned int colb = duRGBA(0, 0, 0, 196);
        dd.begin(DU_DRAW_LINES);
        dd.vertex(m_currentPos.x, m_currentPos.y - 1, m_currentPos.z, colb);
        dd.vertex(m_currentPos.x, m_currentPos.y + 1, m_currentPos.z, colb);
        dd.vertex(m_currentPos.x - m_agentParams.radius / 2, m_currentPos.y + 0.02f, m_currentPos.z, colb);
        dd.vertex(m_currentPos.x + m_agentParams.radius / 2, m_currentPos.y + 0.02f, m_currentPos.z, colb);
        dd.vertex(m_currentPos.x, m_currentPos.y + 0.02f, m_currentPos.z - m_agentParams.radius / 2, colb);
        dd.vertex(m_currentPos.x, m_currentPos.y + 0.02f, m_currentPos.z + m_agentParams.radius / 2, colb);
        dd.end();
        dd.depthMask(true);
        
    }
    void NavAgentSystem::DrawPath()
    {
        const unsigned int startCol = duRGBA(128, 25, 0, 192);
        duDebugDraw& dd = Wiwa::RecastManager::m_RecastMesh->getDebugDraw();

        if (m_agentIndex != -1) {
            const dtCrowdAgent* agent = Crowd::getInstance().getCrowd().getAgent(m_agentIndex);
            if (agent)
            {
                dd.depthMask(false);
                const unsigned int spathCol = duRGBA(64, 16, 0, 220);
                /*const unsigned int offMeshCol = duRGBA(128, 96, 0, 220);*/
                dd.begin(DU_DRAW_LINES, 2.0f);
                for (int i = 0; i < agent->ncorners - 1; ++i)
                {
                    dd.vertex(agent->cornerVerts[i * 3], agent->cornerVerts[i * 3 + 1] + 0.4f, agent->cornerVerts[i * 3 + 2], spathCol);
                    dd.vertex(agent->cornerVerts[(i + 1) * 3], agent->cornerVerts[(i + 1) * 3 + 1] + 0.4f, agent->cornerVerts[(i + 1) * 3 + 2], spathCol);
                }
                dd.end();
            }
        }
        dd.depthMask(true);

    }
}

