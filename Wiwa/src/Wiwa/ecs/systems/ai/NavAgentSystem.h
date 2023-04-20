#pragma once
#include <Wiwa/ecs/systems/System.h>
#include "Wiwa/AI/AI_Crowd.h"
#include <glm/glm.hpp>
#include <Wiwa/ecs/EntityManager.h>

namespace Wiwa
{
    class WI_API NavAgentSystem : public System {
    public:
        NavAgentSystem();

        ~NavAgentSystem();

        void OnInit() override;

        void OnUpdate() override;

        void SetDestination(const glm::vec3& target);
        void SetPosition(const glm::vec3& position);

        void SetMaxSpeed(float maxSpeed);
        void SetMaxAcceleration(float maxAcceleration);

        const glm::vec3& GetCurrentPosition() const;
        //const glm::vec3& GetCurrentVelocity() const;

        float GetMaxSpeed() const;
        float GetMaxAcceleration() const;

        void RegisterWithCrowd();
        void RefreshParamters();
    private:
        void Render();
        void DrawAgent();
        void DrawPath();

        int m_AgentIndex;
        glm::vec3 m_CurrentPos;
        //glm::vec3 m_CurrentVel;
        dtCrowdAgentParams m_AgentParams;

        EntityManager::ComponentIterator m_NavAgentIt;
        //EntityManager::ComponentIterator m_Transform;
    };

}

REGISTER_SYSTEM(Wiwa::NavAgentSystem);