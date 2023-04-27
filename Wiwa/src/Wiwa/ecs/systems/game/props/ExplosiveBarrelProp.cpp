#include <wipch.h>
#include "ExplosiveBarrelProp.h"

namespace Wiwa
{

	Wiwa::ExplosiveBarrelProp::ExplosiveBarrelProp()
	{

	}

	ExplosiveBarrelProp::~ExplosiveBarrelProp()
	{
	}

	void ExplosiveBarrelProp::OnAwake()
	{
	}

	void ExplosiveBarrelProp::OnInit()
	{

	}

	void ExplosiveBarrelProp::OnUpdate()
	{

	}

	void ExplosiveBarrelProp::OnDestroy()
	{
	}

	void ExplosiveBarrelProp::OnCollisionEnter(Object* body1, Object* body2)
	{
		std::string playerStr = "PLAYER_BULLET";
		if (body1->id == m_EntityId && playerStr == body2->selfTagStr)
		{
			Wiwa::Scene* _scene = (Wiwa::Scene*)m_Scene;
			Wiwa::EntityManager& em = _scene->GetEntityManager();
		}
	}
}