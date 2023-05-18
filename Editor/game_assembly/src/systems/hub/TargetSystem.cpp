#include "TargetSystem.h"

TargetSystem::TargetSystem() : 
	m_HitTimer(0.0f),
	m_HitTime(2.0f)
{

}

TargetSystem::~TargetSystem()
{

}

void TargetSystem::OnUpdate()
{
}

void TargetSystem::OnCollisionEnter(Wiwa::Object* body1, Wiwa::Object* body2)
{
	if (body1->id == m_EntityId) {

	}
}