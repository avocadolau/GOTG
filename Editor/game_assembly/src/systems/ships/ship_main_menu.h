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

	private:

		glm::vec3 m_StartPos;

	};
}

