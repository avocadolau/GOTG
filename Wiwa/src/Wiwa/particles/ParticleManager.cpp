#include <wipch.h>
#include <Wiwa/core/Renderer3D.h>
#include "Wiwa/core/Resources.h"
#include "Wiwa/ecs/components/ParticleEmitter.h"

#include <Wiwa/utilities/render/Material.h>
#include <Wiwa/utilities/render/CameraManager.h>
#include <Wiwa/utilities/render/LightManager.h>
#include <Wiwa/utilities/math/Math.h>
#include <glew.h>

namespace Wiwa {
	ParticleManager::ParticleManager()
	{

		ResourceId particleShaderID = Resources::Load<Shader>("resources/shaders/particle_display");
		Shader* shader = Resources::GetResourceById<Shader>(particleShaderID);
		shader->Compile("resources/shaders/particle_display");
		m_Material = new Material(shader);
	}

	void ParticleManager::Awake()
	{

	}

	void ParticleManager::Init()
	{

	}

	void ParticleManager::Update()
	{
		//On system added
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






			UpdateParticles();
		}

	}

	void ParticleManager::OnDestroy()
	{


	}

	void ParticleManager::UpdateParticles()
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


					//fix rotation
					glm::mat4 rotationMatrix = glm::rotate(glm::mat4(1.0f), glm::radians(-(p.transform.localRotation.y + p.transform.rotation.y)), glm::vec3(0, 1, 0));
					glm::vec4 rotatedVertex = rotationMatrix * glm::vec4(scaledVertex, 1.0f);
					glm::vec3 resultantVertex = glm::vec3(rotatedVertex);
					glm::vec3 resultantPosition = p.startingPosition + p.transform.localPosition + resultantVertex;




					if (p.followEmitterPosition)
					{
						resultantPosition = t3D->localPosition + p.startingPosition + p.transform.localPosition + resultantVertex;
					}

					p.vertices[i] = resultantPosition;

					//---------------------------------------

					// Update the animation
					p.animation->Update();

					// Get the current frame of the animation
					glm::vec4& frame = p.animation->GetCurrentFrame();

					// Update the texture coordinates based on the current frame
					/*p.texCoords[0] = frame.x;
					p.texCoords[1] = frame.y;
					p.texCoords[2] = frame.x + frame.z;
					p.texCoords[3] = frame.y + frame.w;*/

					//---------------------------------------

					if (emitter->isAnimated == false)
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

					if (emitter->isAnimated == true)
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

					bool importedCorrectly = Wiwa::Resources::CheckImport<Wiwa::Image>(emitter->texturePath.c_str());

					if (importedCorrectly)
					{
						textureResource = Wiwa::Resources::Load<Wiwa::Image>(emitter->texturePath.c_str());
						texture = Wiwa::Resources::GetResourceById<Wiwa::Image>(textureResource);
					}

					//show in game cameras
					for (size_t i = 0; i < cameraCount; i++)
					{
						CameraId cam_id = cameras[i];
						Camera* camera = man.getCamera(cam_id);

						if (texture)
							r3d.RenderQuad(VAO, indices, p.transform.position, p.transform.rotation, p.transform.localScale,
								lman.GetDirectionalLight(), lman.GetPointLights(), lman.GetSpotLights(), m_Material, false, camera, true, texture, texture->GetSize());
					}

					//show in editor window
					if (texture)
						r3d.RenderQuad(VAO, indices, p.transform.position, p.transform.rotation, p.transform.localScale,
							lman.GetDirectionalLight(), lman.GetPointLights(), lman.GetSpotLights(), m_Material, false, man.editorCamera, true, texture, texture->GetSize());


					glDeleteVertexArrays(1, &VAO);
					glDeleteBuffers(1, &VBO);
					glDeleteBuffers(1, &EBO);
				}
			}
		}



	}

	//bool ParticleManager::CreateCustomParticle()
	//{
	//	return true;
	//}
}

