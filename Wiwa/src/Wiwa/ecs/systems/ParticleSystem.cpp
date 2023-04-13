#include "wipch.h"
#include "ParticleSystem.h"
#include "glew.h"
#include "Wiwa/ecs/components/Mesh.h"
#include <Wiwa/core/Renderer3D.h>

#include <Wiwa/ecs/EntityManager.h>
#include <Wiwa/utilities/render/Material.h>
#include <Wiwa/utilities/render/CameraManager.h>
#include <Wiwa/utilities/render/LightManager.h>
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
		ParticleEmitterComponent* emmiter = GetComponent<ParticleEmitterComponent>();
		Transform3D* t3d = GetComponent<Transform3D>();

		ResourceId meshid = Wiwa::Resources::Load<Model>(emmiter->m_meshPath);
		m_Model = Wiwa::Resources::GetResourceById<Model>(meshid);

		ResourceId matid = Wiwa::Resources::Load<Material>(emmiter->m_materialPath);
		m_Material = Wiwa::Resources::GetResourceById<Material>(matid);

		m_SpawnRate = emmiter->m_spawnRate;

		glm::vec3 initPosition = emmiter->m_p_initialPosition + t3d->localPosition;
		glm::vec3 initRotation = emmiter->m_p_initialRotation + t3d->localRotation;
		glm::vec3 initScale = emmiter->m_p_initialScale + t3d->localScale;


		// init particle struct
		m_Particles.reserve(m_MaxParticles);
		for (int i = 0; i < m_MaxParticles; i++)
		{
			m_Particles[i] = Particle(emmiter->m_p_lifeTime,
									initPosition,initRotation,initScale,
									emmiter->m_p_initialVelocity,glm::vec4 (1.0f, 1.0f, 1.0f, 1.0f));
		}
	}
	void ParticleSystem::OnUpdate()
	{
		ParticleEmitterComponent* emmiter = GetComponent<ParticleEmitterComponent>();

		m_SpawnRate -= Time::GetDeltaTime() * 0.001;
		if (m_SpawnRate > 0 )
		{
			for (unsigned int i = 0; i < m_AvailableParticles; ++i)
			{
				int unusedParticle = FirstUnusedParticle();
				SpawnParticle(m_Particles[unusedParticle]);
			}

			m_SpawnRate = emmiter->m_spawnRate;
		}

		WI_CORE_INFO("emitter volume type = {0}", emmiter->m_spawnVolume);

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
			else if (emmiter->m_loopSpawning) 
			{
				m_AvailableParticles++;
			}
		}

		Render();
	}
	void ParticleSystem::OnDestroy()
	{
		m_Model = nullptr;
		m_Material = nullptr;
	}
	void ParticleSystem::Render()
	{
		Transform3D* t3d = GetComponent<Transform3D>();

		Renderer3D& r3d = Application::Get().GetRenderer3D();
		CameraManager& man = Wiwa::SceneManager::getActiveScene()->GetCameraManager();
		EntityManager& eman = Wiwa::SceneManager::getActiveScene()->GetEntityManager();
		LightManager& lman = Wiwa::SceneManager::getActiveScene()->GetLightManager();

		size_t cameraCount = man.getCameraSize();
		std::vector<CameraId>& cameras = man.getCameras();

		for (size_t i = 0; i < cameraCount; i++)
		{
			CameraId cam_id = cameras[i];
			Camera* camera = man.getCamera(cam_id);

			if (camera->cull && !camera->frustrum.IsBoxVisible(m_Model->boundingBox.getMin(), m_Model->boundingBox.getMax()))
				return;

			for (unsigned int i = 0; i < m_MaxParticles; ++i)
			{
				Particle& particle = m_Particles[i];
				if (particle.life_time > 0.0f)
				{
					r3d.RenderMesh(m_Model, particle.transform, m_Material, lman.GetDirectionalLight(), lman.GetPointLights(), lman.GetSpotLights(), false, camera);
				}
			}
			
		}
		if (man.editorCamera)
		{
			for (unsigned int i = 0; i < m_MaxParticles; ++i)
			{
				Particle& particle = m_Particles[i];
				if (particle.life_time > 0.0f)
				{
					r3d.RenderMesh(m_Model, particle.transform, m_Material, lman.GetDirectionalLight(), lman.GetPointLights(), lman.GetSpotLights(), false, man.editorCamera);
				}
			}
		}
		//glBlendFunc(GL_SRC_ALPHA, GL_ONE);
		//m_Material->Bind();
		//for (unsigned int i = 0; i < m_MaxParticles; ++i)
		//{
		//	Particle& particle = m_Particles[i];
		//	if (particle.life_time > 0.0f)
		//	{
		//	


		//		m_Material->getShader()->SetMVP(particle.transform,camera,);
		//		glBindVertexArray(m_VAO);
		//		glDrawArrays(GL_TRIANGLES, 0, 6);
		//		glBindVertexArray(0);
		//	}
		//}
		//glBlendFunc(GL_SRC_ALPHA, GL_ONE_MINUS_SRC_ALPHA);
	}

	void ParticleSystem::SetValues(ParticleEmitterComponent settings)
	{

	}

	void ParticleSystem::SpawnParticle(Particle& particle)
	{

		ParticleEmitterComponent* particleemmiter = GetComponent<ParticleEmitterComponent>();
		Transform3D* t3d = GetComponent<Transform3D>();


		glm::vec3 initPosition = particleemmiter->m_p_initialPosition + t3d->localPosition;
		glm::vec3 initRotation = particleemmiter->m_p_initialRotation + t3d->localRotation;
		glm::vec3 initScale = particleemmiter->m_p_initialScale + t3d->localScale;



		particle.position = initPosition;
		particle.rotation = initRotation;
		particle.scale = initScale;

		particle.velocity = particleemmiter->m_p_initialVelocity;

		particle.life_time = particleemmiter->m_p_lifeTime;

		m_AvailableParticles--;
		
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