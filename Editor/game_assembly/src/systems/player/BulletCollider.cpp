#include "BulletCollider.h"

void Wiwa::BulletCollider::OnAwake()
{
}
void Wiwa::BulletCollider::OnInit()
{
	m_Parent = m_Scene->GetEntityManager().GetEntityParent(m_EntityId);
}
void Wiwa::BulletCollider::OnUpdate()
{
}

void Wiwa::BulletCollider::OnCollisionEnter(Object* obj1, Object* obj2)
{
	if (obj1 != obj2)
	{
		m_Scene->GetEntityManager().DestroyEntity(m_EntityId);
	}
}
