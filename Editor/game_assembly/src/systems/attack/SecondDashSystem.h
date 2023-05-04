#pragma once
#include <Wiwa/ecs/systems/System.h>

#include <Wiwa/utilities/Reflection.h>

namespace Wiwa {
	class  SecondDashSystem : public System {
	private:
		float m_Timer = 0;
		EntityManager::ComponentIterator m_AttackIt;
		EntityManager::ComponentIterator m_DashIt;
	public:
		SecondDashSystem();
		~SecondDashSystem();

		void OnAwake() override;

		void OnInit() override;

		void InitBullet();

		void OnUpdate() override;

		void OnDestroy() override;

		void OnCollisionEnter(Object* body1, Object* body2) override;

		bool EnableBullet();

		bool OnDisabledFromPool() override;
	};
}

REGISTER_SYSTEM(Wiwa::SecondDashSystem);