#pragma once
#pragma warning(disable : 4302)
#include <Wiwa/core/Core.h>
#include "Wiwa/ecs/EntityManager.h"
#include <glm/glm.hpp>
#include <Wiwa/utilities/Reflection.h>
#include <Wiwa/core/Resources.h>

namespace Wiwa {


	struct WI_API ParticleEmitter {

		//variables
		// 
		// material
		// mesh
		// max particles
		// 
		// lifetime
		// size start
		// size end
		// 
		// initial offset position
		// 
		// initial rotation
		// 
		// initial velocity
		// 
		// initial acceleration
		// 
		// spawn inside volume (mesh)
		// 
		// color over lifetime (alejandro)
		// 
		// spawn loop
		// spawn rate
		// 
		// start delay
		// 

		//Emitter data
		ResourceId m_materialId;
		char m_materialPath[128];

		ResourceId m_mesh;

		unsigned int m_maxParticles;

		bool m_loopSpawning;
		float spawnRate;
		float spawnDelay;


		//Particles Data

		//general data
		float m_particle_maxLifetime;

		bool billboardActive;


		//transform data

		//position & translation
		bool m_particle_followEmitterPosition;
		glm::vec3 m_particle_initialPositionOffset;
		glm::vec3 m_particle_initialVelocity;
		//glm::vec3 particle_m_initialAcceleration;

		//rotation
		bool m_particle_followEmitterRotation;
		glm::vec3 m_particle_initialRotation;

		//size
		bool m_particle_followEmitterScale;
		glm::vec3 m_particle_initialScale;
		

		std::vector<Color3f> m_particle_colorsOverLifetime;

	};
}

REFLECTION_BEGIN(Wiwa::ParticleEmitter)



REFLECTION_END;