#pragma once
#include <Wiwa/ecs/systems/System.h>

#include <Wiwa/utilities/Reflection.h>

namespace Wiwa {
	class  YondusFinSystem : public System {
	private:
		float m_Timer = 0;
		EntityManager::ComponentIterator m_YondusFinIt;
		EntityManager::ComponentIterator m_YondusFinTransformIt;
		EntityManager::ComponentIterator m_PlayerTransformIt;
		EntityManager::ComponentIterator m_WaveEnemiesIt;
	public:
		YondusFinSystem();
		~YondusFinSystem();

		void OnAwake() override;

		void OnInit() override;

		void OnUpdate() override;

		void OnDestroy() override;

		void OnCollisionEnter(Object* body1, Object* body2) override;
	};
}

REGISTER_SYSTEM(Wiwa::YondusFinSystem);