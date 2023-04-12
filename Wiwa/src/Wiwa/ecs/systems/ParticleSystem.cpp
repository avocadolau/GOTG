#include "wipch.h"
#include "ParticleSystem.h"
#include "glew.h"

namespace Wiwa {
	ParticleSystem::ParticleSystem()
	{
	}
	ParticleSystem::~ParticleSystem()
	{
	}
	void ParticleSystem::OnAwake()
	{
	}
	void ParticleSystem::OnInit()
	{
		// init particle struct
		m_Particles.reserve(m_MaxParticles);
		for (int i = 0; i < m_MaxParticles; i++)
		{
			m_Particles[i] = Particle();
		}
		m_VAO = GetVAO(m_ParticleMesh);
	}
	void ParticleSystem::OnUpdate()
	{
		for (unsigned int i = 0; i < m_MaxParticles; ++i)
		{
			Particle& particle = m_Particles[i];
			float dt = Time::GetRealTimeSinceStartup() * 0.001f;

			particle.life_time -= dt;

			if (particle.life_time > 0.0f)
			{	
				particle.position -= particle.velocity * dt;

			}
		}

	}
	void ParticleSystem::OnDestroy()
	{
	}
	void ParticleSystem::Render()
	{
		glBlendFunc(GL_SRC_ALPHA, GL_ONE);
		m_Material.Bind();
		for (unsigned int i = 0; i < m_MaxParticles; ++i)
		{
			Particle& particle = m_Particles[i];
			if (particle.life_time > 0.0f)
			{
				m_Material.SetUniformData("offset", particle.position);
				m_Material.SetUniformData("color", particle.color);
				glBindVertexArray(m_VAO);
				glDrawArrays(GL_TRIANGLES, 0, 6);
				glBindVertexArray(0);
			}
		}
		glBlendFunc(GL_SRC_ALPHA, GL_ONE_MINUS_SRC_ALPHA);
	}

	void ParticleSystem::SpawnParticle(Particle& particle, Transform3D& emmiter, glm::vec2 offset)
	{
		particle.position = emmiter.localPosition;
		particle.rotation = emmiter.localRotation;
		particle.scale = emmiter.localScale;
		
	}

	unsigned int ParticleSystem::FirstUnusedParticle()
	{
		// search from last used particle, this will usually return almost instantly
		for (unsigned int i = lastUsedParticle; i < m_MaxParticles; ++i) {
			if (m_Particles[i].life_time <= 0.0f) {
				lastUsedParticle = i;
				return i;
			}
		}
		// otherwise, do a linear search
		for (unsigned int i = 0; i < lastUsedParticle; ++i) {
			if (m_Particles[i].life_time <= 0.0f) {
				lastUsedParticle = i;
				return i;
			}
		}
		// override first particle if all others are alive
		lastUsedParticle = 0;
		return 0;
	}	
}