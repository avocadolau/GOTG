#pragma once
#include <Wiwa/ecs/systems/System.h>
#include <Wiwa/ecs/components/ai/AINavMesh.h>
#include <Wiwa/utilities/Reflection.h>

namespace Wiwa {
	class WI_API AINavMeshLoadingSystem : public System {
	private:

	public:
		AINavMeshLoadingSystem();
		~AINavMeshLoadingSystem();

		void OnSystemAdded() override;
	};
}

REGISTER_SYSTEM(Wiwa::AINavMeshLoadingSystem);