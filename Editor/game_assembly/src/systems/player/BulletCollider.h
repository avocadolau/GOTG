#pragma once
#include <Wiwa/ecs/systems/System.h>
namespace Wiwa
{
	class BulletCollider : public System
	{
	public:
		//Called the first frame
		virtual void OnAwake() override;
		//Called after OnAwake()
		virtual void OnInit() override;
		//Called every frame
		virtual void OnUpdate() override;

		virtual void OnCollisionEnter(Object* obj1, Object* obj2) override;
	private:
		EntityId m_Parent;
	};
}

REGISTER_SYSTEM(Wiwa::BulletCollider)