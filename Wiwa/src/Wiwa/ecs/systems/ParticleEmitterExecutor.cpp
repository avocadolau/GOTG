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
		ResourceId particleShaderID = Resources::Load<Shader>("resources/shaders/particle_display");
		Shader* shader = Resources::GetResourceById<Shader>(particleShaderID);
		shader->Compile("resources/shaders/particle_display");
		m_Material = new Material(shader);
	}

	ParticleEmitterExecutor::~ParticleEmitterExecutor()
	{

	}

	void ParticleEmitterExecutor::OnAwake()
	{

	}

	void ParticleEmitterExecutor::OnInit()
	{
		WI_CORE_INFO("Init");

	}

	void ParticleEmitterExecutor::OnUpdate()
	{

		emitter = GetComponent<ParticleEmitter>();

		

		if (emitter)
		{

			dt = Time::GetRealDeltaTime() / 1000;


			if (timer <= 0)
			{
				if (emitter->repeat)
				{
					if (emitter->particle_rate_isRanged)
					{
						timer = Wiwa::Math::RandomRange(emitter->particle_rate_range[0], emitter->particle_rate_range[1]);
					}
					else
					{
						timer = emitter->particle_rate;
					}
				}

				AddParticles();
			}
			else
			{
				if (emitter->isPlaying)
				{
					timer -= dt;

				}

			}

			if (textureAnimationPoints <= 1)
			{
				if (animationTimer >= 6)
				{
					textureAnimationPoints = textureAnimationPoints + 0.25f;
					animationTimer = 0;
				}
				else
				{
					animationTimer += dt;
				}
			}
			


			
			UpdateParticles();
		}
	}

	void ParticleEmitterExecutor::OnDestroy()
	{

	}

	void ParticleEmitterExecutor::UpdateParticles()
	{
		Renderer3D& r3d = Application::Get().GetRenderer3D();
		CameraManager& man = Wiwa::SceneManager::getActiveScene()->GetCameraManager();
		EntityManager& eman = Wiwa::SceneManager::getActiveScene()->GetEntityManager();
		LightManager& lman = Wiwa::SceneManager::getActiveScene()->GetLightManager();

		ParticleEmitter* emitter = GetComponent<ParticleEmitter>();
		Transform3D* t3D = GetComponent<Transform3D>();

		std::string info_ = "active particles: " + std::to_string(currentParticleIndex);
		WI_CORE_INFO(info_.c_str());

		//----------------------------------------------------------------------------------
		/*Camera *camera = Wiwa::SceneManager::getActiveScene()->GetCameraManager().editorCamera;

		glViewport(0, 0, camera->frameBuffer->getWidth(), camera->frameBuffer->getHeight());
		camera->frameBuffer->Bind(false);

		glMatrixMode(GL_PROJECTION);
		glLoadMatrixf(glm::value_ptr(camera->getProjection()));
		glMatrixMode(GL_MODELVIEW);
		glLoadMatrixf(glm::value_ptr(camera->getView()));

		glBegin(GL_LINES);
		glColor3f(1, 1, 1);

		float length = 0.2f;
		float spacing = 0.1f;

		float start = 0;
		float z_con = 0;
		int max_elements_line_ref = 50;
		int max_elements_line = max_elements_line_ref;

		float offset = -7;

		for (size_t j = 0; j < MAX_PARTICLES; j++)
		{
			ParticleBillboard& p = particleArray[j];

			if (particleArray[j].isActive)
				glColor3f(1, 0, 0);

			if (!particleArray[j].isActive)
				glColor3f(0, 1, 0);

			if (j > max_elements_line)
			{
				z_con += spacing * 5;
				max_elements_line += max_elements_line_ref;
				start = 0;
			}

			glVertex3f(start + offset, 0.0f, z_con);
			glVertex3f(start + length + offset, 0.0f, z_con);

			start += length + spacing;


		}

		glEnd();

		camera->frameBuffer->Unbind();*/

		//----------------------------------------------------------------------------------

		for (size_t j = 0; j < currentParticleIndex; j++)
		{
			if (currentParticleIndex < MAX_PARTICLES)
			{
				ParticleBillboard& p = particleArray[j];

				//particle death
				if (p.lifetime < 0)
				{
					p.lifetime = 0;
					p.lifetime_percentage = 0;
					p.initial_lifetime = 0;
					p.color = glm::vec4(1, 1, 1, 1);

					//position
					p.transform.position = glm::vec3(0, 0, 0);
					p.transform.localPosition = glm::vec3(0, 0, 0);

					p.startingPosition = glm::vec3(0, 0, 0);
					p.velocity = glm::vec3(0, 0, 0);
					p.acceleration = glm::vec3(0, 0, 0);

					//rotation
					p.transform.rotation = glm::vec3(0, 0, 0);
					p.transform.localRotation = glm::vec3(0, 0, 0);

					p.startingRotation = glm::vec3(0, 0, 0);

					//scale
					p.transform.scale = glm::vec3(1, 1, 1);
					p.transform.localScale = glm::vec3(1, 1, 1);

					p.start_size = glm::vec3(1.0f, 1.0f, 1.0f);
					p.growthVelocity = glm::vec3(0, 0, 0);
					p.growthAcceleration = glm::vec3(0, 0, 0);

					p.followEmitterRotation = false;
					p.followEmitterPosition = false;
					p.followParticle = false;

					particleArray[j].isActive = false;

					//shift all elements after j to the left
					for (size_t k = j; k < currentParticleIndex - 1; k++)
					{
						particleArray[k] = particleArray[k + 1];
					}

					currentParticleIndex--; //decrease currentParticleIndex since one particle has been removed
					j--; //decrement j to account for the shift
					continue;
				}




				//particles look at the camera:
				//ScreenAlign(p);

				p.lifetime -= dt;
				p.lifetime_percentage = p.lifetime / p.initial_lifetime;

				//update particles mesh points

				//position
				p.velocity += p.acceleration * dt;
				glm::vec3 resultantPosition = p.velocity * dt;

				p.transform.position += resultantPosition;

				if (p.startingRotation.x != 0.0f || p.startingRotation.y != 0.0f || p.startingRotation.z != 0.0f)
				{
					p.transform.rotation = p.startingRotation;
				}

				//size
				p.transform.localScale += p.growthVelocity * dt;


				//set data to vertices
				for (size_t i = 0; i < 4; i++)
				{
					//update particle variables



					glm::vec3 rotatedPosition = p.startingPosition;

					if (p.followEmitterRotation)
					{
						float degtorad = PI / 180;

						/*rotatedPosition = glm::vec3(
							p.startingPosition.x * cosf(t3D->localRotation.y * degtorad) - p.startingPosition.z * sinf(t3D->localRotation.y * degtorad),
							0,
							p.startingPosition.z * sinf(t3D->localRotation.y * degtorad) + p.startingPosition.z * cosf(t3D->localRotation.y * degtorad));
						*/	

						rotatedPosition = glm::vec3(
							p.startingPosition.x * cosf(t3D->localRotation.y * degtorad) + p.startingPosition.z * cosf(t3D->localRotation.y * degtorad),
							0,
							p.startingPosition.z * cosf(t3D->localRotation.y * degtorad) + p.startingPosition.x * cosf(t3D->localRotation.y * degtorad));

					}

					glm::vec3 resultantPosition = p.vertices[i] = (ref_vertices[i] + rotatedPosition + p.transform.position);

					glm::vec2 ref_tex_coords[4] =
					{
						glm::vec2(0, 0),	//0		0	  2  		1	  3
						glm::vec2(0, textureAnimationPoints),	//1			     		
						glm::vec2(textureAnimationPoints, 0),	//2			     		
						glm::vec2(textureAnimationPoints, textureAnimationPoints)		//3		1	  3  		0	  2				
					};

					p.tex_coords[i] = ref_tex_coords[i];

					if (p.followEmitterPosition)
					{
						resultantPosition = (ref_vertices[i] + t3D->position + rotatedPosition + p.transform.position);
					}

					/*if (p.followEmitterRotation)
					{
						glm::vec3 test = resultantPosition;

						resultantPosition = glm::vec3(resultantPosition.x * cosf(t3D->rotation.y), 0, test.z);

						std::string rstPos = "x: " + std::to_string(resultantPosition.x) + " y: " + std::to_string(resultantPosition.z);
						WI_CORE_INFO(rstPos.c_str());
					}*/

					//emitterComp->position


					p.vertices[i] = resultantPosition;
				}

				//draw particles
				{
					GLuint VAO, VBO, EBO;

					//set mesh
					glGenVertexArrays(1, &VAO);
					glGenBuffers(1, &VBO);
					glGenBuffers(1, &EBO);

					glBindVertexArray(VAO);
					glBindBuffer(GL_ARRAY_BUFFER, VBO);

					glBufferData(GL_ARRAY_BUFFER, sizeof(glm::vec3) * sizeof(p.vertices), &p.vertices, GL_DYNAMIC_DRAW);



					glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, EBO);
					glBufferData(GL_ELEMENT_ARRAY_BUFFER, sizeof(p.vertex_indices) * sizeof(int), &p.vertex_indices, GL_DYNAMIC_DRAW);

					glEnableVertexAttribArray(0);
					glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, 0, (void*)0);

					glEnableVertexAttribArray(2);
					glVertexAttribPointer(2, 2, GL_FLOAT, GL_FALSE, 0, (void*)sizeof(glm::vec3[4]));

					glBindVertexArray(0);


					size_t cameraCount = man.getCameraSize();
					std::vector<CameraId>& cameras = man.getCameras();


					std::vector<int> indices;
					for (size_t i = 0; i < 6; i++)
					{
						indices.push_back(p.vertex_indices[i]);
					}

					//show in game cameras
					for (size_t i = 0; i < cameraCount; i++)
					{
						CameraId cam_id = cameras[i];
						Camera* camera = man.getCamera(cam_id);

						if (emitter->texture)
							r3d.RenderQuad(VAO, indices, p.transform.position, p.transform.rotation, p.transform.localScale,
								lman.GetDirectionalLight(), lman.GetPointLights(), lman.GetSpotLights(), m_Material, false, camera, true, emitter->texture, emitter->texture->GetSize());
					}

					//show in editor window
					if (emitter->texture)
						r3d.RenderQuad(VAO, indices, p.transform.position, p.transform.rotation, p.transform.localScale,
							lman.GetDirectionalLight(), lman.GetPointLights(), lman.GetSpotLights(), m_Material, false, man.editorCamera, true, emitter->texture, emitter->texture->GetSize());


					glDeleteVertexArrays(1, &VAO);
					glDeleteBuffers(1, &VBO);
					glDeleteBuffers(1, &EBO);
				}
			}
		}
	
	
		
}

	void ParticleEmitterExecutor::OnSystemAdded() // Called when system added to the editor
	{
		m_emitterComponent = GetComponentIterator<ParticleEmitter>();
		Wiwa::ParticleEmitter* emitter = GetComponentByIterator<Wiwa::ParticleEmitter>(m_emitterComponent);

		if (emitter &&!emitter->texturePath.empty())
		{
			emitter->textId1 = Wiwa::Resources::Load<Wiwa::Image>(emitter->texturePath.c_str());

			emitter->texture = Wiwa::Resources::GetResourceById<Wiwa::Image>(emitter->textId1);
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
			sdata.tex_id = emitter->texture->GetTextureId();

			//Problema de Memory Leak
			m_Material->SetUniformData("u_Texture", sdata);

			p.m_material = m_Material;

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


