#pragma once
#pragma warning( disable : 4251 )

#include <Wiwa/core/Core.h>

// 3D maths lib
//#include <MathGeoLib.h>
// 2D maths lib
#include <Wiwa/utilities/math/Math.h>

#include <Wiwa/utilities/Log.h>
#include <Wiwa/core/Resources.h>

//#include "utilities/gui/Texture.h"

#include <Wiwa/utilities/render/VertexBuffer.h>

//#include "Empathy/headers/utilities/renderer/Batch.h"
#include <Wiwa/utilities/render/FrameBuffer.h>
#include <Wiwa/utilities/render/Camera.h>
#include <Wiwa/utilities/render/CameraManager.h>
#include <Wiwa/scene/SceneManager.h>
#include <Wiwa/utilities/render/Model.h>
#include <Wiwa/utilities/render/Material.h>
#include <Wiwa/ecs/components/Transform3D.h>

#include <Wiwa/utilities/render/Uniforms.h>

#include <Wiwa/utilities/render/Skybox.h>

namespace Wiwa {
	class WI_API Renderer3D {
	public:
		enum Options
		{
			DEPTH_TEST = 0,
			CULL_FACE,
			LIGHTING,
			COLOR_MATERIAL,
			TEXTURE_2D,
			WIREFRAME,
			GAMMA_CORRECTION
		};
	private:
		glm::mat4 m_PersProj{ 0.0f };
		glm::mat4 m_View{ 0.0f };

		ResourceId m_BBDisplayShaderId;
		Shader* m_BBDisplayShader;
		DefaultUnlitUniforms m_BBDSUniforms;

		ResourceId m_ParticleShaderId;
		Shader* m_ParticleShader;
		DefaultUnlitUniforms m_ParticleUniforms;

		ResourceId m_NormalDisplayShaderId;
		Shader* m_NormalDisplayShader;
		DefaultUnlitUniforms m_NDSUniforms;

		ResourceId m_DepthShaderId;
		Shader* m_DepthShader;
		DefaultUnlitUniforms m_DepthShaderUniforms;

		Skybox m_DefaultSkybox;

	public:
		Renderer3D();
		~Renderer3D();

		bool Init();
		void Update();

		void SetOption(Options option);
		void DisableOption(Options option);

		void RenderMesh(Model* mesh, const Transform3D& t3d, Material* material,const size_t& directional,
			const std::vector<size_t>& pointLights, const std::vector<size_t>& spotLights, bool clear=false, Camera* camera=NULL, bool cull = false);

		
		void RenderMesh(Model* mesh, const Transform3D& t3d, const Transform3D& parent, Material* material, const size_t& directional,
			const std::vector<size_t>& pointLights, const std::vector<size_t>& spotLights, bool clear = false, Camera* camera = NULL, bool cull = false);
		void RenderMesh(Model* mesh, const glm::vec3& position, const glm::vec3 & rotation, const glm::vec3 & scale, const size_t& directional,
			const std::vector<size_t>& pointLights, const std::vector<size_t>& spotLights, Material* material, bool clear = false, Camera* camera = NULL, bool cull = false);

		void RenderMesh(Model* mesh, const glm::mat4& transform, Material* material, const size_t& directional,
			const std::vector<size_t>& pointLights, const std::vector<size_t>& spotLights, bool clear = false, Camera* camera = NULL, bool cull = false);
		
		void RenderMesh(Model* mesh, const glm::mat4& transform, Material* material, const size_t& directional,
			const std::vector<size_t>& pointLights, const std::vector<size_t>& spotLights, const std::vector<glm::mat4>& finalBoneMatrices, bool clear = false, Camera* camera = NULL, bool cull = false);
		
		void SetUpLight(Wiwa::Shader* matShader, Wiwa::Camera* camera, const size_t& directional, const std::vector<size_t>& pointLights, const std::vector<size_t>& spotLights);
		void RenderShadows(Camera* camera, const size_t& directional);

		void RenderQuad(unsigned int vao, std::vector<int> ebo_data, const glm::vec3& position, const glm::vec3& rotation, const glm::vec3& scale, const size_t& directional,
			const std::vector<size_t>& pointLights, const std::vector<size_t>& spotLights, Material* material, bool clear, Camera* camera, bool cull, Image* textureId, const Size2i& srcSize);

		void RenderSkybox();


		void Close();
		void RenderFrustrums();

		// Getters
		inline uint32_t getColorBufferTexture() { return SceneManager::getActiveScene()->GetCameraManager().getActiveCamera()->frameBuffer->getColorBufferTexture(); }

		inline FrameBuffer* getFrameBuffer() { return SceneManager::getActiveScene()->GetCameraManager().getActiveCamera()->frameBuffer; }

		glm::mat4 GetPersProjection() { return SceneManager::getActiveScene()->GetCameraManager().getActiveCamera()->getProjection(); }
		glm::mat4 GetView() { return SceneManager::getActiveScene()->GetCameraManager().getActiveCamera()->getView(); }
	};
}