#pragma once

#include "Wiwa/ecs/systems/System.h"
#include "Wiwa/ecs/components/Transform2D.h"
#include <Wiwa/utilities/Reflection.h>

#include "Wiwa/ecs/components/Ui/Button.h"
#include "Wiwa/ecs/components/Ui/Checkbox.h"
#include "Wiwa/ecs/components/Ui/Slider.h"


namespace Wiwa
{
	enum class UiControlState
	{
		DISABLED,
		NORMAL,
		FOCUSED,
		PRESSED,
		SELECTED
	};

	class WI_API UiSystem : public System
	{
	private:

	public:
		UiSystem();
		~UiSystem();

		void OnAwake() override;

		void OnInit() override;

		void OnUpdate() override;

		void OnDestroy() override;
	};
}

REGISTER_SYSTEM(Wiwa::UiSystem);