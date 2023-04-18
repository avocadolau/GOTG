#pragma once
#include "AI_Crowd.h"
#include <glm/glm.hpp>

class NavAgent {
public:
    NavAgent(const glm::vec3& position, float radius, float height, float maxSpeed, float maxAcceleration);
   
    ~NavAgent();

    void Update(float deltaTime);

    void SetDestination(const glm::vec3& target);
    void SetPosition(const glm::vec3& position);

    void SetMaxSpeed(float maxSpeed);
    void SetMaxAcceleration(float maxAcceleration);

    const glm::vec3& GetCurrentPosition() const;
    const glm::vec3& GetCurrentVelocity() const;
    
    float GetMaxSpeed() const;
    float GetMaxAcceleration() const;

    void RegisterWithCrowd();

private:
    int m_agentIndex;
    glm::vec3 m_currentPos;
    glm::vec3 m_currentVel;
    float m_radius;
    float m_height;
    float m_maxSpeed;
    float m_maxAcceleration;
    dtCrowdAgentParams m_agentParams;
};
