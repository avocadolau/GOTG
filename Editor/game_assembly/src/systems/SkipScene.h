#pragma once
#include <Wiwa/ecs/systems/System.h>
namespace Wiwa
{
class SkipScene : public System
	{
	public:
		//Called every frame
		virtual void OnUpdate() override;
	};
}
REGISTER_SYSTEM(Wiwa::SkipScene)
