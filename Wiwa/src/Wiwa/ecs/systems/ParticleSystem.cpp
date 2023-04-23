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
		emitter->m_materialChanged = false;

		//init with delay
		float delay = 0;
		m_SpawnTimer = delay;
		m_AvailableParticles = emitter->m_maxParticles;

		emitter->m_activeTimeChanged = false;

		glm::vec3 initPosition = emitter->m_p_initialPosition + t3d->position;
		glm::vec3 initRotation = emitter->m_p_initialRotation + t3d->position;
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
		ParticleEmitterComponent* emitter = GetComponent<ParticleEmitterComponent>();


		if (emitter->m_activeOverTime)
		{
			if (emitter->m_startActive)
			{
				if (emitter->m_rangedTimeActive)
				{
					emitter->m_ActiveTimer = Wiwa::Math::RandomRange(emitter->m_minInitialTimeActive, emitter->m_maxInitialTimeActive);
				}
				else
				{
					emitter->m_ActiveTimer = emitter->m_initialTimeActive;
				}
			}
			else
			{
				emitter->m_ActiveTimer = 0;
			}
			
		}
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

		if (emitter->m_materialChanged)
		{
			ResourceId matid = Wiwa::Resources::Load<Material>(emitter->m_materialPath);
			m_Material = Wiwa::Resources::GetResourceById<Material>(matid);
			emitter->m_materialChanged = false;

		}

		if (emitter->m_activeTimeChanged && emitter->m_startActive)
		{
			if (emitter->m_activeOverTime)
			{
				if (emitter->m_rangedTimeActive)
				{
					emitter->m_ActiveTimer = Wiwa::Math::RandomRange(emitter->m_minInitialTimeActive, emitter->m_maxInitialTimeActive);
				}
				else
				{
					emitter->m_ActiveTimer = emitter->m_initialTimeActive;
				}
			}
			emitter->m_activeTimeChanged = false;
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

		if (emitter->m_active || emitter->m_activeOverTime)
		{
			float dt = Time::GetDeltaTime() * 0.001;

			if (emitter->m_activeOverTime)
			{
				if (emitter->m_ActiveTimer > 0)
					m_SpawnTimer -= dt;
				emitter->m_ActiveTimer -= dt;
			}
			else
				m_SpawnTimer -= dt;


			if (m_SpawnTimer < 0 || (emitter->m_activeOverTime && emitter->m_ActiveTimer > 0))
			{
				for (unsigned int i = 0; i < emitter->m_spawnAmount; ++i)
				{
					if (m_AvailableParticles > 0)
					{
						int unusedParticle = FirstUnusedParticle();
						SpawnParticle(m_Particles[unusedParticle]);
					}
				}

				m_SpawnTimer = emitter->m_spawnRate;

				if (emitter->m_p_rangedSpawnRate)
				{
					m_SpawnTimer = Wiwa::Math::RandomRange(emitter->m_p_minSpawnRate, emitter->m_p_maxSpawnRate);
				}
			}

			//WI_CORE_INFO("spawn timer = {0}", m_SpawnTimer);




			int activeParticles = 0;
			for (unsigned int i = 0; i < m_MaxParticles; ++i)
			{
				Particle& particle = m_Particles[i];
				float dt = Time::GetDeltaTime() * 0.001f;

				if (particle.life_time > 0.0f)
				{
					UpdateParticleLife(particle, dt);

					activeParticles++;

					glm::vec4 color(0);
					if (emitter->m_colorsUsed > 1)
					{
						/*std::string perc = "particle life percentage: " + std::to_string(particle.life_percentage);
						WI_CORE_INFO(perc.c_str());*/

						for (size_t j = 0; j < emitter->m_colorsUsed - 1; j++)
						{
							if (particle.life_percentage >= emitter->m_p_colorsOverLifetime[j].m_percentage * 0.01f &&
								particle.life_percentage < emitter->m_p_colorsOverLifetime[j + 1].m_percentage * 0.01f)
							{
								/*std::string cid = std::to_string(j) + " Color id: " + std::to_string(j);
								WI_CORE_INFO(cid.c_str());*/
								
								color = particle.color = InterpolateVec4(
									emitter->m_p_colorsOverLifetime[j].color,
									emitter->m_p_colorsOverLifetime[j + 1].color,
									particle.life_percentage - emitter->m_p_colorsOverLifetime[j].m_percentage * 0.01f,
									emitter->m_p_colorsOverLifetime[j + 1].m_percentage * 0.01f - emitter->m_p_colorsOverLifetime[j].m_percentage * 0.01f
								);
							}
						}
					}
					else
					{
						color = particle.color = emitter->m_p_colorsOverLifetime[0].color;
					}
					

					Uniform* u_color = m_Material->getUniform("u_Color");
					if (u_color != nullptr)
						u_color->setData(color, UniformType::fVec4);

					Uniform* u_life = m_Material->getUniform("u_LifeTime");
					if (u_life != nullptr)
						u_life->setData(particle.life_percentage, UniformType::Float);
					
					
					//calculate everything
					Transform3D* t3d = GetComponent<Transform3D>();

					if (emitter->m_p_positionTowardsPoint)
					{
						particle.transform.localPosition = (particle.startPosition) * (1 - particle.life_percentage) + (t3d->position + emitter->m_p_positionTowardsPointPos) * particle.life_percentage;
					}
					else
					{
						
						particle.transform.localPosition += particle.velocity * dt;
						
					}

					if (emitter->m_p_useGravity)
					{
						float grav = emitter->m_p_gravity * dt;
						particle.velocity.y -= grav;
					}

					if (emitter->m_p_scaleOverTime)
					{

						//particle.scale = emitter->m_p_scaleOverTimeStart * (1 - particle.life_percentage) + emitter->m_p_scaleOverTimeEnd * particle.life_percentage * emitter->m_p_growTime;
						if (particle.life_percentage > emitter->m_p_scaleOverTimePerStart * 0.01f &&
							particle.life_percentage < emitter->m_p_scaleOverTimePerEnd * 0.01f)
						{
							particle.transform.localScale = InterpolateVec3(
								emitter->m_p_scaleOverTimeStart,
								emitter->m_p_scaleOverTimeEnd,
								particle.life_percentage - emitter->m_p_scaleOverTimePerStart * 0.01f,
								emitter->m_p_scaleOverTimePerEnd * 0.01f - emitter->m_p_scaleOverTimePerStart * 0.01f
							);
							//WI_CORE_INFO("[{0}] interpolating ", particle.life_percentage);
						}
						else if (particle.life_percentage < emitter->m_p_scaleOverTimePerStart * 0.01f)
						{
							//WI_CORE_INFO("[{0}] clamp to start scale", particle.life_percentage);
							particle.transform.localScale = emitter->m_p_scaleOverTimeStart;
						}
						else if (particle.life_percentage > emitter->m_p_scaleOverTimePerEnd * 0.01f)
						{
							//WI_CORE_INFO("[{0}] clamp to end scale", particle.life_percentage);
							particle.transform.localScale = emitter->m_p_scaleOverTimeEnd;
						}
						
					
					}
					else
					{
						particle.transform.localScale += particle.growthVelocity * dt;
					}

					if (emitter->m_p_rotationOverTime)
					{
						//particle.rotation = emitter->m_p_rotationOverTimeStart * (1 - particle.life_percentage) + emitter->m_p_rotationOverTimeEnd * particle.life_percentage * emitter->m_p_growTime;
						if (particle.life_percentage > emitter->m_p_rotationOverTimePerStart * 0.01f &&
							particle.life_percentage < emitter->m_p_rotationOverTimePerEnd * 0.01f)
						{
							particle.transform.localRotation = InterpolateVec3(
								emitter->m_p_rotationOverTimeStart,
								emitter->m_p_rotationOverTimeEnd,
								particle.life_percentage - emitter->m_p_rotationOverTimePerStart * 0.01f,
								emitter->m_p_rotationOverTimePerEnd * 0.01f - emitter->m_p_rotationOverTimePerStart * 0.01f
							);
							//WI_CORE_INFO("[{0}] interpolating ", particle.life_percentage);
						}
						else if (particle.life_percentage < emitter->m_p_rotationOverTimePerStart * 0.01f)
						{
							//WI_CORE_INFO("[{0}] clamp to start rotation", particle.life_percentage);
							particle.transform.localRotation = emitter->m_p_rotationOverTimeStart;
						}
						else if (particle.life_percentage > emitter->m_p_rotationOverTimePerEnd * 0.01f)
						{
							//WI_CORE_INFO("[{0}] clamp to end rotation", particle.life_percentage);
							particle.transform.localRotation = emitter->m_p_rotationOverTimeEnd;

						}
					}
					else 
					{
						particle.transform.localRotation += particle.angularVelocity * dt;
					}



					//update world space values
					if (emitter->m_p_followEmitterPosition)
					{
						particle.transform.position = t3d->position + particle.transform.localPosition;

					}
					else
					{
						particle.transform.position = particle.transform.localPosition;

					}

					if (emitter->m_p_followEmitterRotation)
					{
						particle.transform.rotation = t3d->rotation + particle.transform.localRotation;

					}
					else
					{
						particle.transform.rotation = particle.transform.localRotation;

					}

					particle.transform.scale = particle.transform.localScale;

					

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

						transform = glm::translate(transform, particle.transform.position);

						//transform = billboardMatrix * transform;

						transform = glm::rotate(transform, rotationRad.y, glm::vec3(0.0f, 1.0f, 0.0f));
						transform = glm::rotate(transform, particle.transform.rotation.z, glm::vec3(0.0f, 0.0f, 1.0f));
						transform = glm::rotate(transform, rotationRad.x, glm::vec3(1.0f, 0.0f, 0.0f));

						//debug
						rotationRad.x = glm::degrees(glm::asin(rotationMatrix[1][2]));
						rotationRad.y = glm::degrees(glm::atan(rotationMatrix[0][2], rotationMatrix[2][2]));
						rotationRad.z = glm::degrees(glm::atan(rotationMatrix[1][0], rotationMatrix[1][1]));
						transform = glm::scale(transform, particle.transform.scale);

						//pass transformation matrix
						particle.transform.worldMatrix = transform;
						
					}
					else
					{
						// Convert rotation angles from degrees to radians
						glm::vec3 rotationRad = glm::radians(particle.transform.rotation);

						transform = glm::translate(transform, particle.transform.position);
						transform = glm::rotate(transform, rotationRad.x, glm::vec3(1.0f, 0.0f, 0.0f));
						transform = glm::rotate(transform, rotationRad.y, glm::vec3(0.0f, 1.0f, 0.0f));
						transform = glm::rotate(transform, rotationRad.z, glm::vec3(0.0f, 0.0f, 1.0f));
						transform = glm::scale(transform, particle.transform.scale);
						//pass transformation matrix
						particle.transform.worldMatrix = transform;
					}
					

					//DEBUG PURPOSES ----------------------------------------------------------------------------

					/*WI_CORE_INFO("Local Matrix: ");

					for (size_t _y = 0; _y < 4; _y++)
					{
						std::string r;
						for (size_t _x = 0; _x < 4; _x++)
						{
							r += std::to_string(particle.transform.localMatrix[_x][_y]) + " ";
						}
						WI_CORE_INFO(r.c_str());
					}
					WI_CORE_INFO("-------------------");

					WI_CORE_INFO("World Matrix: ");

					for (size_t _y = 0; _y < 4; _y++)
					{
						std::string r;
						for (size_t _x = 0; _x < 4; _x++)
						{
							r += std::to_string(particle.transform.worldMatrix[_x][_y]) + " ";
						}
						WI_CORE_INFO(r.c_str());
					}
					WI_CORE_INFO("===================");*/

					Render(particle);
				}
			}

			emitter->m_activeParticles = activeParticles;
			m_AvailableParticles = emitter->m_maxParticles - activeParticles;

		}
		if (emitter->m_ActiveTimer < 0)  emitter->m_ActiveTimer = 0;
	}


	glm::vec4 ParticleSystem::InterpolateVec4(glm::vec4 valStart, glm::vec4 valEnd, float currentPercentage, float targetPercentage)
	{

		float process = currentPercentage / targetPercentage;
		glm::vec4 res = valStart + (valEnd - valStart) * process;

		/*std::string cp = "currentPer: " + std::to_string(currentPercentage);
		std::string tp = "targetPer: " + std::to_string(targetPercentage);
		std::string p = "process: " + std::to_string(process);
		std::string t = "Color: " + std::to_string(color.x) + " " + std::to_string(color.y) + " " + std::to_string(color.z) + " " + std::to_string(color.w);
		
		WI_CORE_INFO(cp.c_str());
		WI_CORE_INFO(tp.c_str());
		WI_CORE_INFO(p.c_str());
		WI_CORE_INFO(t.c_str());*/
		return res;
	}

	float ParticleSystem::InterpolateFloat(float valStart,float valEnd, float currentPercentage, float targetPercentage)
	{

		float process = currentPercentage / targetPercentage;
		float res = valStart + (valEnd - valStart) * process;

		return res;
	}

	glm::vec3 ParticleSystem::InterpolateVec3(glm::vec3 valStart, glm::vec3 valEnd, float currentPercentage, float targetPercentage)
	{

		float process = currentPercentage / targetPercentage;
		glm::vec3 res = valStart + (valEnd - valStart) * process;

		return res;
	}


	void ParticleSystem::OnDestroy()
	{
		
		m_Model = nullptr;
		m_Material = nullptr;

		ParticleEmitterComponent* emitter = GetComponent<ParticleEmitterComponent>();
		emitter->m_ActiveTimer = 0;
	}
	void ParticleSystem::Render(Particle& particle)
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
		glBlendEquation(GL_FUNC_ADD);
		for (size_t i = 0; i < cameraCount; i++)
		{
			CameraId cam_id = cameras[i];
			Camera* camera = man.getCamera(cam_id);

			if (camera->cull && !camera->frustrum.IsBoxVisible(m_Model->boundingBox.getMin(), m_Model->boundingBox.getMax()))
				return;
			r3d.RenderMesh(m_Model, particle.transform.worldMatrix, m_Material, lman.GetDirectionalLight(), lman.GetPointLights(), lman.GetSpotLights(), false, camera);
			
		}
		if (man.editorCamera)
		{
			r3d.RenderMesh(m_Model, particle.transform.worldMatrix, m_Material, lman.GetDirectionalLight(), lman.GetPointLights(), lman.GetSpotLights(), false, man.editorCamera);
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


		glm::vec3 followSpawnPos(0);
		glm::vec3 followSpawnRot(0);

		if (emitter->m_p_followEmitterPositionSpawn) followSpawnPos = t3d->position;
		if (emitter->m_p_followEmitterRotationSpawn) followSpawnRot = t3d->rotation;


		switch (emitter->m_spawnVolume)
		{
			case Wiwa::ParticleSpawnVolume::NONE:
			{
				initPosition = emitter->m_p_initialPosition + followSpawnPos/*+ t3d->position*/;
			}
			break;
			case Wiwa::ParticleSpawnVolume::CUBE:
			{
				float x = Wiwa::Math::RandomRange(emitter->m_p_initialPositionBoxA.x, emitter->m_p_initialPositionBoxB.x);
				float y = Wiwa::Math::RandomRange(emitter->m_p_initialPositionBoxA.y, emitter->m_p_initialPositionBoxB.y);
				float z = Wiwa::Math::RandomRange(emitter->m_p_initialPositionBoxA.z, emitter->m_p_initialPositionBoxB.z);

				initPosition = emitter->m_p_initialPosition + followSpawnPos /*+ t3d->position*/ + glm::vec3(x, y, z);
			}
			break;
			case Wiwa::ParticleSpawnVolume::SPHERE:
			{
				//Spawn in sphere

				float x = Wiwa::Math::RandomRange(1.0f,-1.0f);
				float y = Wiwa::Math::RandomRange(1.0f,-1.0f);
				float z = Wiwa::Math::RandomRange(1.0f,-1.0f);


				initPosition = emitter->m_p_initialPosition + followSpawnPos /*t3d->position*/ + emitter->m_p_initialPositionSphCenter + glm::normalize(glm::vec3(x, y, z)) * Wiwa::Math::RandomRange(0.0f, emitter->m_p_initialPositionSphRadius);

			}
			break;
			default:
			{
				initPosition = emitter->m_p_initialPosition + followSpawnPos/*+ t3d->position*/;

			}
			break;
		}
		//initial rotation
		if (emitter->m_p_rangedInitialRotation)
		{
			float x = Wiwa::Math::RandomRange(emitter->m_p_minInitialRotation.x, emitter->m_p_maxInitialRotation.x);
			float y = Wiwa::Math::RandomRange(emitter->m_p_minInitialRotation.y, emitter->m_p_maxInitialRotation.y);
			float z = Wiwa::Math::RandomRange(emitter->m_p_minInitialRotation.z, emitter->m_p_maxInitialRotation.z);

			initRotation = emitter->m_p_initialRotation + followSpawnRot /*+ t3d->localRotation*/ + glm::vec3(x, y, z);
		}
		else
		{
			initRotation = emitter->m_p_initialRotation + followSpawnRot /*+ t3d->localRotation*/;
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

		particle.startPosition = initPosition;
		particle.transform.localPosition = initPosition;
		particle.transform.localRotation= initRotation;
		particle.transform.localScale = initScale;


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