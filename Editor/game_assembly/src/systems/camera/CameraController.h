#pragma once
#include <Wiwa/core/Core.h>
#include <Wiwa/ecs/systems/System.h>
#include <glm/glm.hpp>

namespace Wiwa
{
	class  CameraController : public System
	{
	private:
		Camera* m_Camera;
		EntityId m_PlayerId;
		EntityManager::ComponentIterator m_PlayerTransformIt;
		EntityManager::ComponentIterator m_TransformIt;


        glm::vec3 m_CameraOffset{ 0, 40.0f, -15.0f };
        glm::vec3 m_CameraRotation{ 90, -70, 0 };
	public:
		CameraController();
		~CameraController();

		void OnAwake() override;

		void OnInit() override;

		void OnUpdate() override;

		void OnDestroy() override;

        glm::vec3 CalculateForward(Transform3D transform)
        {
            glm::vec3 rotrad;
            rotrad.x = DegToRad(transform.localRotation.x);
            rotrad.y = DegToRad(transform.localRotation.y);
            rotrad.z = DegToRad(transform.localRotation.z);

            glm::vec3 forward;
            forward.x = cos(rotrad.x) * sin(rotrad.y);
            forward.y = -sin(rotrad.x);
            forward.z = cos(rotrad.x) * cos(rotrad.y);

            forward.x = RadToDeg(forward.x);
            forward.y = RadToDeg(forward.y);
            forward.z = RadToDeg(forward.z);

            return glm::normalize(forward);
        }

        float DegToRad(float deg)
        {
            return deg * PI / 180.0f;
        }

        float RadToDeg(float rad)
        {
            return rad * 180.0f / PI;
        }
	};
}
REGISTER_SYSTEM(Wiwa::CameraController);

