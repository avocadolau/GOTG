#pragma once
#include <wipch.h>
#include "System.h"
#include <Wiwa/core/Core.h>

#include "Wiwa/ecs/components/Transform3D.h"
#include "Wiwa/ecs/components/ParticleEmitterComponent.h"
#include <Wiwa/utilities/render/shaders/Shader.h>
#include <Wiwa/utilities/render/Uniforms.h>
#include <Wiwa/core/Resources.h>

#include <Wiwa/utilities/Reflection.h>

#include <glm/glm.hpp>

namespace Wiwa
{
	struct Particle
	{
		Particle() :
			life_time(0.0f),
			life_time_start(0.0f),
			life_percentage(0.0f),
			position(0.0f),
			startPosition(0.0f),
			rotation(0.0f),
			angularVelocity(0.0f),
			scale(0.0f),
			growthVelocity(0.0f),
			velocity(0.0f),
			color(0.0f) {};

		Particle(float _life, glm::vec3 _startPosition, glm::vec3 _rotation, glm::vec3 _angularVelocity, glm::vec3 _scale, glm::vec3 _growthVelocity, glm::vec3 _velocity, glm::vec4 _color)
		{
			life_time = _life;
			life_time_start = _life;
			life_percentage = 0.0f;
			startPosition = _startPosition;
			position = startPosition;
			rotation = _rotation;
			angularVelocity = _angularVelocity;
			scale = _scale;
			growthVelocity = _growthVelocity;
			velocity = _velocity;
			color = _color;		
		};
		
		float life_time = 0;
		float life_time_start = 0;
		float life_percentage = 0;
		glm::vec3 position;
		glm::vec3 startPosition;
		glm::vec3 velocity;
		glm::vec3 rotation;
		glm::vec3 angularVelocity;
		glm::vec3 scale;
		glm::vec3 growthVelocity;
		glm::vec4 color;
		glm::mat4 transform;
	};

	class WI_API ParticleSystem : public System
	{
	public: 
		ParticleSystem();
		~ParticleSystem();

		void OnSystemAdded() override;

		void OnAwake() override;

		void OnInit() override;

		void OnUpdate() override;

		void OnDestroy() override;

		void Render();

		void SetValues(ParticleEmitterComponent settings);

		void SpawnParticle(Particle& particle);

		void UpdateParticleLife(Particle& particle, float deltaTime);
		void SetParticleLifeTime(Particle& particle, float lifeTime);

		unsigned int FirstUnusedParticle();

		unsigned int GetVAO() { return m_VAO; }

		unsigned int  m_MaxParticles = 10;

		bool m_Loop;

		float m_Duration = 0.0f;

		glm::vec4 ColorInterpolation(glm::vec4 col1, glm::vec4 col2, float percentage);

	private:
		unsigned int m_VAO;

		unsigned int m_LastUsedParticle = 0;
		
		unsigned int m_AvailableParticles = 0;

		float m_SpawnTimer;

		std::vector<Particle> m_Particles;

		Material* m_Material;

		Model* m_Model;
	};
}
REGISTER_SYSTEM(Wiwa::ParticleSystem);

