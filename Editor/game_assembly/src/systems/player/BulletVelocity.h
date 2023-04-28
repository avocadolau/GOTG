#pragma once
#include <Wiwa/ecs/systems/System.h>

namespace Wiwa
{
	class BulletVelocity : public System
	{
	public:
		BulletVelocity();
		virtual ~BulletVelocity();

		//Called the first frame
		virtual void OnAwake() override;
		//Called after OnAwake()
		virtual void OnInit() override;
		//Called every frame
		virtual void OnUpdate() override;
		virtual void OnDestroy() override;
		bool EnableBullet();
		bool OnDisabledFromPool();
		void InitBullet();

		virtual void OnCollisionEnter(Object* obj1, Object* obj2) override;
	private:
		EntityManager::ComponentIterator m_BulletIt;
		float m_Timer;
	};
}

REGISTER_SYSTEM(Wiwa::BulletVelocity);
