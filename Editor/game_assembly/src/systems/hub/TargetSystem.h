#pragma once

#include <Wiwa/ecs/systems/System.h>
#include <Wiwa/utilities/Reflection.h>

class TargetSystem : public Wiwa::System {
private:
	float m_HitTimer;
	float m_HitTime;
public:
	TargetSystem();
	~TargetSystem();

	void OnUpdate() override;

	void OnCollisionEnter(Wiwa::Object* body1, Wiwa::Object* body2) override;
};