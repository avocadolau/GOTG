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
		NONE = 0, CUBE = 1, SPHERE = 2
	};


	struct WI_API ParticleEmitterComponent {

		//Emitter data			----------------------------------------
		
		struct ColorNode
		{
			float m_percentage;
			glm::vec4 color;
			ColorNode()
			{
				m_percentage = 0.f;
				color = glm::vec4(1, 1, 1, 1);
			}
		};

		ResourceId m_materialId;
		char m_materialPath[128];

		ResourceId m_mesh;
		char m_meshPath[128];

		bool m_meshChanged;
		bool m_materialChanged;

		
		int m_maxParticles;

		int m_activeParticles;

		//spawning
		bool m_loopSpawning;
		float m_spawnRate;
		
		bool m_rangedSpawnAmount;
		int  m_spawnAmount;
		int  m_minSpawnAmount;
		int  m_maxSpawnAmount;


		bool	m_p_rangedSpawnRate;
		float	m_p_minSpawnRate;
		float	m_p_maxSpawnRate;
		//float m_spawnDelay;

		//if it starts counting down from start (if using active over time)
		bool	m_startActive;
		bool	m_active;
		bool	m_activeTimeChanged;
		bool	m_rangedTimeActive;
		bool	m_activeOverTime;
		float	m_ActiveTimer;
		float	m_initialTimeActive;
		float	m_minInitialTimeActive;
		float	m_maxInitialTimeActive;

		ParticleSpawnVolume m_spawnVolume;

		EntityId emitterOwner;
		//Particle Data			----------------------------------------

		//general data
		float	m_p_lifeTime;
		bool	m_p_rangedLifeTime;
		float	m_p_minLifeTime;
		float	m_p_maxLifeTime;


		bool m_billboardActive;


		//transform data

		//Follow emitter options only available on world space.

		//position & translation
		//bool m_p_followEmitterPosOnlyOnSpawn;
		bool		m_p_followEmitterPosition;
		bool		m_p_followEmitterPositionSpawn;
		glm::vec3	m_p_initialPosition;
		glm::vec3	m_p_initialPositionSphCenter;
		float		m_p_initialPositionSphRadius;
		glm::vec3	m_p_initialPositionBoxA;
		glm::vec3	m_p_initialPositionBoxB;
		bool		m_p_positionTowardsPoint;
		glm::vec3	m_p_positionTowardsPointPos;
		float		m_p_translationTime;

		glm::vec3	m_p_initialVelocity;
		bool		m_p_rangedVelocity;
		glm::vec3	m_p_minVelocity;
		glm::vec3	m_p_maxVelocity;

		bool m_p_useGravity;
		float m_p_gravity;

		//glm::vec3 particle_m_initialAcceleration;

		//rotation
		//bool m_p_followEmitterRotOnlyOnSpawn;
		bool		m_p_followEmitterRotation;
		bool		m_p_followEmitterRotationSpawn;
		glm::vec3	m_p_initialRotation;
		bool		m_p_rangedInitialRotation;
		glm::vec3	m_p_minInitialRotation;
		glm::vec3	m_p_maxInitialRotation;

		glm::vec3	m_p_initialAngularVelocity;
		bool		m_p_rangedAngularVelocity;
		glm::vec3	m_p_minInitialAngularVelocity;
		glm::vec3	m_p_maxInitialAngularVelocity;
		float		m_p_rotateTime;

		bool		m_p_rotationOverTime;
		float		m_p_rotationOverTimePerStart;
		float		m_p_rotationOverTimePerEnd;
		glm::vec3	m_p_rotationOverTimeStart;
		glm::vec3	m_p_rotationOverTimeEnd;
		


		glm::vec3	m_p_initialScale;
		bool		m_p_rangedInitialScale;
		glm::vec3	m_p_minInitialScale;
		glm::vec3	m_p_maxInitialScale;

		bool		m_p_scaleOverTime;
		float		m_p_scaleOverTimePerStart;
		float		m_p_scaleOverTimePerEnd;
		glm::vec3	m_p_scaleOverTimeStart;
		glm::vec3	m_p_scaleOverTimeEnd;

		glm::vec3	m_p_initialGrowthVelocity;
		bool		m_p_rangedGrowthVelocity;
		glm::vec3	m_p_minInitialGrowthVelocity;
		glm::vec3	m_p_maxInitialGrowthVelocity;


		ColorNode m_p_colorsOverLifetime[6] = {ColorNode()};
		int m_colorsUsed;

		bool m_useAdditiveBlending;
		bool m_p_positionFollowsRotation;
		bool m_deactivateFaceCulling;

		bool m_p_rangedSpawnDelay;
		float m_p_spawnDelay;
		float m_p_minSpawnDelay;
		float m_p_maxSpawnDelay;

		bool m_p_positionFollowsRotationX;
		bool m_p_positionFollowsRotationY;
		bool m_p_positionFollowsRotationZ;

		bool m_p_followEmitterRotationX;
		bool m_p_followEmitterRotationY;
		bool m_p_followEmitterRotationZ;

		ColorNode temp[20] = { ColorNode() }; //--> to fix colors later

		bool m_p_growUniformly;
		float m_p_uniformGrowthVal;
		float m_p_minUniformGrowthVal;
		float m_p_maxUniformGrowthVal;

		bool m_destroyOnFinishActive;
		// todo					----------------------------------------
		// 
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