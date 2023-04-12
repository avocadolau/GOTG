#pragma once
#pragma warning(disable : 4302)
#include <Wiwa/core/Core.h>
#include "Wiwa/ecs/EntityManager.h"
#include <glm/glm.hpp>
#include <Wiwa/utilities/Reflection.h>
#include <Wiwa/core/Resources.h>

namespace Wiwa {

	enum ParticleSimulationSpace
	{
		//The simulation depends on the Emitter's transform.
		LOCAL,
		//The simulation is independent from the Emitter's transform.
		WORLD,
	};

	struct WI_API ParticleEmitterComponent {

		//Emitter data			----------------------------------------
		
		ResourceId m_materialId;
		char m_materialPath[128];

		ResourceId m_mesh;

		unsigned int m_maxParticles;

		//spawning
		bool m_loopSpawning;
		float m_spawnRate;
		float m_spawnDelay;

		bool m_active;

		


		//Particle Data			----------------------------------------

		//general data
		float m_particle_maxLifeTime;

		bool m_billboardActive;


		//transform data

		//Follow emitter options only available on world space.

		//position & translation
		bool m_p_followEmitterPosOnlyOnSpawn;
		glm::vec3 m_p_initialPositionOffset;
		glm::vec3 m_p_initialVelocity;
		//glm::vec3 particle_m_initialAcceleration;

		//rotation
		bool m_p_followEmitterRotOnlyOnSpawn;
		glm::vec3 m_p_initialRotation;
		glm::vec3 m_p_initialAngularVelocity;

		//size
		bool m_p_followEmitterScaleOnlyOnSpawn;
		glm::vec3 m_p_initialScale;
		glm::vec3 m_p_initialGrowthVelocity;


		std::vector<glm::vec4> m_p_colorsOverLifetime;

		

		// todo					----------------------------------------
		// 
		// ranged data!!!!
		// 
		// animations
		// accelerations
		// data over lifetime
		// emit particles over a certain time
		// gravity?
		// 

	};
}

REFLECTION_BEGIN(Wiwa::ParticleEmitterComponent)

	REFLECT_MEMBER(m_materialId);
	REFLECT_MEMBER(m_materialPath);
	REFLECT_MEMBER(m_mesh);
	REFLECT_MEMBER(m_maxParticles);
	REFLECT_MEMBER(m_loopSpawning);
	REFLECT_MEMBER(m_spawnRate);
	REFLECT_MEMBER(m_spawnDelay);
	REFLECT_MEMBER(m_active);
	REFLECT_MEMBER(m_particle_maxLifeTime);
	REFLECT_MEMBER(m_billboardActive);
	REFLECT_MEMBER(m_p_followEmitterPosOnlyOnSpawn);
	REFLECT_MEMBER(m_p_initialPositionOffset);
	REFLECT_MEMBER(m_p_initialVelocity);
	REFLECT_MEMBER(m_p_followEmitterRotOnlyOnSpawn);
	REFLECT_MEMBER(m_p_initialRotation);
	REFLECT_MEMBER(m_p_initialAngularVelocity);
	REFLECT_MEMBER(m_p_followEmitterScaleOnlyOnSpawn);
	REFLECT_MEMBER(m_p_initialScale);
	REFLECT_MEMBER(m_p_initialGrowthVelocity);
	REFLECT_MEMBER(m_p_colorsOverLifetime);

REFLECTION_END;