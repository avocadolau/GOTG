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
	void ParticleSystem::OnSystemAdded()
	{

		ParticleEmitterComponent* emitter = GetComponent<ParticleEmitterComponent>();
		Transform3D* t3d = GetComponent<Transform3D>();

		ResourceId meshid = Wiwa::Resources::Load<Model>(emitter->m_meshPath);
		m_Model = Wiwa::Resources::GetResourceById<Model>(meshid);

		ResourceId matid = Wiwa::Resources::Load<Material>(emitter->m_materialPath);
		m_Material = Wiwa::Resources::GetResourceById<Material>(matid);

		m_MaxParticles = emitter->m_maxParticles;
		emitter->m_meshChanged = false;

		//init with delay
		float delay = 0;
		m_SpawnTimer = delay;

		glm::vec3 initPosition = emitter->m_p_initialPosition + t3d->localPosition;
		glm::vec3 initRotation = emitter->m_p_initialRotation + t3d->localRotation;
		glm::vec3 initScale = emitter->m_p_initialScale + t3d->localScale;
		glm::vec3 initZero(0, 0, 0);

		// init particle struct
		m_Particles.resize(m_MaxParticles);
		for (int i = 0; i < m_MaxParticles; i++)
		{
			m_Particles[i] = Particle(0, initZero, initZero, initZero, initZero, initZero, initZero, glm::vec4(1.0f, 1.0f, 1.0f, 1.0f));
		}


	}
	void ParticleSystem::OnAwake()
	{
	}
	void ParticleSystem::OnInit()
	{
		
	}
	void ParticleSystem::OnUpdate()
	{
		ParticleEmitterComponent* emitter = GetComponent<ParticleEmitterComponent>();

		if (emitter->m_meshChanged)
		{
			ResourceId meshid = Wiwa::Resources::Load<Model>(emitter->m_meshPath);
			m_Model = Wiwa::Resources::GetResourceById<Model>(meshid);
			emitter->m_meshChanged = false;

		}
		
		if (m_MaxParticles != emitter->m_maxParticles)
		{
			glm::vec3 initZero(0, 0, 0);

			m_Particles.clear();
			m_MaxParticles = emitter->m_maxParticles;
			
			m_Particles.resize(m_MaxParticles);
			
			for (int i = 0; i < m_MaxParticles; i++)
			{
				m_Particles[i] = Particle(0, initZero, initZero, initZero, initZero, initZero, initZero, glm::vec4(1.0f, 1.0f, 1.0f, 1.0f));
			}

		}



		emitter->m_activeParticles = 0;

		if (emitter->m_active)
		{

			m_SpawnTimer -= Time::GetDeltaTime() * 0.001;

			if (m_SpawnTimer < 0)
			{
				for (unsigned int i = 0; i < emitter->m_spawnAmount; ++i)
				{
					int unusedParticle = FirstUnusedParticle();
					SpawnParticle(m_Particles[unusedParticle]);
				}

				m_SpawnTimer = emitter->m_spawnRate;

				if (emitter->m_p_rangedSpawnRate)
				{
					m_SpawnTimer = Wiwa::Math::RandomRange(emitter->m_p_minSpawnRate, emitter->m_p_maxSpawnRate);
				}

			}

			//WI_CORE_INFO("emitter volume type = {0}", emitter->m_spawnVolume);




			int activeParticles = 0;
			for (unsigned int i = 0; i < m_MaxParticles; ++i)
			{
				Particle& particle = m_Particles[i];
				float dt = Time::GetDeltaTime() * 0.001f;

				if (particle.life_time > 0.0f)
				{
					UpdateParticleLife(particle, dt);

					activeParticles++;

					glm::vec4 color = emitter->m_p_colorsOverLifetime[0].color;

					//Shader* shader = m_Material->getShader();

					//shader->setUniformFloat(shader->getUniformLocation("u_DissolveAmount"), particle.life_percentage);
					//shader->setUniformVec4(shader->getUniformLocation("u_Color"), particle.color);

					Uniform* u_color = m_Material->getUniform("u_Color");
					if (u_color != nullptr)
						u_color->setData(color, UniformType::fVec4);

					Uniform* u_life = m_Material->getUniform("u_LifeTime");
					if (u_life != nullptr)
						u_life->setData(particle.life_percentage, UniformType::Float);
					// 
					//u_Time
					//u_

					//calculate everything
					particle.position += particle.velocity * dt;

					if (emitter->m_p_scaleOverTime)
					{
						particle.scale = emitter->m_p_scaleOverTimeStart * (1 - particle.life_percentage) + emitter->m_p_scaleOverTimeEnd * particle.life_percentage;
					}
					else
					{
						particle.scale += particle.growthVelocity * dt;
					}

					if (emitter->m_p_rotationOverTime)
					{
						particle.rotation = emitter->m_p_rotationOverTimeStart * (1 - particle.life_percentage) + emitter->m_p_rotationOverTimeEnd * particle.life_percentage;
					}
					else
					{
						particle.rotation += particle.angularVelocity * dt;
					}


					// Create transformation matrix
					glm::mat4 transform = glm::mat4(1.0f);

					if (emitter->m_billboardActive)
					{
						Wiwa::CameraManager& cm = Wiwa::SceneManager::getActiveScene()->GetCameraManager();
						CameraId cameraId;
						cameraId = cm.getActiveCameraId();
						Wiwa::Camera* cam = cm.getCamera(cameraId);

						//fix rotation
						glm::mat4 rotationMatrix = cam->getView();
						glm::mat4 billboardMatrix = glm::inverse(rotationMatrix);

						glm::vec3 rotationRad = glm::vec3(0, 0, 0);
						rotationRad.x = -glm::asin(rotationMatrix[1][2]);
						rotationRad.y = glm::atan(rotationMatrix[0][2], rotationMatrix[2][2]);
						rotationRad.z = glm::atan(rotationMatrix[1][0], rotationMatrix[1][1]);

						transform = glm::translate(transform, particle.position);

						//transform = billboardMatrix * transform;

						transform = glm::rotate(transform, rotationRad.y, glm::vec3(0.0f, 1.0f, 0.0f));
						transform = glm::rotate(transform, particle.rotation.z, glm::vec3(0.0f, 0.0f, 1.0f));
						transform = glm::rotate(transform, rotationRad.x, glm::vec3(1.0f, 0.0f, 0.0f));

						//debug
						rotationRad.x = glm::degrees(glm::asin(rotationMatrix[1][2]));
						rotationRad.y = glm::degrees(glm::atan(rotationMatrix[0][2], rotationMatrix[2][2]));
						rotationRad.z = glm::degrees(glm::atan(rotationMatrix[1][0], rotationMatrix[1][1]));

						//pass transformation matrix
						particle.transform = transform;
						
					}
					else
					{
						// Convert rotation angles from degrees to radians
						glm::vec3 rotationRad = particle.rotation;


						transform = glm::translate(transform, particle.position);
						transform = glm::rotate(transform, rotationRad.x, glm::vec3(1.0f, 0.0f, 0.0f));
						transform = glm::rotate(transform, rotationRad.y, glm::vec3(0.0f, 1.0f, 0.0f));
						transform = glm::rotate(transform, rotationRad.z, glm::vec3(0.0f, 0.0f, 1.0f));
						transform = glm::scale(transform, particle.scale);

						//pass transformation matrix
						particle.transform = transform;
					}
				}
			}

			emitter->m_activeParticles = activeParticles;
			Render();
		}
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

		glEnable(GL_BLEND);
		glDepthMask(GL_FALSE);
		glBlendFunc(GL_SRC_ALPHA, GL_ONE_MINUS_SRC_ALPHA);

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
		glDepthMask(GL_TRUE);
		glDisable(GL_BLEND);
	}

	void ParticleSystem::SetValues(ParticleEmitterComponent settings)
	{

	}

	void ParticleSystem::SetParticleLifeTime(Particle& particle, float lifeTime)
	{
		particle.life_time_start = lifeTime;
		particle.life_time = lifeTime;
		particle.life_percentage = 0.f;
	}
	void ParticleSystem::UpdateParticleLife(Particle& particle, float deltaTime)
	{
		particle.life_time -= deltaTime;
		particle.life_percentage = 1 - particle.life_time / particle.life_time_start;
	}

	void ParticleSystem::SpawnParticle(Particle& particle)
	{

		ParticleEmitterComponent* emitter = GetComponent<ParticleEmitterComponent>();
		Transform3D* t3d = GetComponent<Transform3D>();

		glm::vec3 initPosition(0, 0, 0);
		glm::vec3 initRotation(0, 0, 0);
		glm::vec3 initScale(0, 0, 0);

		//initial position


		switch (emitter->m_spawnVolume)
		{
		case Wiwa::ParticleSpawnVolume::NONE:
		{
			initPosition = emitter->m_p_initialPosition + t3d->localPosition;
		}
		break;
		case Wiwa::ParticleSpawnVolume::CUBE:
		{
			float x = Wiwa::Math::RandomRange(emitter->m_p_InitialPositionBoxA.x, emitter->m_p_InitialPositionBoxB.x);
			float y = Wiwa::Math::RandomRange(emitter->m_p_InitialPositionBoxA.y, emitter->m_p_InitialPositionBoxB.y);
			float z = Wiwa::Math::RandomRange(emitter->m_p_InitialPositionBoxA.z, emitter->m_p_InitialPositionBoxB.z);

			initPosition = emitter->m_p_initialPosition + t3d->localPosition + glm::vec3(x, y, z);
		}
		break;
		case Wiwa::ParticleSpawnVolume::SPHERE:
		{
			//Spawn in sphere

			float x = Wiwa::Math::RandomRange(1.0f,-1.0f);
			float y = Wiwa::Math::RandomRange(1.0f,-1.0f);
			float z = Wiwa::Math::RandomRange(1.0f,-1.0f);


			initPosition = emitter->m_p_initialPosition + t3d->localPosition + glm::normalize(glm::vec3(x, y, z)) * Wiwa::Math::RandomRange(0.0f, emitter->m_p_initialPositionSphRadius);

		}
		break;

		default:
		{
			initPosition = emitter->m_p_initialPosition + t3d->localPosition;

		}
		break;
		}



		//initial rotation
		if (emitter->m_p_rangedInitialRotation)
		{
			float x = Wiwa::Math::RandomRange(emitter->m_p_minInitialRotation.x, emitter->m_p_maxInitialRotation.x);
			float y = Wiwa::Math::RandomRange(emitter->m_p_minInitialRotation.y, emitter->m_p_maxInitialRotation.y);
			float z = Wiwa::Math::RandomRange(emitter->m_p_minInitialRotation.z, emitter->m_p_maxInitialRotation.z);

			initPosition = emitter->m_p_initialRotation + t3d->localRotation + glm::vec3(x, y, z);
		}
		else
		{
			initRotation = emitter->m_p_initialRotation + t3d->localRotation;
		}

		//initial scale
		if (emitter->m_p_rangedInitialScale)
		{
			float x = Wiwa::Math::RandomRange(emitter->m_p_minInitialScale.x, emitter->m_p_maxInitialScale.x);
			float y = Wiwa::Math::RandomRange(emitter->m_p_minInitialScale.y, emitter->m_p_maxInitialScale.y);
			float z = Wiwa::Math::RandomRange(emitter->m_p_minInitialScale.z, emitter->m_p_maxInitialScale.z);

			initScale = emitter->m_p_initialScale + t3d->localScale + glm::vec3(x, y, z);
		}
		else
		{
			initScale = emitter->m_p_initialScale + t3d->localScale;
		}

		particle.position = initPosition;
		particle.rotation = initRotation;
		particle.scale = initScale;


		//initial velocity
		if (emitter->m_p_rangedVelocity)
		{
			float x = Wiwa::Math::RandomRange(emitter->m_p_minVelocity.x, emitter->m_p_maxVelocity.x);
			float y = Wiwa::Math::RandomRange(emitter->m_p_minVelocity.y, emitter->m_p_maxVelocity.y);
			float z = Wiwa::Math::RandomRange(emitter->m_p_minVelocity.z, emitter->m_p_maxVelocity.z);

			particle.velocity =  glm::vec3(x, y, z);
		}
		else
		{
			particle.velocity = emitter->m_p_initialVelocity;
		}

		//initial angular velocity
		if (emitter->m_p_rangedAngularVelocity)
		{
			float x = Wiwa::Math::RandomRange(emitter->m_p_minInitialAngularVelocity.x, emitter->m_p_maxInitialAngularVelocity.x);
			float y = Wiwa::Math::RandomRange(emitter->m_p_minInitialAngularVelocity.y, emitter->m_p_maxInitialAngularVelocity.y);
			float z = Wiwa::Math::RandomRange(emitter->m_p_minInitialAngularVelocity.z, emitter->m_p_maxInitialAngularVelocity.z);

			particle.angularVelocity = glm::vec3(x, y, z);
		}
		else
		{
			particle.angularVelocity = emitter->m_p_initialAngularVelocity;
		}

		//initial growth velocity
		if (emitter->m_p_rangedGrowthVelocity)
		{
			float x = Wiwa::Math::RandomRange(emitter->m_p_minInitialGrowthVelocity.x, emitter->m_p_maxInitialGrowthVelocity.x);
			float y = Wiwa::Math::RandomRange(emitter->m_p_minInitialGrowthVelocity.y, emitter->m_p_maxInitialGrowthVelocity.y);
			float z = Wiwa::Math::RandomRange(emitter->m_p_minInitialGrowthVelocity.z, emitter->m_p_maxInitialGrowthVelocity.z);

			particle.growthVelocity = glm::vec3(x, y, z);
		}
		else
		{
			particle.growthVelocity = emitter->m_p_initialGrowthVelocity;
		}

		if (emitter->m_p_rangedLifeTime)
		{
			SetParticleLifeTime(particle, Wiwa::Math::RandomRange(emitter->m_p_minLifeTime, emitter->m_p_maxLifeTime));
		}
		else
		{
			SetParticleLifeTime(particle, emitter->m_p_lifeTime);
		}

		//particle.life_time = emitter->m_p_lifeTime;

		

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