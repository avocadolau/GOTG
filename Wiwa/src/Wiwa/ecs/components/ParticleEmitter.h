#pragma once
#pragma warning(disable : 4302)
#include <Wiwa/core/Core.h>
//#include <Wiwa/utilities/math/Vector3f.h>
#include <glm/glm.hpp>
#include <Wiwa/utilities/Reflection.h>
#include <Wiwa/core/Resources.h>

namespace Wiwa {


	struct WI_API ParticleEmitter {

		Material m_material;

		//Emitter parameters
		float	particle_rate;
		bool	particle_rate_isRanged;
		float	particle_rate_range[2];

		bool	repeat = true;

		int		particle_amount;
		bool	particle_amount_isRanged;
		int		particle_amount_range[2];

		float	particle_initial_lifetime;
		float	particle_lifetime;
		bool	particle_lifetime_isRanged;
		float	particle_lifetime_range[2];

		bool isPlaying = true;

		//std::list<ColorTime> particle_color;

		int nextPos = 100;
		glm::vec4 nextColor = glm::vec4(0.0f, 0.0f, 0.0f, 1.0f);

		//position
		glm::vec3	particle_startingPosition;
		bool		particle_startingPosition_isRanged;
		glm::vec3	particle_startingPosition_range[2];

		glm::vec3	particle_velocity;
		bool		particle_velocity_isRanged;
		glm::vec3	particle_velocity_range[2];

		glm::vec3	particle_acceleration;
		bool		particle_acceleration_isRanged;
		glm::vec3	particle_acceleration_range[2];


		//rotation
		glm::vec3	particle_startingRotation;
		bool		particle_startingRotation_isRanged;
		glm::vec3	particle_startingRotation_range[2];

		
		//scale
		glm::vec3	particle_startingSize = glm::vec3(1,1,1);
		bool		particle_startingSize_isRanged;
		glm::vec3	particle_startingSize_range[2] = {glm::vec3(1,1,1) , glm::vec3(1,1,1)};

		glm::vec3	particle_growthVelocity;
		bool		particle_growthVelocity_isRanged;
		glm::vec3	particle_growthVelocity_range[2];

		glm::vec3	particle_growthAcceleration;
		bool		particle_growthAcceleration_isRanged;
		glm::vec3	particle_growthAcceleration_range[2];

		bool		particle_followEmitterRotation = false;
		bool		particle_followEmitterPosition = false;
		bool		particle_followParticle = false;;

		ResourceId textId1 = -4;
		Image* texture = nullptr;
		std::string texturePath;
	};
}

REFLECTION_BEGIN(Wiwa::ParticleEmitter)

	REFLECT_MEMBER(particle_rate)
	REFLECT_MEMBER(particle_rate_isRanged)
	REFLECT_MEMBER(particle_rate_range)
	REFLECT_MEMBER(particle_amount)
	REFLECT_MEMBER(particle_amount_isRanged)
	REFLECT_MEMBER(particle_amount_range)
	REFLECT_MEMBER(particle_lifetime)
	REFLECT_MEMBER(particle_lifetime_isRanged)
	REFLECT_MEMBER(particle_lifetime_range)
	REFLECT_MEMBER(repeat)
	REFLECT_MEMBER(nextPos)
	REFLECT_MEMBER(nextColor)
	REFLECT_MEMBER(particle_startingPosition)
	REFLECT_MEMBER(particle_startingPosition_isRanged)
	REFLECT_MEMBER(particle_startingPosition_range)
	REFLECT_MEMBER(particle_velocity)
	REFLECT_MEMBER(particle_velocity_isRanged)
	REFLECT_MEMBER(particle_velocity_range)
	REFLECT_MEMBER(particle_acceleration)
	REFLECT_MEMBER(particle_acceleration_isRanged)
	REFLECT_MEMBER(particle_acceleration_range)
	REFLECT_MEMBER(particle_startingRotation)
	REFLECT_MEMBER(particle_startingRotation_isRanged)
	REFLECT_MEMBER(particle_startingRotation_range)
	REFLECT_MEMBER(particle_startingSize)
	REFLECT_MEMBER(particle_startingSize_isRanged)
	REFLECT_MEMBER(particle_startingSize_range)
	REFLECT_MEMBER(particle_growthVelocity)
	REFLECT_MEMBER(particle_growthVelocity_isRanged)
	REFLECT_MEMBER(particle_growthVelocity_range)
	REFLECT_MEMBER(particle_startingSize_range)
	REFLECT_MEMBER(particle_startingSize_range)
	REFLECT_MEMBER(particle_startingSize_range)
	REFLECT_MEMBER(particle_startingSize_range)
	REFLECT_MEMBER(particle_startingSize_range)
	REFLECT_MEMBER(particle_startingSize_range)
	REFLECT_MEMBER(particle_growthAcceleration)
	REFLECT_MEMBER(particle_growthAcceleration_isRanged)
	REFLECT_MEMBER(particle_growthAcceleration_range)
	REFLECT_MEMBER(particle_followEmitterPosition)
	REFLECT_MEMBER(particle_followParticle)

REFLECTION_END;