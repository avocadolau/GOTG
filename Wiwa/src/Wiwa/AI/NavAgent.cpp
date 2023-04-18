#include "NavAgent.h"

NavAgent::NavAgent(const glm::vec3& position, float radius, float height, float maxSpeed, float maxAcceleration)
    : m_agentIndex(-1), m_currentPos(position), m_radius(radius), m_height(height), m_maxSpeed(maxSpeed), m_maxAcceleration(maxAcceleration)
{
    m_agentParams.radius = m_radius;
    m_agentParams.height = m_height;
    m_agentParams.maxSpeed = m_maxSpeed;
    m_agentParams.maxAcceleration = m_maxAcceleration;
    m_agentParams.collisionQueryRange = m_radius * 12.f;
    m_agentParams.pathOptimizationRange = m_radius * 30.f;
    m_agentParams.updateFlags = DT_CROWD_ANTICIPATE_TURNS | DT_CROWD_OPTIMIZE_VIS | DT_CROWD_OPTIMIZE_TOPO | DT_CROWD_OBSTACLE_AVOIDANCE;
    m_agentParams.obstacleAvoidanceType = 3;
    m_agentParams.queryFilterType = 0;
}

NavAgent::~NavAgent()
{
    if (m_agentIndex != -1) {
        Crowd::getInstance()->RemoveAgent(m_agentIndex);
    }
}

void NavAgent::Update(float deltaTime)
{
    if (m_agentIndex != -1) {
        const dtCrowdAgent* agent = Crowd::getInstance()->getCrowd().getAgent(m_agentIndex);
        if (agent)
        {
            m_currentPos.x = agent->npos[0];
            m_currentPos.y = agent->npos[1];
            m_currentPos.z = agent->npos[2];
        }
        Crowd::getInstance()->SetAgentMaxSpeed(m_agentIndex, m_maxSpeed);
        Crowd::getInstance()->SetAgentMaxAcceleration(m_agentIndex, m_maxAcceleration);
        /* Crowd::getInstance()->SetAgentPosition(m_agentIndex, &m_currentPos[0]);
         Crowd::getInstance()->SetAgentMaxSpeed(m_agentIndex, m_maxSpeed);
         Crowd::getInstance()->SetAgentMaxAcceleration(m_agentIndex, m_maxAcceleration);*/
    }
}

void NavAgent::SetDestination(const glm::vec3& target)
{
    if (m_agentIndex != -1) {
        Crowd::getInstance()->SetAgentTarget(m_agentIndex, &target[0]);
    }
}

void NavAgent::SetPosition(const glm::vec3& position)
{
    if (m_agentIndex != -1) {
        dtCrowdAgent* agent = Crowd::getInstance()->getCrowd().getEditableAgent(m_agentIndex);
        if (agent)
        {
            agent->npos[0] = position.x;
            agent->npos[1] = position.y;
            agent->npos[2] = position.z;
        }
    }
}

void NavAgent::SetMaxSpeed(float maxSpeed)
{
    m_maxSpeed = maxSpeed;
}

void NavAgent::SetMaxAcceleration(float maxAcceleration)
{
    m_maxAcceleration = maxAcceleration;
}

const glm::vec3& NavAgent::GetCurrentPosition() const
{
    return m_currentPos;
}

const glm::vec3& NavAgent::GetCurrentVelocity() const
{
    return m_currentVel;
}

float NavAgent::GetMaxSpeed() const
{
    return m_maxSpeed;
}

float NavAgent::GetMaxAcceleration() const
{
    return m_maxAcceleration;
}

void NavAgent::RegisterWithCrowd()
{
    m_agentIndex = Crowd::getInstance()->AddAgent(&m_currentPos[0]);
    Crowd::getInstance()->SetAgentParameters(m_agentIndex, m_agentParams);
}