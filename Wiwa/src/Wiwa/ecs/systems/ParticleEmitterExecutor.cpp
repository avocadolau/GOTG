#include <wipch.h>
#include <Wiwa/core/Renderer3D.h>
#include "ParticleEmitterExecutor.h"
#include "Wiwa/core/Resources.h"
#include "Wiwa/ecs/components/ParticleEmitter.h"
#include <Wiwa/ecs/EntityManager.h>
#include <Wiwa/utilities/render/Material.h>
#include <Wiwa/utilities/render/CameraManager.h>
#include <Wiwa/utilities/render/LightManager.h>
#include <Wiwa/utilities/math/Math.h>
#include <glew.h>

namespace Wiwa {
	ParticleEmitterExecutor::ParticleEmitterExecutor()
	{
	}

	ParticleEmitterExecutor::~ParticleEmitterExecutor()
	{

	}

	void ParticleEmitterExecutor::OnAwake()
	{
		m_emitterComponent = GetComponentIterator<Wiwa::ParticleEmitter>();
		m_emitter = GetComponentByIterator<Wiwa::ParticleEmitter>(m_emitterComponent);

		if (m_emitter && !m_emitter->texturePath.empty())
		{
			m_emitter->textureId = Wiwa::Resources::Load<Wiwa::Image>(m_emitter->texturePath.c_str());
		}
	}

	void ParticleEmitterExecutor::OnInit()
	{
		WI_CORE_INFO("Init");

	}

	void ParticleEmitterExecutor::OnUpdate()
	{
		
	}

	void ParticleEmitterExecutor::OnDestroy()
	{

	}

	
	void ParticleEmitterExecutor::OnSystemAdded() // Called when system added to the editor
	{
		EntityManager& eman = m_Scene->GetEntityManager();

		ParticleManager& pman = m_Scene->GetParticleManager();

		m_emitterComponent = GetComponentIterator<ParticleEmitter>();
		Wiwa::ParticleEmitter* emitter = GetComponentByIterator<Wiwa::ParticleEmitter>(m_emitterComponent);

		if (emitter &&!emitter->texturePath.empty())
		{
			emitter->textureId = Wiwa::Resources::Load<Wiwa::Image>(emitter->texturePath.c_str());
		}

	}

	void ParticleEmitterExecutor::AddParticles()
	{

		

		Transform3D* transform = GetComponent<Transform3D>();
		ParticleEmitter* emitter = GetComponent<ParticleEmitter>();

		int amountToAdd = emitter->particle_amount;

		if (emitter->particle_amount_isRanged)
		{
			amountToAdd = Wiwa::Math::RandomRange(emitter->particle_amount_range[0], emitter->particle_amount_range[1]);
		}

		/*currentParticleIndex = 0;*/

		for (size_t i = 0; i < amountToAdd; i++)
		{
			size_t j;

			for (j = 0; j < particleArray.size(); j++)
			{
				if (!particleArray[j].isActive)
				{
					break;
				}
			}

			if (j >= particleArray.size()) {
				// If we reach the end of the array, break out of the loop
				break;
			}

			ParticleBillboard& p = particleArray[j];

			//set particle lifetime
			if (emitter->particle_lifetime_isRanged)
			{
				p.lifetime = Wiwa::Math::RandomRange(emitter->particle_lifetime_range[0], emitter->particle_lifetime_range[1]);
			}
			else
			{
				p.lifetime = emitter->particle_lifetime;
			}

			//position
			{
				//set starting position
				if (emitter->particle_startingPosition_isRanged)
				{
					float x = Wiwa::Math::RandomRange(emitter->particle_startingPosition_range[0].x, emitter->particle_startingPosition_range[1].x);
					float y = Wiwa::Math::RandomRange(emitter->particle_startingPosition_range[0].y, emitter->particle_startingPosition_range[1].y);
					float z = Wiwa::Math::RandomRange(emitter->particle_startingPosition_range[0].z, emitter->particle_startingPosition_range[1].z);

					p.startingPosition = glm::vec3(x, y, z);
				}
				else
				{
					p.startingPosition = emitter->particle_startingPosition;
				}

				//set initial velocity
				if (emitter->particle_velocity_isRanged)
				{

					float x = Wiwa::Math::RandomRange(emitter->particle_velocity_range[0].x, emitter->particle_velocity_range[1].x);
					float y = Wiwa::Math::RandomRange(emitter->particle_velocity_range[0].y, emitter->particle_velocity_range[1].y);
					float z = Wiwa::Math::RandomRange(emitter->particle_velocity_range[0].z, emitter->particle_velocity_range[1].z);

					p.velocity = glm::vec3(x, y, z);
				}
				else
				{
					p.velocity = emitter->particle_velocity;
				}

				//set initial acceleration
				if (emitter->particle_acceleration_isRanged)
				{

					float x = Wiwa::Math::RandomRange(emitter->particle_acceleration_range[0].x, emitter->particle_acceleration_range[1].x);
					float y = Wiwa::Math::RandomRange(emitter->particle_acceleration_range[0].y, emitter->particle_acceleration_range[1].y);
					float z = Wiwa::Math::RandomRange(emitter->particle_acceleration_range[0].z, emitter->particle_acceleration_range[1].z);

					p.acceleration = glm::vec3(x, y, z);
				}
				else
				{
					p.acceleration = emitter->particle_acceleration;
				}
			}
			
			//rotation
			{
				//set starting rotation
				if (emitter->particle_startingRotation_isRanged)
				{
					float x = Wiwa::Math::RandomRange(emitter->particle_startingRotation_range[0].x, emitter->particle_startingRotation_range[1].x);
					float y = Wiwa::Math::RandomRange(emitter->particle_startingRotation_range[0].y, emitter->particle_startingRotation_range[1].y);
					float z = Wiwa::Math::RandomRange(emitter->particle_startingRotation_range[0].z, emitter->particle_startingRotation_range[1].z);

					p.startingRotation = glm::vec3(x, y, z);
				}
				else
				{
					p.startingRotation.x = emitter->particle_startingRotation.x;
					p.startingRotation.y = emitter->particle_startingRotation.y;
					p.startingRotation.z = emitter->particle_startingRotation.z;
				}
			}

			//scale
			{
				glm::vec3 startingSize(1.0f, 1.0f, 1.0f);

				if (emitter->particle_startingSize.x != 0.0f || emitter->particle_startingSize.y != 0.0f || emitter->particle_startingSize.z != 0.0f)
				{
					p.start_size = emitter->particle_startingSize;
				}

				else
				{
					emitter->particle_startingSize = startingSize;
				}

				//set initial size growth rate
				if (emitter->particle_growthVelocity_isRanged)
				{
					float x = Wiwa::Math::RandomRange(emitter->particle_growthVelocity_range[0].x, emitter->particle_growthVelocity_range[1].x);
					float y = Wiwa::Math::RandomRange(emitter->particle_growthVelocity_range[0].y, emitter->particle_growthVelocity_range[1].y);
					float z = Wiwa::Math::RandomRange(emitter->particle_growthVelocity_range[0].z, emitter->particle_growthVelocity_range[1].z);

					p.growthVelocity = glm::vec3(x, y, z);
				}
				else
				{
					p.growthVelocity = emitter->particle_growthVelocity;
				}

				//set initial size
				if (emitter->particle_growthVelocity_isRanged)
				{
					float x = Wiwa::Math::RandomRange(emitter->particle_startingSize_range[0].x, emitter->particle_startingSize_range[1].x);
					float y = Wiwa::Math::RandomRange(emitter->particle_startingSize_range[0].y, emitter->particle_startingSize_range[1].y);
					float z = Wiwa::Math::RandomRange(emitter->particle_startingSize_range[0].z, emitter->particle_startingSize_range[1].z);

					p.transform.localScale = glm::vec3(x, y, z);
				}
				else
				{
					p.transform.localScale = emitter->particle_startingSize;
				}
			}

			p.followEmitterPosition = emitter->particle_followEmitterPosition;
			p.followEmitterRotation = emitter->particle_followEmitterRotation;
			p.followParticle = emitter->particle_followParticle;


			//set geometry
			for (size_t i = 0; i < 4; i++)
			{
				p.vertices[i] = ref_vertices[i];
				//p.tex_coords[i] = ref_tex_coords[i];
			}
			for (size_t i = 0; i < 6; i++)
			{
				p.vertex_indices[i] = ref_vertex_indices[i];
			}

			p.transform.scale = glm::vec3(1, 1, 1);

			Uniform::SamplerData sdata;



			//get texture from resourceId
			Image* texture = nullptr;

			Wiwa::ResourceId textureResource = -4;

			bool importedCorrectly = Wiwa::Resources::CheckImport<Wiwa::Image>(emitter->texturePath.c_str());

			if (importedCorrectly)
			{
				textureResource = Wiwa::Resources::Load<Wiwa::Image>(emitter->texturePath.c_str());
				texture = Wiwa::Resources::GetResourceById<Wiwa::Image>(textureResource);
			}


			sdata.tex_id = texture->GetTextureId();

			//Problema de Memory Leak
			m_Material->SetUniformData("u_Texture", sdata);

			p.m_material = m_Material;

			p.animation = new AnimationParticles();

			if (p.animation != nullptr)
			{
				p.animation->speed = 1.0f;
				p.animation->loop = true;
				p.animation->PushBack(glm::vec4(0.0f, 0.0f, 0.25f, 0.25f)); // Add the frames of the animation here
				p.animation->PushBack(glm::vec4(0.25f, 0.0f, 0.25f, 0.25f));
				p.animation->PushBack(glm::vec4(0.5f, 0.0f, 0.25f, 0.25f));
			}

			/*delete p.animation;*/

			particleArray[currentParticleIndex] = p;

			particleArray[currentParticleIndex].isActive = true;

			currentParticleIndex++;
		}

	}

	void ParticleEmitterExecutor::ScreenAlign(std::shared_ptr<ParticleBillboard> particle)
	{
		Wiwa::CameraManager& cm = Wiwa::SceneManager::getActiveScene()->GetCameraManager();
		CameraId cameraId;
		cameraId = cm.getActiveCameraId();
		Wiwa::Camera* cam = cm.getCamera(cameraId);
		glm::vec3 offset = glm::vec3(-90.0f, -60.0f, 0.0f); //Adjust the desired offset

		particle->transform.rotation.x = cam->getRotation().x + offset.x;
		particle->transform.rotation.y = cam->getRotation().y + offset.y;
		particle->transform.rotation.z = cam->getRotation().z + offset.z;
	}

	void ParticleEmitterExecutor::OnSystemRemoved() // Called when system removed to the editor
	{

	}

	void ParticleEmitterExecutor::EmitBatch()
	{
		AddParticles();
	}

	void ParticleEmitterExecutor::Play()
	{
		if (emitter)
		{
			emitter->isPlaying = true;

			if (emitter->particle_rate_isRanged)
			{
				timer = Wiwa::Math::RandomRange(emitter->particle_rate_range[0], emitter->particle_rate_range[1]);
			}
			else
			{
				timer = emitter->particle_rate;
			}
		}
	}

	void ParticleEmitterExecutor::Stop()
	{
		if (emitter)
		{
			emitter->isPlaying = false;
		}
	}

	void ParticleEmitterExecutor::DeleteParticleSystem()
	{
		
	}

	ParticleBillboard::ParticleBillboard()
	{

		

		//set variables
		initial_lifetime = 0;
		lifetime_percentage = 0;
		lifetime = 0;
		color = glm::vec4(1, 1, 1, 1);
		
		//position
		transform.position = glm::vec3(0, 0, 0);
		transform.localPosition = glm::vec3(0, 0, 0);

		startingPosition = glm::vec3(0, 0, 0);
		velocity = glm::vec3(0, 0, 0);
		acceleration = glm::vec3(0, 0, 0);


		//rotation
		transform.rotation = glm::vec3(0, 0, 0);
		transform.localRotation = glm::vec3(0, 0, 0);

		startingRotation = glm::vec3(0, 0, 0);


		//scale
		transform.scale = glm::vec3(1, 1, 1);
		transform.localScale = glm::vec3(1, 1, 1);

		start_size = glm::vec3(1.0f, 1.0f, 1.0f);
		growthVelocity = glm::vec3(0, 0, 0);
		growthAcceleration = glm::vec3(0, 0, 0);


		followEmitterRotation = false;
		followEmitterPosition = false;
		followParticle = false;
	}
}


