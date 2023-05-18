#include "OneTimeDialogSystem.h"
#include <Wiwa/game/GameStateManager.h>

#include "Wiwa/Dialog/DialogManager.h"

namespace Wiwa
{

	Wiwa::OneTimeDialogSystem::OneTimeDialogSystem()
	{
		
	}

	OneTimeDialogSystem::~OneTimeDialogSystem()
	{
	}

	void OneTimeDialogSystem::OnAwake()
	{
	}

	void OneTimeDialogSystem::OnInit()
	{

	}

	void OneTimeDialogSystem::OnUpdate()
	{
		
	}

	void OneTimeDialogSystem::OnDestroy()
	{
	}

	void OneTimeDialogSystem::OnCollisionEnter(Object* body1, Object* body2)
	{
		std::string playerStr = "PLAYER";
		if (body1->id == m_EntityId && playerStr == body2->selfTagStr)
		{
			Wiwa::Scene* _scene = (Wiwa::Scene*)m_Scene;
			Wiwa::EntityManager& em = _scene->GetEntityManager();
			
			if ((Wiwa::Input::IsKeyPressed(Wiwa::Key::Space) || Wiwa::Input::IsButtonPressed(0, 3)))
			{
				//_scene->GetDialogManager()
			}
		}
	}

	void OneTimeDialogSystem::OnCollisionExit(Object* body1, Object* body2)
	{
		std::string playerStr = "PLAYER";
		if (body1->id == m_EntityId && playerStr == body2->selfTagStr)
		{
			Wiwa::Scene* _scene = (Wiwa::Scene*)m_Scene;
			Wiwa::EntityManager& em = _scene->GetEntityManager();

			// Should do nothing for now...
		}
	}
}