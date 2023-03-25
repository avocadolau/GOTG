#pragma once

#include <Wiwa/core/Core.h>
#include <Wiwa/utilities/math/Vector3f.h>
#include "Wiwa/ecs/components/ParticleEmitter.h"
#include <Wiwa/utilities/render/TextureAnimation.h>
#include <Wiwa/ecs/EntityManager.h>

#include <vector>
#include <string>

#define MAX_PARTICLES 4000

class Camera;
class AnimationParticles;

namespace Wiwa {


	struct ParticleBillboard
	{
		Material* m_material;

		ParticleEmitter* emitterOwner;
		glm::vec3	vertices[4];
		glm::vec2	tex_coords[4];
		int			vertex_indices[6];
		Transform3D transform;

		float		initial_lifetime = 0;
		float		lifetime_percentage = 0;
		float		lifetime = 0;
		glm::vec4	color;

		//position
		glm::vec3	startingPosition;
		glm::vec3	velocity;
		glm::vec3	acceleration;

		//rotation
		glm::vec3	startingRotation;

		//size
		glm::vec3	start_size;
		glm::vec3	growthVelocity;
		glm::vec3	growthAcceleration;

		bool		followEmitterRotation;
		bool		followEmitterPosition;
		bool		followParticle;

		bool        isActive = false;

		class AnimationParticles* animation = nullptr;

		ParticleBillboard();

	};

	class WI_API ParticleManager
	{

	public:

		ParticleManager();
		~ParticleManager();

		void Awake();

		void Init();

		void Update();

		void OnDestroy();

		void ScreenAlign(std::shared_ptr<ParticleBillboard> particle);
		void AddEmitter(ParticleEmitter* emitter);
		void RemoveEmitter(ParticleEmitter* emitter);
		bool ContainsEmitter(ParticleEmitter* emitter);

		void EmitBatch(EntityId eid);
		void Play(EntityId eid, float delaySec);
		void Stop(EntityId eid);

	private:

		std::vector<std::shared_ptr<ParticleEmitter>> emitterList;
		
		Transform3D* t3D = nullptr;

		//EntityManager& m_entityManager;

		void DeleteParticleSystem();

		void AddParticles(ParticleEmitter* emitter);

		void UpdateParticles();


		
		float dt;

		int currentParticleIndex = 0;


		std::array<ParticleBillboard, MAX_PARTICLES> particleArray;

		class Material* m_Material;


		glm::vec3 ref_vertices[4] =
		{
			glm::vec3(-1, 1, 0),	//0		0	  2   		1	  3
			glm::vec3(-1, -1, 0),	//1				  		
			glm::vec3(1, 1, 0),		//2				  		
			glm::vec3(1, -1, 0)		//3		1	  3	  		0	  2

		};

		int ref_vertex_indices[6] =
		{
			0,1,3,
			0,3,2
		};
	};

}