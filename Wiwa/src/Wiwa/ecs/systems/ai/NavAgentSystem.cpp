#include <wipch.h>
#include "NavAgentSystem.h"
#include "Wiwa/ecs/components/ai/NavAgent.h"
#include <glew.h>
#include <Wiwa/ecs/systems/PhysicsSystem.h>

namespace Wiwa
{
    NavAgentSystem::NavAgentSystem()
    {
        m_AgentIndex = -1;
      
        //m_AgentParams.radius = ;
        //m_AgentParams.height = m_height;
        //m_AgentParams.maxSpeed = m_maxSpeed;
        //m_AgentParams.maxAcceleration = m_maxAcceleration;
        //m_AgentParams.collisionQueryRange = m_radius * 12.f;
        //m_AgentParams.pathOptimizationRange = m_radius * 30.f;
        //m_AgentParams.updateFlags = DT_CROWD_ANTICIPATE_TURNS | DT_CROWD_OPTIMIZE_VIS | DT_CROWD_OPTIMIZE_TOPO | DT_CROWD_OBSTACLE_AVOIDANCE;
        //m_AgentParams.obstacleAvoidanceType = 3;
        //m_AgentParams.queryFilterType = 0;
        //m_AgentParams.separationWeight = 1;
    }

    NavAgentSystem::~NavAgentSystem()
    {
        if (m_AgentIndex != -1) {
            Crowd::getInstance().RemoveAgent(m_AgentIndex);
        }
    }

    void NavAgentSystem::OnInit()
    {
        Transform3D* tr = GetComponent<Transform3D>();
        m_CurrentPos = tr->localPosition;
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
        Crowd& crowd = Crowd::getInstance();
       /* const dtQueryFilter* filter= Crowd::getInstance().getCrowd().getFilter(m_AgentIndex);
        const dtNavMeshQuery* query = Crowd::getInstance().getCrowd().getNavMeshQuery();
        dtPolyRef startRef;
        float m_spos[3];
        dtStatus status = query->findRandomPoint(filter, frand, &startRef, m_spos);
        if (dtStatusSucceed(status))
        {
            SetDestination(glm::vec3(m_spos[0], m_spos[1], m_spos[2]));
        }*/

        if (m_AgentIndex != -1) {
            const dtCrowdAgent* crowdAgent = crowd.getCrowd().getAgent(m_AgentIndex);
            Wiwa::NavAgent* navAgent = GetComponentByIterator<Wiwa::NavAgent>(m_NavAgentIt);

            if (crowdAgent)
            {
                m_CurrentPos.x = crowdAgent->npos[0];
                m_CurrentPos.y = crowdAgent->npos[1];
                m_CurrentPos.z = crowdAgent->npos[2];

            /*    if (glm::distance(m_CurrentPos, glm::vec3(crowdAgent->targetPos[0], crowdAgent->targetPos[1], crowdAgent->targetPos[2])) <= navAgent->stoppingDistance)
                {
                    crowd.getCrowd().resetMoveTarget(m_AgentIndex);
                }*/
                // Calculate distance to target
                float distanceToTarget = glm::distance(m_CurrentPos, glm::vec3(crowdAgent->targetPos[0], crowdAgent->targetPos[1], crowdAgent->targetPos[2]));

                if (distanceToTarget > navAgent->stoppingDistance) {
                    // Set agent target and speed
                    crowd.SetAgentMaxAcceleration(m_AgentIndex, m_AgentParams.maxAcceleration);
                    crowd.SetAgentMaxSpeed(m_AgentIndex, m_AgentParams.maxSpeed);
                }
                else {
                    // Slow down agent as it approaches target
                    float deceleration = (m_AgentParams.maxSpeed * m_AgentParams.maxSpeed) / (2 * navAgent->stoppingDistance);
                    float desiredSpeed = (distanceToTarget / navAgent->stoppingDistance) * m_AgentParams.maxSpeed;
                    float speed = std::min(desiredSpeed, m_AgentParams.maxSpeed);
                    speed = std::max(speed, 0.0f);
                    crowd.SetAgentMaxAcceleration(m_AgentIndex, m_AgentParams.maxAcceleration);
                    crowd.SetAgentMaxSpeed(m_AgentIndex, speed);
                }
            }

            //crowd.SetAgentMaxSpeed(m_AgentIndex, m_AgentParams.maxSpeed);
            //crowd.SetAgentParameters(m_AgentIndex, m_AgentParams);

            Render();

            Wiwa::EntityManager& em = m_Scene->GetEntityManager();
            Wiwa::PhysicsSystem* physSys = em.GetSystem<PhysicsSystem>(m_EntityId);

            Transform3D* tr = GetComponent<Transform3D>();
            tr->localPosition = m_CurrentPos;
        }
    }

    void NavAgentSystem::SetDestination(const glm::vec3& target)
    {
        if (m_AgentIndex != -1) {
            Crowd::getInstance().SetAgentTarget(m_AgentIndex, &target[0]);
        }
    }

    void NavAgentSystem::SetPosition(const glm::vec3& position)
    {
        if (m_AgentIndex != -1) {
            dtCrowdAgent* agent = Crowd::getInstance().getCrowd().getEditableAgent(m_AgentIndex);

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
        m_AgentParams.maxSpeed = maxSpeed;
    }

    void NavAgentSystem::SetMaxAcceleration(float maxAcceleration)
    {
        m_AgentParams.maxAcceleration = maxAcceleration;
    }

    const glm::vec3& NavAgentSystem::GetCurrentPosition() const
    {
        return m_CurrentPos;
    }

  /*  const glm::vec3& NavAgentSystem::GetCurrentVelocity() const
    {
        return m_CurrentVel;
    }*/

    float NavAgentSystem::GetMaxSpeed() const
    {
        return m_AgentParams.maxSpeed;
    }

    float NavAgentSystem::GetMaxAcceleration() const
    {
        return m_AgentParams.maxAcceleration;
    }

    void NavAgentSystem::RegisterWithCrowd()
    {
        m_AgentIndex = Crowd::getInstance().AddAgent(&m_CurrentPos[0], &m_AgentParams);
        Crowd::getInstance().SetAgentParameters(m_AgentIndex, m_AgentParams);
    }

    void NavAgentSystem::RefreshParamters()
    {
        Wiwa::NavAgent* agent = GetComponentByIterator<Wiwa::NavAgent>(m_NavAgentIt);
        if (agent)
        {
            m_AgentParams.radius = agent->radius;
            m_AgentParams.height = agent->height;
            m_AgentParams.maxSpeed = agent->maxSpeed;
            m_AgentParams.maxAcceleration = agent->maxAcceleration;
            m_AgentParams.collisionQueryRange = agent->radius * 12.f;
            m_AgentParams.pathOptimizationRange = agent->radius * 30.f;
            m_AgentParams.updateFlags = DT_CROWD_ANTICIPATE_TURNS | DT_CROWD_OPTIMIZE_VIS | DT_CROWD_OPTIMIZE_TOPO | DT_CROWD_OBSTACLE_AVOIDANCE;
            m_AgentParams.obstacleAvoidanceType = 3;
            m_AgentParams.queryFilterType = 0;
            m_AgentParams.separationWeight = 1;
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
        duDebugDrawCylinderWire(&dd, m_CurrentPos.x - m_AgentParams.radius, m_CurrentPos.y + 0.02f, m_CurrentPos.z - m_AgentParams.radius, m_CurrentPos.x + m_AgentParams.radius, m_CurrentPos.y + m_AgentParams.height, m_CurrentPos.z + m_AgentParams.radius, startCol, 2.0f);

        duDebugDrawCircle(&dd, m_CurrentPos.x, m_CurrentPos.y + 1, m_CurrentPos.z, m_AgentParams.radius, duRGBA(0, 0, 0, 64), 1.0f);

        unsigned int colb = duRGBA(0, 0, 0, 196);
        dd.begin(DU_DRAW_LINES);
        dd.vertex(m_CurrentPos.x, m_CurrentPos.y - 1, m_CurrentPos.z, colb);
        dd.vertex(m_CurrentPos.x, m_CurrentPos.y + 1, m_CurrentPos.z, colb);
        dd.vertex(m_CurrentPos.x - m_AgentParams.radius / 2, m_CurrentPos.y + 0.02f, m_CurrentPos.z, colb);
        dd.vertex(m_CurrentPos.x + m_AgentParams.radius / 2, m_CurrentPos.y + 0.02f, m_CurrentPos.z, colb);
        dd.vertex(m_CurrentPos.x, m_CurrentPos.y + 0.02f, m_CurrentPos.z - m_AgentParams.radius / 2, colb);
        dd.vertex(m_CurrentPos.x, m_CurrentPos.y + 0.02f, m_CurrentPos.z + m_AgentParams.radius / 2, colb);
        dd.end();
        dd.depthMask(true);
        
    }
    void NavAgentSystem::DrawPath()
    {
        const unsigned int startCol = duRGBA(128, 25, 0, 192);
        duDebugDraw& dd = Wiwa::RecastManager::m_RecastMesh->getDebugDraw();

        if (m_AgentIndex != -1) {
            const dtCrowdAgent* agent = Crowd::getInstance().getCrowd().getAgent(m_AgentIndex);
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

