#pragma once

#include "System.h"
#include <vector>
#include <map>

#include <Wiwa/utilities/Reflection.h>

namespace Wiwa {




	class WI_API ParticleEmitterExecutor : public System {
	private:

	public:
		ParticleEmitterExecutor();
		~ParticleEmitterExecutor();

		void OnAwake() override;

		void OnInit() override;

		void OnUpdate() override;

		void OnDestroy() override;

		void OnSystemAdded() override;

		void OnSystemRemoved() override;

		void EmitBatch(EntityId eid);
		void Play(EntityId eid,float delaySec);
		void Stop(EntityId eid);
	private:

		EntityManager::ComponentIterator m_emitterComponent;
		ParticleEmitter* m_emitter;


	};
}

REGISTER_SYSTEM(Wiwa::ParticleEmitterExecutor);