#pragma once
#include "Wiwa/ecs//Systems.h"

#include <Wiwa/utilities/Reflection.h>

namespace Wiwa {
	class  HowardShopSystem : public System {
	public:
		bool m_ReturnToBaseHUD;
		bool m_ActiveSecondPage;

		bool m_FirstShopActive;
		bool m_SecondShopActive;
	public:
		HowardShopSystem();
		~HowardShopSystem();

		void OnAwake() override;

		void OnInit() override;

		void OnUpdate() override;

		void OnDestroy() override;

		void OnCollisionEnter(Object* body1, Object* body2) override;
	};
}

REGISTER_SYSTEM(Wiwa::HowardShopSystem);