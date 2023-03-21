#pragma once

#include "System.h"
#include <vector>
#include <map>

#include <Wiwa/utilities/Reflection.h>
#include <Wiwa/utilities/render/Animation.h>

#define MAX_PARTICLES 1000
class Camera;
class AnimationParticles;
namespace Wiwa {

	struct ParticleBillboard
	{
		Material	*m_material;
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

		class AnimationParticles* animation;

		ParticleBillboard();

	};


	class WI_API ParticleEmitterExecutor : public System {
	private:

	public:
		ParticleEmitterExecutor();
		~ParticleEmitterExecutor();

		void OnAwake() override;

		void OnInit() override;

		void OnUpdate() override;

		void OnDestroy() override;

		void OnSystemAdded() override;

		void OnSystemRemoved() override;

		void ScreenAlign(std::shared_ptr<ParticleBillboard> particle);

		void EmitBatch();
		void Play();
		void Stop();
	private:

		ParticleEmitter* emitter;

		EntityManager::ComponentIterator m_emitterComponent;

		void DeleteParticleSystem();

		void AddParticles();

		void UpdateParticles();

		float timer = 0;
		float animationTimer = 0;
		float dt;

		int currentParticleIndex = 0;

		ParticleBillboard* setRotation(const glm::vec3 rot);

		std::array<ParticleBillboard, MAX_PARTICLES> particleArray;

		class Material* m_Material;

		int gameTime = 0;
		float textureAnimationPoints = 0.25f;

		glm::vec3 ref_vertices[4] = 
		{
		glm::vec3(-1, 1, 0),	//0		0	  2   		1	  3
		glm::vec3(-1, -1, 0),	//1				  		
		glm::vec3(1, 1, 0),		//2				  		
		glm::vec3(1, -1, 0)		//3		1	  3	  		0	  2
		
		};

		//glm::vec2 ref_tex_coords[4] =
		//{
		//	glm::vec2(0, 0),	//0		0	  2  		1	  3
		//	glm::vec2(0, 1),	//1			     		
		//	glm::vec2(1, 0),	//2			     		
		//	glm::vec2(1, 1)		//3		1	  3  		0	  2				
		//};

		int ref_vertex_indices[6] =
		{
			0,1,3,
			0,3,2
		};
	};
}

REGISTER_SYSTEM(Wiwa::ParticleEmitterExecutor);