#pragma once

#include <Wiwa/ecs/systems/System.h>

#include <Wiwa/utilities/Reflection.h>

namespace Wiwa {
	class ShipMainMenu : public System
	{
	public:
		ShipMainMenu();
		virtual ~ShipMainMenu();

		virtual void OnAwake() override;
		virtual void OnInit() override;
		virtual void OnUpdate() override;

		void PanToCamera();

		void SetPanToCamera(bool ret);

	private:
		EntityManager::ComponentIterator m_ShipDataIt;

		glm::vec3 m_StartPos;
		float m_Time;
		float m_TimerToChangeScreen;
		glm::vec3 m_EndPanPos;
	};
}
REGISTER_SYSTEM(Wiwa::ShipMainMenu);

