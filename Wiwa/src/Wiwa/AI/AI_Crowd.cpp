#include <wipch.h>
#include "AI_Crowd.h"

Crowd::Crowd() : m_crowd(nullptr)
{
    Init();
}

Crowd::~Crowd()
{
    CleanUp();
}

void Crowd::Init()
{
    CleanUp();

    // Initialize crowd simulation
    m_crowd = dtAllocCrowd();
    m_crowd->init(MAX_AGENTS, 0.6f, Wiwa::RecastManager::getNavMesh());
    memset(&m_agentParams, 0, sizeof(m_agentParams));
    m_agentParams.radius = 0.6f;
    m_agentParams.height = 1.8f;
    m_agentParams.maxAcceleration = 8.0f;
    m_agentParams.maxSpeed = 3.0f;
    m_agentParams.collisionQueryRange = m_agentParams.radius * 12.0f;
    m_agentParams.pathOptimizationRange = m_agentParams.radius * 30.0f;
    m_agentParams.updateFlags = DT_CROWD_ANTICIPATE_TURNS | DT_CROWD_OPTIMIZE_VIS | DT_CROWD_SEPARATION;
}

void Crowd::CleanUp()
{
    dtFreeCrowd(m_crowd);
}


void Crowd::Update(float deltaTime)
{
    m_crowd->update(deltaTime, nullptr);
}

int Crowd::AddAgent(const float* position, dtCrowdAgentParams* param, bool defaultParam)
{
    if (!param)
        return -1;

    if (defaultParam){
        *param = m_agentParams;
    }
    int index = m_crowd->addAgent(position, param);
    if (index != -1) {
        m_agents.emplace_back(index);
        param->userData = &m_agents.back();
    }
    return index;
}

void Crowd::RemoveAgent(int agentIndex)
{
    if (agentIndex >= 0) {
        m_crowd->removeAgent(agentIndex);
        std::vector<int>::iterator it = std::find(m_agents.begin(), m_agents.end(), agentIndex);
        if (it != m_agents.end()) {
            m_agents.erase(it);
        }
        //m_agents.erase(m_agents.begin() + agentIndex);
    }
}

void Crowd::SetAgentTarget(int agentIndex, const float* target)
{
    if (agentIndex >= 0 && agentIndex < m_agents.size()) {
        const dtCrowdAgent* agent = m_crowd->getAgent(agentIndex);

        dtPolyRef targetRef;
        float targetPos[3];
        const dtQueryFilter* filter = m_crowd->getFilter(agentIndex);
        const float* extents = m_crowd->getQueryExtents();
        m_crowd->getNavMeshQuery()->findNearestPoly(target, extents, filter, &targetRef, targetPos);
        m_crowd->requestMoveTarget(agentIndex, targetRef, targetPos);
    }
}

void Crowd::SetAgentMaxSpeed(int agentIndex, float speed) {
    if (agentIndex >= 0 && agentIndex < m_agents.size()) {
        dtCrowdAgentParams params = m_agentParams;
        params.maxSpeed = speed;
        m_crowd->updateAgentParameters(agentIndex, &params);
    }
}

void Crowd::SetAgentMaxAcceleration(int agentIndex, float acceleration)
{
    if (agentIndex >= 0 && agentIndex < m_agents.size()) {
        dtCrowdAgentParams params = m_agentParams;
        params.maxAcceleration = acceleration;
        m_crowd->updateAgentParameters(agentIndex, &params);
    }
}

void Crowd::SetAgentParameters(int agentIndex, const dtCrowdAgentParams& params)
{
    m_crowd->updateAgentParameters(agentIndex, &params);
}

int Crowd::GetAgentCount() const
{
    if (m_agents.size() == m_crowd->getAgentCount())
        return  m_crowd->getAgentCount();
    else
        return -1;
}

const float* Crowd::GetAgentPosition(int agentIndex) const
{
    if (agentIndex >= 0 && agentIndex < m_agents.size()) {
        const dtCrowdAgent* agent = m_crowd->getAgent(agentIndex);
        return agent->npos;
    }
    return nullptr;
}

const float* Crowd::GetAgentVelocity(int agentIndex) const
{
    if (agentIndex >= 0 && agentIndex < m_agents.size()) {
        const dtCrowdAgent* agent = m_crowd->getAgent(agentIndex);
        return agent->dvel;
    }
    return nullptr;
}

glm::vec3 Crowd::GetAgentPositionVec(int agentIndex) const
{
    if (agentIndex >= 0 && agentIndex < m_agents.size()) {
        const dtCrowdAgent* agent = m_crowd->getAgent(agentIndex);
        return glm::vec3(agent->npos[0], agent->npos[1], agent->npos[2]);
    }
    return glm::vec3(0.0f);
}

glm::vec3 Crowd::GetAgentVelocityVec(int agentIndex) const
{
    if (agentIndex >= 0 && agentIndex < m_agents.size()) {
        const dtCrowdAgent* agent = m_crowd->getAgent(agentIndex);
        return glm::vec3(agent->dvel[0], agent->dvel[1], agent->dvel[2]);
    }
    return glm::vec3(0.0f);
}



