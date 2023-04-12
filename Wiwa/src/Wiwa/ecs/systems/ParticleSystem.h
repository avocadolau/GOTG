#pragma once
#include <wipch.h>
#include "System.h"
#include <Wiwa/core/Core.h>

#include "Wiwa/ecs/components/Transform3D.h"
#include <Wiwa/utilities/render/shaders/Shader.h>
#include <Wiwa/utilities/render/Uniforms.h>
#include <Wiwa/core/Resources.h>

#include <glm/glm.hpp>

#include <vector>
#include <map>

namespace Wiwa
{
	enum class ParticleMeshType
	{
		QUAD,
		CUBE,
		CONE,
		CYLYNDER,
		SPHERE,
		UNDEFINED
	};

	struct Particle
	{
		Particle() :
			life_time(0.0f),
			position(0.0f),
			rotation(0.0f),
			scale(0.0f),
			velocity(0.0f),
			color(0.0f) {};
		
		float life_time = 0;
		glm::vec3 position;
		glm::vec3 rotation;
		glm::vec3 scale;
		glm::vec3 velocity;
		glm::vec4 color;
	};

	class WI_API ParticleSystem : public System
	{
	public: 
		ParticleSystem();
		~ParticleSystem();

		void OnAwake() override;

		void OnInit() override;

		void OnUpdate() override;

		void OnDestroy() override;

		void Render();


		void SpawnParticle(Particle& particle, Transform3D& emmiter, glm::vec2 offset);

		unsigned int GetVAO() { return m_VAO; }

		unsigned int  m_MaxParticles;
		unsigned int lastUsedParticle = 0;
		unsigned int FirstUnusedParticle();

	private:
		unsigned int m_VAO;
		std::vector<Particle> m_Particles;
		ParticleMeshType m_ParticleMesh = ParticleMeshType::UNDEFINED;
		Material m_Material;
	};

}


