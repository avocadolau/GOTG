#pragma once
#include <wipch.h>
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
		bool is_active;
		
		float life_time = 0;
		
		glm::vec4 Color;
		

	};

	class ParticleEmmiter
	{
	public: 
		ParticleEmmiter();
		~ParticleEmmiter();

		void Awake();
		void Init();
		void Update();
		void CleanUp();
		void Render();

		unsigned int GetVAO(ParticleMeshType type);

		unsigned int  m_MaxParticles;


	private:
		unsigned int m_VAO;
		std::vector<Particle> m_Particles;
		ParticleMeshType m_ParticleMesh = ParticleMeshType::UNDEFINED;

	};

}


