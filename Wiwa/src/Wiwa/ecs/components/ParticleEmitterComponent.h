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

	enum ParticleSpawnVolume
	{
		NONE, CUBE, SPHERE
	};


	struct WI_API ParticleEmitterComponent {

		//Emitter data			----------------------------------------
		
		ResourceId m_materialId;
		char m_materialPath[128];

		ResourceId m_mesh;
		char m_meshPath[128];

		
		int m_maxParticles;

		//spawning
		bool m_loopSpawning;
		float m_spawnRate;
		bool		m_p_rangedSpawnRate;
		glm::vec3	m_p_minSpawnRate;
		glm::vec3	m_p_maxSpawnRate;
		//float m_spawnDelay;

		bool m_active;

		ParticleSpawnVolume m_spawnVolume;

		//Particle Data			----------------------------------------

		//general data
		float m_p_lifeTime;
		bool		m_p_rangedLifeTime;
		glm::vec3	m_p_minLifeTime;
		glm::vec3	m_p_maxLifeTime;


		bool m_billboardActive;


		//transform data

		//Follow emitter options only available on world space.

		//position & translation
		//bool m_p_followEmitterPosOnlyOnSpawn;
		glm::vec3 m_p_initialPosition;
		bool		m_p_rangedInitialPosition;
		glm::vec3	m_p_minInitialPosition;
		glm::vec3	m_p_maxInitialPosition;

		glm::vec3 m_p_initialVelocity;
		bool		m_p_rangedVelocity;
		glm::vec3	m_p_minVelocity;
		glm::vec3	m_p_maxVelocity;

		//glm::vec3 particle_m_initialAcceleration;

		//rotation
		//bool m_p_followEmitterRotOnlyOnSpawn;
		glm::vec3 m_p_initialRotation;
		bool		m_p_rangedInitialRotation;
		glm::vec3	m_p_minInitialRotation;
		glm::vec3	m_p_maxInitialRotation;

		glm::vec3 m_p_initialAngularVelocity;
		bool		m_p_rangedAngularVelocity;
		glm::vec3	m_p_minAngularVelocity;
		glm::vec3	m_p_maxAngularVelocity;
		//size
		//bool m_p_followEmitterScaleOnlyOnSpawn;
		glm::vec3 m_p_initialScale;
		bool		m_p_rangedInitialScale;
		glm::vec3	m_p_minInitialScale;
		glm::vec3	m_p_maxInitialScale;

		glm::vec3 m_p_initialGrowthVelocity;
		bool		m_p_rangedGrowthVelocity;
		glm::vec3	m_p_minInitialGrowthVelocity;
		glm::vec3	m_p_maxInitialGrowthVelocity;


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
	//REFLECT_MEMBER(m_spawnDelay);
	REFLECT_MEMBER(m_active);
	REFLECT_MEMBER(m_p_lifeTime);
	REFLECT_MEMBER(m_billboardActive);
	//REFLECT_MEMBER(m_p_followEmitterPosOnlyOnSpawn);
	REFLECT_MEMBER(m_p_initialPosition);
	REFLECT_MEMBER(m_p_initialVelocity);
	//REFLECT_MEMBER(m_p_followEmitterRotOnlyOnSpawn);
	REFLECT_MEMBER(m_p_initialRotation);
	REFLECT_MEMBER(m_p_initialAngularVelocity);
	//REFLECT_MEMBER(m_p_followEmitterScaleOnlyOnSpawn);
	REFLECT_MEMBER(m_p_initialScale);
	REFLECT_MEMBER(m_p_initialGrowthVelocity);
	REFLECT_MEMBER(m_p_colorsOverLifetime);

REFLECTION_END;