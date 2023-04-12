#include "wipch.h"
#include "ParticleSystem.h"
#include "glew.h"
#include "Wiwa/ecs/components/Mesh.h"

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
				//calculate everything
				particle.position += particle.velocity * dt;





				// Convert rotation angles from degrees to radians
				glm::vec3 rotationRad = glm::radians(particle.rotation);

				// Create transformation matrix
				glm::mat4 transform = glm::mat4(1.0f);
				transform = glm::translate(transform, particle.position);
				transform = glm::rotate(transform, rotationRad.x, glm::vec3(1.0f, 0.0f, 0.0f));
				transform = glm::rotate(transform, rotationRad.y, glm::vec3(0.0f, 1.0f, 0.0f));
				transform = glm::rotate(transform, rotationRad.z, glm::vec3(0.0f, 0.0f, 1.0f));
				transform = glm::scale(transform, particle.scale);

				//pass transformation matrix
				particle.transform = transform;

			}
			else {

			}
		}
	}
	void ParticleSystem::OnDestroy()
	{
	}
	void ParticleSystem::Render()
	{

		Transform3D* t3d = GetComponent<Transform3D>();

		glBlendFunc(GL_SRC_ALPHA, GL_ONE);
		m_Material->Bind();
		for (unsigned int i = 0; i < m_MaxParticles; ++i)
		{
			Particle& particle = m_Particles[i];
			if (particle.life_time > 0.0f)
			{
			


				//m_Material->getShader()->SetMVP(particle.transform,);
				glBindVertexArray(m_VAO);
				glDrawArrays(GL_TRIANGLES, 0, 6);
				glBindVertexArray(0);
			}
		}
		glBlendFunc(GL_SRC_ALPHA, GL_ONE_MINUS_SRC_ALPHA);
	}

	void ParticleSystem::SetValues(ParticleEmitterComponent settings)
	{

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
		for (unsigned int i = m_LastUsedParticle; i < m_MaxParticles; ++i) {
			if (m_Particles[i].life_time <= 0.0f) {
				m_LastUsedParticle = i;
				return i;
			}
		}
		// otherwise, do a linear search
		for (unsigned int i = 0; i < m_LastUsedParticle; ++i) {
			if (m_Particles[i].life_time <= 0.0f) {
				m_LastUsedParticle = i;
				return i;
			}
		}
		// override first particle if all others are alive
		m_LastUsedParticle = 0;
		return 0;
	}	
}