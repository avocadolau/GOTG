#pragma once

#include "System.h"
#include <vector>
#include <map>

#include <Wiwa/utilities/Reflection.h>

class Camera;
class AnimationParticles;

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

		void ScreenAlign(std::shared_ptr<ParticleBillboard> particle);

		void EmitBatch();
		void Play();
		void Stop();
	private:

		EntityManager::ComponentIterator m_emitterComponent;
		ParticleEmitter* m_emitter;


	};
}

REGISTER_SYSTEM(Wiwa::ParticleEmitterExecutor);