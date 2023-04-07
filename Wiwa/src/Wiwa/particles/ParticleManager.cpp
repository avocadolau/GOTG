#include <wipch.h>
#include <Wiwa/core/Renderer3D.h>
#include "Wiwa/core/Resources.h"
#include "Wiwa/ecs/components/ParticleEmitter.h"
#include <Wiwa/core/Application.h>
#include <Wiwa/utilities/render/Material.h>
#include <Wiwa/utilities/render/CameraManager.h>
#include <Wiwa/utilities/render/LightManager.h>
#include <Wiwa/utilities/math/Math.h>
#include <glew.h>

namespace Wiwa {
	ParticleManager::ParticleManager()// : m_entityManager(Wiwa::SceneManager::getActiveScene()->GetEntityManager())
	{

		ResourceId particleShaderID = Resources::Load<Shader>("resources/shaders/particle_display");
		Shader* shader = Resources::GetResourceById<Shader>(particleShaderID);
		shader->Compile("resources/shaders/particle_display");
		m_Material = new Material(shader);

		currentFrameParticles = 0;
	}


	ParticleManager::~ParticleManager()
	{
		delete m_Material;
	}

	void ParticleManager::Awake()
	{

	}

	void ParticleManager::Init()
	{

	}

	void ParticleManager::Update()
	{
		/*currentFrameParticles++;*/ //check
		if (emitterList.size() > 0)
		{
			dt = Time::GetRealDeltaTime() / 1000;


			for (std::shared_ptr<ParticleEmitter> emitter : emitterList)
			{

				if (emitter->isPlaying)
				{
					if (emitter->timer < 0)
					{

						AddParticles(emitter.get());

						if (emitter->repeat)
						{
							if (emitter->particle_rate_isRanged)
							{
								emitter->timer = Wiwa::Math::RandomRange(emitter->particle_rate_range_0, emitter->particle_rate_range_1);
							}
							else
							{
								emitter->timer = emitter->particle_rate;

							}
						}



					}


					emitter->timer -= dt;

				}

			}
		}

		UpdateParticles();
	}

	void ParticleManager::OnDestroy()
	{


	}

	void ParticleManager::UpdateParticles()
	{
		Wiwa::Renderer3D& r3d = Application::Get().GetRenderer3D();
		CameraManager& man = Wiwa::SceneManager::getActiveScene()->GetCameraManager();
		EntityManager& eman = Wiwa::SceneManager::getActiveScene()->GetEntityManager();
		LightManager& lman = Wiwa::SceneManager::getActiveScene()->GetLightManager();

		for (size_t j = 0; j < currentParticleIndex; j++)
		{


			if (currentParticleIndex < MAX_PARTICLES)
			{

				ParticleBillboard& p = particleArray[j];

				//Transform3D* t3D = m_entityManager.GetComponent<Transform3D>(p.emitterOwner->entityOwnerId);
				Transform3D* t3D = eman.GetComponent<Transform3D>(p.emitterOwner->entityOwnerId);

				if (t3D == nullptr)
				{
					p.lifetime = -1;
				}


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

					p.followSpawn = false;

					currentFrameParticles = 0;

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

				p.transform.localPosition += resultantPosition;

				if (p.startingRotation.x != 0.0f || p.startingRotation.y != 0.0f || p.startingRotation.z != 0.0f)
				{
					p.transform.localRotation = p.startingRotation;
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
						p.transform.rotation = t3D->rotation;
					}

					glm::vec3 scaledVertex = ref_vertices[i] * p.transform.localScale;


					glm::vec3 resultantVertex = glm::vec3(0, 0, 0);

					if (p.emitterOwner->activateBillboard == true)
					{

						Wiwa::CameraManager& cm = Wiwa::SceneManager::getActiveScene()->GetCameraManager();
						CameraId cameraId;
						cameraId = cm.getActiveCameraId();
						Wiwa::Camera* cam = cm.getCamera(cameraId);

						//fix rotation
						glm::mat4 rotationMatrix = cam->getView();
						glm::mat4 billboardMatrix = glm::inverse(rotationMatrix);


						glm::vec4 rotatedVertex = billboardMatrix * glm::vec4(scaledVertex, 1.0f);
						resultantVertex = glm::vec3(rotatedVertex);
					}
					else
					{
						// Rotate around all three axes
						float xRotation = p.transform.localRotation.x + p.transform.rotation.x;
						float yRotation = p.transform.localRotation.y + p.transform.rotation.y;
						float zRotation = p.transform.localRotation.z + p.transform.rotation.z;
						glm::mat4 rotationMatrix = glm::rotate(glm::mat4(1.0f), glm::radians(-yRotation), glm::vec3(0, 1, 0))
							* glm::rotate(glm::mat4(1.0f), glm::radians(-xRotation), glm::vec3(1, 0, 0))
							* glm::rotate(glm::mat4(1.0f), glm::radians(-zRotation), glm::vec3(0, 0, 1));
						glm::vec4 rotatedVertex = rotationMatrix * glm::vec4(scaledVertex, 1.0f);
						resultantVertex = glm::vec3(rotatedVertex);
					}

					if (p.followSpawn)
					{
						p.startingPosition = t3D->position;
					}

					glm::vec3 resultantPosition = p.startingPosition + p.transform.localPosition + resultantVertex + p.offset;

					p.followSpawn = false;


					if (p.followEmitterPosition)
					{
						resultantPosition = t3D->localPosition + p.startingPosition + p.transform.localPosition + resultantVertex + p.offset;
					}

					p.vertices[i] = resultantPosition;

					//---------------------------------------

					if (p.m_material)
					{
						// calculate elapsed time since particle's start time
						double elapsedTime = Wiwa::Time::GetTime() - p.startTime;

						/*int animationFrame = currentFrameParticles - p.animationCreationTime;*/

						// Update the animation
						p.animation->Update(elapsedTime, p.emitterOwner->particle_animation_speed);

						// Get the current frame of the animation
						glm::vec4& frame = p.animation->GetCurrentFrame();

						// Update the texture coordinates based on the current frame
						/*p.texCoords[0] = frame.x;
						p.texCoords[1] = frame.y;
						p.texCoords[2] = frame.x + frame.z;
						p.texCoords[3] = frame.y + frame.w;*/

						//---------------------------------------

						if (p.emitterOwner->isAnimated == false)
						{
							glm::vec2 ref_tex_coords[4] =
							{
								glm::vec2(0, 0),	//0		0	  2  		1	  3
								glm::vec2(0, 1),	//1			     		
								glm::vec2(1, 0),	//2			     		
								glm::vec2(1, 1)		//3		1	  3  		0	  2				
							};

							p.tex_coords[i] = ref_tex_coords[i];
						}

						if (p.emitterOwner->isAnimated == true)
						{
							glm::vec2 ref_tex_coords[4] =
							{
								glm::vec2(frame.x, frame.y),                     //0    0    2    1    3
								glm::vec2(frame.x, frame.y + frame.w),           //1
								glm::vec2(frame.x + frame.z, frame.y),           //2
								glm::vec2(frame.x + frame.z, frame.y + frame.w)  //3    1    3    0    2	
							};

							p.tex_coords[i] = ref_tex_coords[i];
						}
					}
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

					//get texture from resourceId
					Image* texture = nullptr;

					Wiwa::ResourceId textureResource = -4;

					bool importedCorrectly = Wiwa::Resources::CheckImport<Wiwa::Image>(p.emitterOwner->texturePath);

					if (importedCorrectly)
					{
						textureResource = Wiwa::Resources::Load<Wiwa::Image>(p.emitterOwner->texturePath);
						texture = Wiwa::Resources::GetResourceById<Wiwa::Image>(textureResource);
					}

					//show in game cameras
					for (size_t i = 0; i < cameraCount; i++)
					{
						CameraId cam_id = cameras[i];
						Camera* camera = man.getCamera(cam_id);

						if (texture)
							r3d.RenderQuad(VAO, indices, p.transform.position - (camera->getPosition() * (float)p.emitterOwner->activateBillboard), p.transform.rotation, p.transform.localScale,
								lman.GetDirectionalLight(), lman.GetPointLights(), lman.GetSpotLights(), m_Material, false, camera, true, texture, texture->GetSize(), p.emitterOwner->colorParticles, p.emitterOwner->particle_colour_range);
					}

					Wiwa::CameraManager& cm = Wiwa::SceneManager::getActiveScene()->GetCameraManager();
					CameraId cameraId;
					cameraId = cm.getActiveCameraId();
					Wiwa::Camera* cam = cm.getCamera(cameraId);

					//show in editor window
					if (texture)
						r3d.RenderQuad(VAO, indices, p.transform.position - (cam->getPosition() * (float)p.emitterOwner->activateBillboard), p.transform.rotation, p.transform.localScale,
							lman.GetDirectionalLight(), lman.GetPointLights(), lman.GetSpotLights(), m_Material, false, man.editorCamera, true, texture, texture->GetSize(), p.emitterOwner->colorParticles, p.emitterOwner->particle_colour_range);

					glDeleteVertexArrays(1, &VAO);
					glDeleteBuffers(1, &VBO);
					glDeleteBuffers(1, &EBO);
				}
			}
		}
	}

	void ParticleManager::ScreenAlign(std::shared_ptr<ParticleBillboard> particle)
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

	int ParticleManager::CalculateNumberOfAnimations(char texturePath[128])
	{
		// Load the image data using stb_image
		int width, height, channels;
		unsigned char* image_data = stbi_load(texturePath, &width, &height, &channels, 0);

		// Check if there is data for each square of the animation
		int square_size = width / 4;
		int num_animations = 0;

		for (int row = 0; row < 4; row++)
		{
			for (int col = 0; col < 4; col++)
			{
				int x = col * square_size;
				int y = row * square_size;
				bool has_data = false;

				for (int i = 0; i < square_size; i++)
				{
					for (int j = 0; j < square_size; j++)
					{
						int index = ((y + i) * width + (x + j)) * channels;

						if (image_data[index + 3] > 0)
						{
							has_data = true;
							break;
						}
					}

					if (has_data)
					{
						break;
					}
				}

				if (has_data)
				{
					num_animations++;
				}
			}
		}

		// Free the image data
		stbi_image_free(image_data);

		return num_animations;
	}

	void ParticleManager::AddEmitter(ParticleEmitter* emitter)
	{
		std::shared_ptr<ParticleEmitter> sm_emitter = std::make_shared<ParticleEmitter>(*emitter);
		emitterList.push_back(sm_emitter);
	}

	void ParticleManager::RemoveEmitter(ParticleEmitter* emitter)
	{
		if (emitterList.size() > 0)
		{
			if (emitter->entityOwnerId)
			{
				for (int i = emitterList.size() - 1; i >= 0; i--)
				{
					if (emitterList[i]->entityOwnerId == emitter->entityOwnerId)
					{
						emitterList.erase(emitterList.begin() + i);
					}
				}
			}
		}

		/*if (emitterList.size() > 0)
		{
			if (emitter->entityOwnerId)
			{
				emitterList.erase(std::remove_if(emitterList.begin(), emitterList.end(),
					[&](std::shared_ptr<ParticleEmitter> const& item)
					{
						return item->entityOwnerId == emitter->entityOwnerId;
					}), emitterList.end());
			}
		}*/

		//emitterList.erase(
		//	std::remove_if(emitterList.begin(), emitterList.end(),[&]()));
	}

	bool ParticleManager::ContainsEmitter(ParticleEmitter* emitter)
	{
		if (emitter == nullptr)
		{
			return false;
		}

		for (std::shared_ptr<ParticleEmitter> item : emitterList)
		{
			if (item->entityOwnerId == emitter->entityOwnerId)
				return true;
		}
		return false;
	}
	void ParticleManager::AddParticles(ParticleEmitter* emitter)
	{

		EntityManager& eman = Wiwa::SceneManager::getActiveScene()->GetEntityManager();

		Transform3D* t3D = eman.GetComponent<Transform3D>(emitter->entityOwnerId);

		if (t3D == nullptr)
			return;

		int amountToAdd = emitter->particle_amount;

		if (emitter->particle_amount_isRanged)
		{
			amountToAdd = Wiwa::Math::RandomRange(emitter->particle_amount_range_0, emitter->particle_amount_range_1);
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

			//set particle emitter owner
			p.emitterOwner = emitter;

			//set particle lifetime
			if (emitter->particle_lifetime_isRanged)
			{
				p.lifetime = Wiwa::Math::RandomRange(emitter->particle_lifetime_range_0, emitter->particle_lifetime_range_1);
			}
			else
			{
				p.lifetime = emitter->particle_lifetime;
			}

			//position
			{
				if (p.followSpawn == false)
				{
					//set starting position
					if (emitter->particle_startingPosition_isRanged)
					{
						float x = Wiwa::Math::RandomRange(emitter->particle_startingPosition_range_0.x, emitter->particle_startingPosition_range_1.x);
						float y = Wiwa::Math::RandomRange(emitter->particle_startingPosition_range_0.y, emitter->particle_startingPosition_range_1.y);
						float z = Wiwa::Math::RandomRange(emitter->particle_startingPosition_range_0.z, emitter->particle_startingPosition_range_1.z);

						p.startingPosition = glm::vec3(x, y, z);
					}
					else
					{
						p.startingPosition = emitter->particle_startingPosition;
					}
				}

				//set initial velocity
				if (emitter->particle_velocity_isRanged)
				{

					float x = Wiwa::Math::RandomRange(emitter->particle_velocity_range_0.x, emitter->particle_velocity_range_1.x);
					float y = Wiwa::Math::RandomRange(emitter->particle_velocity_range_0.y, emitter->particle_velocity_range_1.y);
					float z = Wiwa::Math::RandomRange(emitter->particle_velocity_range_0.z, emitter->particle_velocity_range_1.z);

					p.velocity = glm::vec3(x, y, z);
				}
				else
				{
					p.velocity = emitter->particle_velocity;
				}

				//set initial acceleration
				if (emitter->particle_acceleration_isRanged)
				{

					float x = Wiwa::Math::RandomRange(emitter->particle_acceleration_range_0.x, emitter->particle_acceleration_range_1.x);
					float y = Wiwa::Math::RandomRange(emitter->particle_acceleration_range_0.y, emitter->particle_acceleration_range_1.y);
					float z = Wiwa::Math::RandomRange(emitter->particle_acceleration_range_0.z, emitter->particle_acceleration_range_1.z);

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
					float x = Wiwa::Math::RandomRange(emitter->particle_startingRotation_range_0.x, emitter->particle_startingRotation_range_1.x);
					float y = Wiwa::Math::RandomRange(emitter->particle_startingRotation_range_0.y, emitter->particle_startingRotation_range_1.y);
					float z = Wiwa::Math::RandomRange(emitter->particle_startingRotation_range_0.z, emitter->particle_startingRotation_range_1.z);

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
					float x = Wiwa::Math::RandomRange(emitter->particle_growthVelocity_range_0.x, emitter->particle_growthVelocity_range_1.x);
					float y = Wiwa::Math::RandomRange(emitter->particle_growthVelocity_range_0.y, emitter->particle_growthVelocity_range_1.y);
					float z = Wiwa::Math::RandomRange(emitter->particle_growthVelocity_range_0.z, emitter->particle_growthVelocity_range_1.z);

					p.growthVelocity = glm::vec3(x, y, z);
				}
				else
				{
					p.growthVelocity = emitter->particle_growthVelocity;
				}

				//set initial size
				if (emitter->particle_growthVelocity_isRanged)
				{
					float x = Wiwa::Math::RandomRange(emitter->particle_startingSize_range_0.x, emitter->particle_startingSize_range_1.x);
					float y = Wiwa::Math::RandomRange(emitter->particle_startingSize_range_0.y, emitter->particle_startingSize_range_1.y);
					float z = Wiwa::Math::RandomRange(emitter->particle_startingSize_range_0.z, emitter->particle_startingSize_range_1.z);

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

			p.followSpawn = emitter->followOnSpawn;
			p.offset = emitter->particle_Offset;

			if (!p.emitterOwner->particle_Offset_isRanged)
			{
				p.offset = glm::vec3(0.0f, 0.0f, 0.0f);
			}


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

			//get texture from resourceId
			Image* texture = nullptr;

			Wiwa::ResourceId textureResource = -4;

			bool importedCorrectly = Wiwa::Resources::CheckImport<Wiwa::Image>(emitter->texturePath);

			if (importedCorrectly)
			{
				textureResource = Wiwa::Resources::Load<Wiwa::Image>(emitter->texturePath);
				texture = Wiwa::Resources::GetResourceById<Wiwa::Image>(textureResource);
			}

			if (texture)
			{
				Uniform::SamplerData sdata;
				sdata.tex_id = texture->GetTextureId();

				//Problema de Memory Leak
				m_Material->SetUniformData("u_Texture", sdata);

				p.m_material = m_Material;

				AnimationParticles& particleAnimation = AnimationParticles::getInstance();

				p.animation = &particleAnimation;

				p.animationCreationTime = currentFrameParticles;

				p.animation->creationTimes.push_back(p.animationCreationTime);

				//Calculates the total number of animations
				int numberOfAnimations = CalculateNumberOfAnimations(emitter->texturePath);

				if (p.animation != nullptr)
				{
					/*p.animation->speedParticlesAnimation = p.emitterOwner->particle_animation_speed;*/
					p.animation->loop = false;

					float frameX = 0.0f;
					float frameY = 0.0f;

					for (int i = 0; i < numberOfAnimations; i++)
					{

						p.animation->PushBack(glm::vec4(frameX, frameY, 0.25f, 0.25f));

						frameX = frameX + 0.25f;

						if (frameX > 0.75)
						{
							frameY = frameY + 0.25;
							frameX = 0.0f;
						}
					}

					//p.animation->PushBack(glm::vec4(0.0f, 0.0f, 0.25f, 0.25f)); // Add the frames of the animation here
					//p.animation->PushBack(glm::vec4(0.25f, 0.0f, 0.25f, 0.25f));
					//p.animation->PushBack(glm::vec4(0.5f, 0.0f, 0.25f, 0.25f));
					//p.animation->PushBack(glm::vec4(0.75f, 0.0f, 0.25f, 0.25f));
					//p.animation->PushBack(glm::vec4(0.0f, 0.25f, 0.25f, 0.25f));
					//p.animation->PushBack(glm::vec4(0.25f, 0.25f, 0.25f, 0.25f));
				}
			}

			/*delete p.animation;*/

			p.startTime = Wiwa::Time::GetTime();

			particleArray[currentParticleIndex] = p;

			particleArray[currentParticleIndex].isActive = true;

			currentParticleIndex++;
		}

		currentFrameParticles++;

	}

	void ParticleManager::EmitBatch(EntityId eid)
	{
		EntityManager& eman = Wiwa::SceneManager::getActiveScene()->GetEntityManager();

		ParticleEmitter* emitter = eman.GetComponent<ParticleEmitter>(eid);

		AddParticles(emitter);
	}

	void ParticleManager::Play(EntityId eid, float delaySec)
	{
		EntityManager& eman = Wiwa::SceneManager::getActiveScene()->GetEntityManager();
		ParticleEmitter* emitter = eman.GetComponent<ParticleEmitter>(eid);

		if (emitter && !emitter->isPlaying)
		{
			emitter->isPlaying = true;
			WI_CORE_INFO("time = {}", delaySec);
			emitter->timer = delaySec;

		}
	}

	void ParticleManager::Stop(EntityId eid)
	{
		EntityManager& eman = Wiwa::SceneManager::getActiveScene()->GetEntityManager();
		ParticleEmitter* emitter = eman.GetComponent<ParticleEmitter>(eid);

		if (emitter && emitter->isPlaying)
		{
			emitter->isPlaying = false;

		}
	}

	void ParticleManager::DeleteParticleSystem()
	{

	}

	ParticleBillboard::ParticleBillboard()
	{



		//set variables
		this->emitterOwner = nullptr;
		initial_lifetime = 0;
		lifetime_percentage = 0;
		lifetime = 0;
		color = glm::vec4(1, 1, 1, 1);

		//position
		transform.position = glm::vec3(0, 0, 0);
		transform.localPosition = glm::vec3(0, 0, 0);

		offset = glm::vec3(0, 0, 0);
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
		followSpawn = false;
	}
	//bool ParticleManager::CreateCustomParticle()
	//{
	//	return true;
	//}
}