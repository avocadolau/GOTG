#pragma once
#include <Wiwa/ecs/systems/System.h>

namespace Wiwa
{
	class BulletVelocity : public System
	{
	public:
		BulletVelocity() = default;
		virtual ~BulletVelocity();

		//Called the first frame
		virtual void OnAwake() override;
		//Called after OnAwake()
		virtual void OnInit() override;
		//Called every frame
		virtual void OnUpdate() override;
	private:
		EntityManager::ComponentIterator m_BulletDataIt;
		float m_Timer;
	};
}

REGISTER_SYSTEM(Wiwa::BulletVelocity);
