#include "wipch.h"
#include "ParticleEmmiter.h"


namespace Wiwa {
	ParticleEmmiter::ParticleEmmiter()
	{
	}
	ParticleEmmiter::~ParticleEmmiter()
	{
	}
	void ParticleEmmiter::Awake()
	{
	}
	void ParticleEmmiter::Init()
	{
		// init particle struct
		m_Particles.reserve(m_MaxParticles);
		for (int i = 0; i < m_MaxParticles; i++)
		{
			m_Particles[i] = Particle();
		}
		m_VAO = GetVAO(m_ParticleMesh);
	}
	void ParticleEmmiter::Update()
	{
	}
	void ParticleEmmiter::CleanUp()
	{
	}
	void ParticleEmmiter::Render()
	{
	}
	unsigned int ParticleEmmiter::GetVAO(ParticleMeshType type)
	{
		unsigned int vao = -1;
		switch (m_ParticleMesh)
		{
		case ParticleMeshType::QUAD:
		{
			ResourceId id = Wiwa::Resources::Load<Model>("../library/models/basic_shapes/quad.fbx");

			Model* quad = Wiwa::Resources::GetResourceById<Model>(id);

			vao = quad->getVAO();
		}
			break;

		case ParticleMeshType::CUBE:
		{	
			ResourceId id = Wiwa::Resources::Load<Model>("../library/models/basic_shapes/cube.fbx");

			Model* quad = Wiwa::Resources::GetResourceById<Model>(id);

			vao = quad->getVAO(); 
		}
		break; 

		case ParticleMeshType::SPHERE:
		{
			ResourceId id = Wiwa::Resources::Load<Model>("../library/models/basic_shapes/sphere.fbx");

			Model* quad = Wiwa::Resources::GetResourceById<Model>(id);

			vao = quad->getVAO();
		}
		break; 

		case ParticleMeshType::CONE:
		{
			ResourceId id = Wiwa::Resources::Load<Model>("../library/models/basic_shapes/cone.fbx");

			Model* quad = Wiwa::Resources::GetResourceById<Model>(id);

			vao = quad->getVAO();
		}			
		break; 
		case ParticleMeshType::CYLYNDER:
		{
			ResourceId id = Wiwa::Resources::Load<Model>("../library/models/basic_shapes/cylinder.fbx");

			Model* quad = Wiwa::Resources::GetResourceById<Model>(id);

			vao = quad->getVAO();
		}
			break;
		default:
			vao = -1;
			break;
		}

		return vao;
	}
}