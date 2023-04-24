#include <wipch.h>
#include "Renderer3D.h"

#include <Wiwa/core/Application.h>
#include <Wiwa/utilities/Log.h>
#include <Wiwa/utilities/render/CameraManager.h>
#include <Wiwa/scene/SceneManager.h>
#include <glew.h>
#include <Wiwa/utilities/render/shaders/Shader.h>
#include <Wiwa/utilities/render/animator.h>

#include <Wiwa/ecs/systems/LightSystem.h>

namespace Wiwa
{
	Renderer3D::Renderer3D()
	{
	}

	Renderer3D::~Renderer3D()
	{
	}

	bool Renderer3D::Init()
	{
		Size2i &resolution = Application::Get().GetTargetResolution();

		WI_CORE_INFO("Renderer3D initialized");
		SetOption(Options::DEPTH_TEST);
		SetOption(Options::CULL_FACE);

		// Init default shaders with uniforms
		ResourceId textShaderId = Wiwa::Resources::Load<Shader>("resources/shaders/light/toon_textured");
		Shader *textShader = Wiwa::Resources::GetResourceById<Shader>(textShaderId);
		textShader->Compile("resources/shaders/light/toon_textured");
		textShader->addUniform("u_Texture", UniformType::Sampler2D);
		textShader->addUniform("u_ToonLevels", UniformType::Int);
		textShader->addUniform("u_RimLightPower", UniformType::Float);
		textShader->addUniform("u_SpecularValue", UniformType::Float);
		textShader->addUniform("u_MatAmbientColor", UniformType::fVec4);
		textShader->addUniform("u_MatDiffuseColor", UniformType::fVec4);
		textShader->addUniform("u_MatSpecularColor", UniformType::fVec4);
		Wiwa::Resources::Import<Shader>("resources/shaders/light/toon_textured", textShader);

		// Init skinned
		ResourceId skinnedShaderId = Wiwa::Resources::Load<Shader>("resources/shaders/skinned/skinned");
		Shader *skinnedShader = Wiwa::Resources::GetResourceById<Shader>(skinnedShaderId);

		skinnedShader->Compile("resources/shaders/skinned/skinned");
		Wiwa::Resources::Import<Shader>("resources/shaders/skinned/skinned", skinnedShader);
		// Init skinned textured
		ResourceId skinnedTexturedShaderId = Wiwa::Resources::Load<Shader>("resources/shaders/skinned/skinned_textured");
		Shader* skinnedTexturedShader = Wiwa::Resources::GetResourceById<Shader>(skinnedTexturedShaderId);
		skinnedTexturedShader->Compile("resources/shaders/skinned/skinned_textured");
		skinnedTexturedShader->addUniform("u_Texture", UniformType::Sampler2D);
		skinnedTexturedShader->addUniform("u_ToonLevels", UniformType::Int);
		skinnedTexturedShader->addUniform("u_RimLightPower", UniformType::Float);
		skinnedTexturedShader->addUniform("u_SpecularValue", UniformType::Float);
		skinnedTexturedShader->addUniform("u_MatAmbientColor", UniformType::fVec4);
		skinnedTexturedShader->addUniform("u_MatDiffuseColor", UniformType::fVec4);
		skinnedTexturedShader->addUniform("u_MatSpecularColor", UniformType::fVec4);
		Wiwa::Resources::Import<Shader>("resources/shaders/skinned/skinned_textured", skinnedTexturedShader);
		// Init skinned textured outlined
		ResourceId skinnedTexturedOutlinedShaderId = Wiwa::Resources::Load<Shader>("resources/shaders/skinned/skinned_textured_outlined");
		Shader* skinnedTexturedOutlinedShader = Wiwa::Resources::GetResourceById<Shader>(skinnedTexturedOutlinedShaderId);
		skinnedTexturedOutlinedShader->Compile("resources/shaders/skinned/skinned_textured_outlined");
		skinnedTexturedOutlinedShader->addUniform("u_Texture", UniformType::Sampler2D);
		skinnedTexturedOutlinedShader->addUniform("u_OutlineColor", UniformType::fVec4);
		skinnedTexturedOutlinedShader->addUniform("u_OutlineSmoothRange", UniformType::fVec2);
		skinnedTexturedOutlinedShader->addUniform("u_ToonLevels", UniformType::Int);
		skinnedTexturedOutlinedShader->addUniform("u_RimLightPower", UniformType::Float);
		skinnedTexturedOutlinedShader->addUniform("u_SpecularValue", UniformType::Float);
		skinnedTexturedOutlinedShader->addUniform("u_MatAmbientColor", UniformType::fVec4);
		skinnedTexturedOutlinedShader->addUniform("u_MatDiffuseColor", UniformType::fVec4);
		skinnedTexturedOutlinedShader->addUniform("u_MatSpecularColor", UniformType::fVec4);
		Wiwa::Resources::Import<Shader>("resources/shaders/skinned/skinned_textured_outlined", skinnedTexturedOutlinedShader);
		//init skinned Outline
		ResourceId skinnedOutlineShaderId = Wiwa::Resources::Load<Shader>("resources/shaders/skinned/skinned_outline");
		Shader* skinnedOutlineShader = Wiwa::Resources::GetResourceById<Shader>(skinnedOutlineShaderId);
		skinnedOutlineShader->Compile("resources/shaders/skinned/skinned_outline");
		skinnedOutlineShader->addUniform("u_Texture", UniformType::Sampler2D);
		skinnedOutlineShader->addUniform("u_OutlineColor", UniformType::fVec4);
		skinnedOutlineShader->addUniform("u_OutlineSmoothRange", UniformType::fVec2);
		Wiwa::Resources::Import<Shader>("resources/shaders/skinned/skinned_outline", skinnedOutlineShader);
		// Init skinned depth
		ResourceId skinnedDepthShaderId = Wiwa::Resources::Load<Shader>("resources/shaders/skinned/skinned_depth");
		Shader* skinnedDepthShader = Wiwa::Resources::GetResourceById<Shader>(skinnedDepthShaderId);
		skinnedDepthShader->Compile("resources/shaders/skinned/skinned_depth");
		skinnedDepthShader->addUniform("u_Texture", UniformType::Sampler2D);
		Wiwa::Resources::Import<Shader>("resources/shaders/skinned/skinned_depth", skinnedDepthShader);
		// Init bone debug
		ResourceId debugBonesShaderId = Wiwa::Resources::Load<Shader>("resources/shaders/skinned/debug_bones");
		Shader *debugBoneShader = Wiwa::Resources::GetResourceById<Shader>(debugBonesShaderId);
		debugBoneShader->Compile("resources/shaders/skinned/debug_bones");
		debugBoneShader->addUniform("u_DisplayBoneIndex", UniformType::Int);
		Wiwa::Resources::Import<Shader>("resources/shaders/skinned/debug_bones", debugBoneShader);

		ResourceId colorShaderId = Wiwa::Resources::Load<Shader>("resources/shaders/light/toon_color");
		Shader *colorShader = Wiwa::Resources::GetResourceById<Shader>(colorShaderId);
		colorShader->Compile("resources/shaders/light/toon_color");

		colorShader->addUniform("u_Color", UniformType::fVec4);
		colorShader->addUniform("u_ToonLevels", UniformType::Int);
		colorShader->addUniform("u_RimLightPower", UniformType::Float);
		colorShader->addUniform("u_SpecularValue", UniformType::Float);
		colorShader->addUniform("u_MatAmbientColor", UniformType::fVec4);
		colorShader->addUniform("u_MatDiffuseColor", UniformType::fVec4);
		colorShader->addUniform("u_MatSpecularColor", UniformType::fVec4);
		Wiwa::Resources::Import<Shader>("resources/shaders/light/toon_color", colorShader);
		//toon textured outlined
		ResourceId outlinedShaderId = Wiwa::Resources::Load<Shader>("resources/shaders/light/toon_textured_outlined");
		Shader* outlinedShader = Wiwa::Resources::GetResourceById<Shader>(outlinedShaderId);
		outlinedShader->Compile("resources/shaders/light/toon_textured_outlined");
		outlinedShader->addUniform("u_Texture", UniformType::Sampler2D);
		outlinedShader->addUniform("u_OutlineColor", UniformType::fVec4);
		outlinedShader->addUniform("u_OutlineSmoothRange", UniformType::fVec2);
		outlinedShader->addUniform("u_ToonLevels", UniformType::Int);
		outlinedShader->addUniform("u_RimLightPower", UniformType::Float);
		outlinedShader->addUniform("u_SpecularValue", UniformType::Float);
		outlinedShader->addUniform("u_MatAmbientColor", UniformType::fVec4);
		outlinedShader->addUniform("u_MatDiffuseColor", UniformType::fVec4);
		outlinedShader->addUniform("u_MatSpecularColor", UniformType::fVec4);
		Wiwa::Resources::Import<Shader>("resources/shaders/light/toon_textured_outlined", outlinedShader);


		ResourceId basecolorShaderId = Wiwa::Resources::Load<Shader>("resources/shaders/light/base_color");
		Shader* basecolorShader = Wiwa::Resources::GetResourceById<Shader>(basecolorShaderId);
		basecolorShader->Compile("resources/shaders/light/base_color");

		basecolorShader->addUniform("u_Color", UniformType::fVec4);
		
		Wiwa::Resources::Import<Shader>("resources/shaders/light/base_color", basecolorShader);

		//===========================================================================================================
		// VFX shaders
		//===========================================================================================================

		ResourceId dissolveShaderId = Wiwa::Resources::Load<Shader>("resources/shaders/vfx/dissolve");
		Shader* dissolveShader = Wiwa::Resources::GetResourceById<Shader>(dissolveShaderId);
		dissolveShader->Compile("resources/shaders/vfx/dissolve");

		dissolveShader->addUniform("u_LifeTime", UniformType::Float);
		dissolveShader->addUniform("u_Color", UniformType::fVec4);
		dissolveShader->addUniform("u_Texture", UniformType::Sampler2D);
		dissolveShader->addUniform("u_DissolveAmount", UniformType::Float);
		dissolveShader->addUniform("u_DiscardTex", UniformType::Sampler2D);

		Wiwa::Resources::Import<Shader>("resources/shaders/vfx/dissolve", dissolveShader);
		
		ResourceId smokeShaderId = Wiwa::Resources::Load<Shader>("resources/shaders/vfx/smoke");

		Shader* smokeShader = Wiwa::Resources::GetResourceById<Shader>(smokeShaderId);
		smokeShader->Compile("resources/shaders/vfx/smoke");

		smokeShader->addUniform("u_LifeTime", UniformType::Float);
		smokeShader->addUniform("u_Color", UniformType::fVec4);
		smokeShader->addUniform("u_Texture", UniformType::Sampler2D);
		smokeShader->addUniform("u_DissolveAmount", UniformType::Float);
		smokeShader->addUniform("u_EnterDissolve", UniformType::Float);
		smokeShader->addUniform("u_OutDissolve", UniformType::Float);
		smokeShader->addUniform("u_DiscardTex", UniformType::Sampler2D);

		Wiwa::Resources::Import<Shader>("resources/shaders/vfx/smoke", smokeShader);

		ResourceId shieldShaderId = Wiwa::Resources::Load<Shader>("resources/shaders/vfx/shield");
		Shader* shieldShader = Wiwa::Resources::GetResourceById<Shader>(shieldShaderId);
		shieldShader->Compile("resources/shaders/vfx/shield");

		shieldShader->addUniform("u_LifeTime", UniformType::Float);
		shieldShader->addUniform("u_Color", UniformType::fVec4);
		shieldShader->addUniform("u_FresnelColor", UniformType::fVec4);
		shieldShader->addUniform("u_FresnelRange", UniformType::fVec2);
		shieldShader->addUniform("u_Texture", UniformType::Sampler2D);
		shieldShader->addUniform("u_DiscardTex", UniformType::Sampler2D);
		shieldShader->addUniform("u_HologramTexture", UniformType::Sampler2D);
		shieldShader->addUniform("u_HologramColor", UniformType::fVec4);
		shieldShader->addUniform("u_Amplitude", UniformType::Float);
		shieldShader->addUniform("u_Frequency", UniformType::Float);
		shieldShader->addUniform("u_StartDissolve", UniformType::Float);

		Wiwa::Resources::Import<Shader>("resources/shaders/vfx/shield", shieldShader);

		ResourceId justTextureShaderId = Wiwa::Resources::Load<Shader>("resources/shaders/vfx/justtexture");
		Shader* justTextureShader = Wiwa::Resources::GetResourceById<Shader>(justTextureShaderId);
		justTextureShader->Compile("resources/shaders/vfx/justtexture");

		justTextureShader->addUniform("u_LifeTime", UniformType::Float);
		justTextureShader->addUniform("u_Color", UniformType::fVec4);
		justTextureShader->addUniform("u_Texture", UniformType::Sampler2D);

		Wiwa::Resources::Import<Shader>("resources/shaders/vfx/justtexture", justTextureShader);

		ResourceId impactShaderId = Wiwa::Resources::Load<Shader>("resources/shaders/vfx/shot_impact");
		Shader* impactShader = Wiwa::Resources::GetResourceById<Shader>(impactShaderId);
		impactShader->Compile("resources/shaders/vfx/shot_impact");

		impactShader->addUniform("u_LifeTime", UniformType::Float);
		impactShader->addUniform("u_Color", UniformType::fVec4);
		impactShader->addUniform("u_Texture", UniformType::Sampler2D);

		Wiwa::Resources::Import<Shader>("resources/shaders/vfx/barrel_explosion/explosion", impactShader);

		ResourceId explosionShaderId = Wiwa::Resources::Load<Shader>("resources/shaders/vfx/barrel_explosion/explosion");
		Shader* explosionShader = Wiwa::Resources::GetResourceById<Shader>(explosionShaderId);
		explosionShader->Compile("resources/shaders/vfx/barrel_explosion/explosion");

		explosionShader->addUniform("u_LifeTime", UniformType::Float);
		explosionShader->addUniform("u_Color", UniformType::fVec4);
		explosionShader->addUniform("u_FresnelColor", UniformType::fVec4);
		explosionShader->addUniform("u_FresnelRange", UniformType::fVec2);
		explosionShader->addUniform("u_Texture", UniformType::Sampler2D);
		explosionShader->addUniform("u_DiscardTex", UniformType::Sampler2D);
		explosionShader->addUniform("u_HologramTexture", UniformType::Sampler2D);
		explosionShader->addUniform("u_HologramColor", UniformType::fVec4);
		explosionShader->addUniform("u_Amplitude", UniformType::Float);
		explosionShader->addUniform("u_Frequency", UniformType::Float);
		explosionShader->addUniform("u_StartDissolve", UniformType::Float);

		Wiwa::Resources::Import<Shader>("resources/shaders/vfx/barrel_explosion/explosion", impactShader);

		ResourceId bulletShaderId = Wiwa::Resources::Load<Shader>("resources/shaders/vfx/bullet");
		Shader* bulletShader = Wiwa::Resources::GetResourceById<Shader>(bulletShaderId);
		bulletShader->Compile("resources/shaders/vfx/bullet");

		bulletShader->addUniform("u_LifeTime", UniformType::Float);
		bulletShader->addUniform("u_Color", UniformType::fVec4);
		bulletShader->addUniform("u_Texture", UniformType::Sampler2D);
		bulletShader->addUniform("u_TransparencyTexture", UniformType::Sampler2D);

		Wiwa::Resources::Import<Shader>("resources/shaders/vfx/bullet", impactShader);

		//===========================================================================================================

		// Normal Display Shader
		m_NormalDisplayShaderId = Resources::Load<Shader>("resources/shaders/debug/normal_display");
		m_NormalDisplayShader = Resources::GetResourceById<Shader>(m_NormalDisplayShaderId);
		m_NormalDisplayShader->Compile("resources/shaders/debug/normal_display");

		m_NDSUniforms.Model = m_NormalDisplayShader->getUniformLocation("u_Model");
		m_NDSUniforms.View = m_NormalDisplayShader->getUniformLocation("u_View");
		m_NDSUniforms.Projection = m_NormalDisplayShader->getUniformLocation("u_Projection");
		// Bounding box
		m_BBDisplayShaderId = Resources::Load<Shader>("resources/shaders/debug/boundingbox_display");
		m_BBDisplayShader = Resources::GetResourceById<Shader>(m_BBDisplayShaderId);
		m_BBDisplayShader->Compile("resources/shaders/debug/boundingbox_display");
		m_BBDSUniforms.Model = m_BBDisplayShader->getUniformLocation("u_Model");
		m_BBDSUniforms.View = m_BBDisplayShader->getUniformLocation("u_View");
		m_BBDSUniforms.Projection = m_BBDisplayShader->getUniformLocation("u_Proj");

		// Particle Shader ==> Temporal hasta materiales
		m_ParticleShaderId = Resources::Load<Shader>("resources/shaders/particle_display");
		m_ParticleShader = Resources::GetResourceById<Shader>(m_ParticleShaderId);
		m_ParticleShader->Compile("resources/shaders/particle_display");
		m_ParticleShader->addUniform("u_Texture", Wiwa::UniformType::Sampler2D);
		m_ParticleShader->addUniform("u_ColorP", Wiwa::UniformType::fVec4);
		m_ParticleUniforms.Model = m_ParticleShader->getUniformLocation("u_Model");
		m_ParticleUniforms.View = m_ParticleShader->getUniformLocation("u_View");
		m_ParticleUniforms.Projection = m_ParticleShader->getUniformLocation("u_Proj");

		m_DepthShaderId = Resources::Load<Shader>("resources/shaders/light/depth");
		m_DepthShader = Resources::GetResourceById<Shader>(m_DepthShaderId);
		m_DepthShader->Compile("resources/shaders/light/depth");
		m_DepthShaderUniforms.Model = m_DepthShader->getUniformLocation("u_Model");
		m_DepthShaderUniforms.View = m_DepthShader->getUniformLocation("u_View");
		m_DepthShaderUniforms.Projection = m_DepthShader->getUniformLocation("u_Proj");
		
		m_HDRShaderId = Resources::Load<Shader>("resources/shaders/renderlayer/hdr");
		m_HDRShader = Resources::GetResourceById<Shader>(m_HDRShaderId);
		m_HDRShader->Compile("resources/shaders/renderlayer/hdr");
		m_HDRShader->addUniform("u_HdrBufferTexture", Wiwa::UniformType::Sampler2D);
		m_HDRUniforms.Model = m_HDRShader->getUniformLocation("u_Model");
		m_HDRUniforms.View = m_HDRShader->getUniformLocation("u_View");
		m_HDRUniforms.Projection = m_HDRShader->getUniformLocation("u_Proj");

		m_BlurShaderId = Resources::Load<Shader>("resources/shaders/renderlayer/blur");
		m_BlurShader = Resources::GetResourceById<Shader>(m_BlurShaderId);
		m_BlurShader->Compile("resources/shaders/renderlayer/blur");
		m_BlurUniforms.Model = m_BlurShader->getUniformLocation("u_Model");
		m_BlurUniforms.View = m_BlurShader->getUniformLocation("u_View");
		m_BlurUniforms.Projection = m_BlurShader->getUniformLocation("u_Proj");

		m_BloomShaderId = Resources::Load<Shader>("resources/shaders/renderlayer/bloom");
		m_BloomShader = Resources::GetResourceById<Shader>(m_BloomShaderId);
		m_BloomShader->Compile("resources/shaders/renderlayer/bloom");
		m_BloomUniforms.Model = m_BlurShader->getUniformLocation("u_Model");
		m_BloomUniforms.View = m_BlurShader->getUniformLocation("u_View");
		m_BloomUniforms.Projection = m_BlurShader->getUniformLocation("u_Proj");


		std::vector<const char *> faces = {
			"resources/images/skybox/right.jpg",
			"resources/images/skybox/left.jpg",
			"resources/images/skybox/top.jpg",
			"resources/images/skybox/bottom.jpg",
			"resources/images/skybox/front.jpg",
			"resources/images/skybox/back.jpg"};

		std::vector<const char *> skybox1_faces = {
		"resources/images/skybox/right.jpg",
		"resources/images/skybox/left.jpg",
		"resources/images/skybox/top.jpg",
		"resources/images/skybox/bottom.jpg",
		"resources/images/skybox/front.jpg",
		"resources/images/skybox/back.jpg"};
		std::vector<const char*> dooms_day = {
		"resources/images/skybox/dooms_day/right.png",
		"resources/images/skybox/dooms_day/left.png",
		"resources/images/skybox/dooms_day/top.png",
		"resources/images/skybox/dooms_day/bottom.png",
		"resources/images/skybox/dooms_day/front.png",
		"resources/images/skybox/dooms_day/back.png" };
		std::vector<const char*> urban_light = {
		"resources/images/skybox/urban_light/right.png",
		"resources/images/skybox/urban_light/left.png",
		"resources/images/skybox/urban_light/top.png",
		"resources/images/skybox/urban_light/bottom.png",
		"resources/images/skybox/urban_light/front.png",
		"resources/images/skybox/urban_light/back.png" };
		std::vector<const char*> main_menu = {
		"resources/images/skybox/main_menu/UI_RightSkyBox_01.png",
		"resources/images/skybox/main_menu/UI_LeftSkyBox_01.png",
		"resources/images/skybox/main_menu/UI_LeftSkyBox_01.png",
		"resources/images/skybox/main_menu/UI_BottomSkyBox_01.png",
		"resources/images/skybox/main_menu/UI_FrontSkyBox_01.png",
		"resources/images/skybox/main_menu/UI_BackSkyBox_01.png" };
		m_DefaultSkybox.LoadCubemap(dooms_day);



		// Init orthographic projection
		m_OrthoProj = glm::ortho(0.0f, (float)1920, (float)920, 0.0f, 0.1f, 100.0f);
		// Init main camera view
		m_View = glm::mat4(1.0f);
		m_View = glm::translate(m_View, glm::vec3(0.0f, 0.0f, -3.0f));
		// Init model
		m_Model = glm::mat4(1.0f);
		m_Model = glm::translate(m_Model, glm::vec3((float)1920 / 2.0f, (float)920 / 2.0f, 0.0f));
		m_Model = glm::scale(m_Model, glm::vec3((float)1920, (float)920, 1.0f));


		return true;
	}

	void Renderer3D::PreUpdate()
	{
		RenderSkybox();
	}

	void Renderer3D::Update()
	{
		OPTICK_EVENT("Renderer 3D Update");

		
	}

	void Renderer3D::PostUpdate()
	{
		//once everything is rendered and passed by the hdr 

		/*Camera* cam = SceneManager::getActiveScene()->GetCameraManager().getActiveCamera();

		if (cam == nullptr)
			return;*/
		//// Bind VAO
		//RenderManager::BindVAO();
		//glDisable(GL_DEPTH_TEST);
		//bool horizontal = true, first_iteration = false;
		//unsigned int amount = 10;
		//m_BlurShader->Bind();

		//// blur bright fragments with two-pass Gaussian Blur 
		//for (unsigned int i = 0; i < amount; i++)
		//{
		//	m_BlurShader->setUniformInt(m_BlurShader->getUniformLocation("u_Horizontal"), (int)horizontal);
		//	if (first_iteration) {
		//		glBindTexture(GL_TEXTURE_2D, cam->frameBuffer->getColorBuffers()[1]);
		//		first_iteration = false;
		//	}
		//	else {
		//		if (horizontal) {
		//			cam->vBlurBuffer->Bind(false);
		//			cam->vBlurBuffer->BindTexture();
		//		}
		//		else {
		//			cam->vBlurBuffer->Bind(false);
		//			cam->hBlurBuffer->BindTexture();
		//		}
		//	}

		//	glDrawElements(GL_TRIANGLES, 6, GL_UNSIGNED_INT, 0);
		//	horizontal = !horizontal;
		//}
		//m_BlurShader->UnBind();
		//glEnable(GL_DEPTH_TEST);

		////3. now render floating point color buffer to 2D quad and tonemap HDR colors to default framebuffer's (clamped) color range
		////--------------------------------------------------------------------------------------------------------------------------
		//m_BloomShader->Bind();

		//cam->frameBuffer->Bind(false);

		//glActiveTexture(GL_TEXTURE0);
		//glBindTexture(GL_TEXTURE_2D, cam->frameBuffer->getColorBuffers()[0]);
		//glActiveTexture(GL_TEXTURE1);
		//glBindTexture(GL_TEXTURE_2D, cam->vBlurBuffer->getColorTexture());


		//m_BloomShader->setUniformInt(m_BloomShader->getUniformLocation("u_Bloom"), (int)true);
		//m_BloomShader->setUniformFloat(m_BloomShader->getUniformLocation("u_exposure"), 5);

		//glDrawElements(GL_TRIANGLES, 6, GL_UNSIGNED_INT, 0);

		//m_BloomShader->UnBind();

		//glBindVertexArray(0);
		//glBindFramebuffer(GL_FRAMEBUFFER, 0);


		/*	TESTING overwrite text 0 with text 1 color
			Bind the framebuffer object*/
			//glBindFramebuffer(GL_READ_FRAMEBUFFER, cam->frameBuffer->getFBO());

			//// Set the source and destination textures
			//glActiveTexture(GL_TEXTURE0);
			//glBindTexture(GL_TEXTURE_2D, cam->vBlurBuffer->getColorTexture());
			//glActiveTexture(GL_TEXTURE1);
			//glBindTexture(GL_TEXTURE_2D, cam->frameBuffer->getColorBuffers()[0]);

			//// Copy the content of texture 1 to texture 0
			//glCopyImageSubData(
			//	cam->vBlurBuffer->getColorTexture(), GL_TEXTURE_2D, 0, 0, 0, 0,
			//	cam->frameBuffer->getColorBuffers()[0], GL_TEXTURE_2D, 0, 0, 0, 0,
			//	cam->frameBuffer->getWidth(), cam->frameBuffer->getHeight(), 1
			//);

			//// Unbind the textures and framebuffer object
			//glBindTexture(GL_TEXTURE_2D, 0);
			//glBindFramebuffer(GL_READ_FRAMEBUFFER, 0);
			//glBindVertexArray(0);


			//glBindFramebuffer(GL_FRAMEBUFFER, 0);
	}

	void Renderer3D::RenderMesh(Model *mesh, const Transform3D &t3d, Material *material, const size_t &directional,
								const std::vector<size_t> &pointLights, const std::vector<size_t> &spotLights, bool clear /*=false*/, Camera *camera /*=NULL*/, bool cull /*= false*/)
	{
		if (!camera)
		{
			camera = SceneManager::getActiveScene()->GetCameraManager().getActiveCamera();
		}

		glViewport(0, 0, camera->frameBuffer->getWidth(), camera->frameBuffer->getHeight());

		camera->frameBuffer->Bind(clear);

		glm::mat4 model = glm::mat4(1.0f);

		model = glm::translate(model, glm::vec3(t3d.position.x, t3d.position.y, t3d.position.z));
		model = glm::rotate(model, glm::radians(t3d.rotation.x), glm::vec3(1, 0, 0));
		model = glm::rotate(model, glm::radians(t3d.rotation.y), glm::vec3(0, 1, 0));
		model = glm::rotate(model, glm::radians(t3d.rotation.z), glm::vec3(0, 0, 1));
		model = glm::scale(model, glm::vec3(t3d.scale.x, t3d.scale.y, t3d.scale.z));

		Shader *matShader = material->getShader();
		matShader->Bind();
		matShader->SetMVP(model, camera->getView(), camera->getProjection());

		SetUpLight(matShader, camera, directional, pointLights, spotLights);

		material->Bind();

		mesh->Render();

		material->UnBind();

		if (mesh->showNormals)
		{
			m_NormalDisplayShader->Bind();
			m_NormalDisplayShader->setUniform(m_NDSUniforms.Model, model);
			m_NormalDisplayShader->setUniform(m_NDSUniforms.View, camera->getView());
			m_NormalDisplayShader->setUniform(m_NDSUniforms.Projection, camera->getProjection());

			mesh->Render();
			m_NormalDisplayShader->UnBind();
		}
		if (camera->drawBoundingBoxes)
		{
			m_BBDisplayShader->Bind();
			m_BBDisplayShader->setUniform(m_BBDSUniforms.Model, model);
			m_BBDisplayShader->setUniform(m_BBDSUniforms.View, camera->getView());
			m_BBDisplayShader->setUniform(m_BBDSUniforms.Projection, camera->getProjection());
			mesh->DrawBoudingBox();
			m_BBDisplayShader->UnBind();
		}

		camera->frameBuffer->Unbind();
	}

	void Renderer3D::RenderMesh(Model *mesh, const Transform3D &t3d, const Transform3D &parent, Material *material, const size_t &directional,
								const std::vector<size_t> &pointLights, const std::vector<size_t> &spotLights, bool clear, Camera *camera, bool cull)
	{
		if (!camera)
		{
			camera = SceneManager::getActiveScene()->GetCameraManager().getActiveCamera();
		}

		glViewport(0, 0, camera->frameBuffer->getWidth(), camera->frameBuffer->getHeight());

		camera->frameBuffer->Bind(clear);

		glm::mat4 model = glm::mat4(1.0f);

		// Local transformation
		model = glm::translate(model, glm::vec3(t3d.localPosition.x, t3d.localPosition.y, t3d.localPosition.z));
		model = glm::rotate(model, glm::radians(t3d.localRotation.x), glm::vec3(1, 0, 0));
		model = glm::rotate(model, glm::radians(t3d.localRotation.y), glm::vec3(0, 1, 0));
		model = glm::rotate(model, glm::radians(t3d.localRotation.z), glm::vec3(0, 0, 1));
		model = glm::scale(model, glm::vec3(t3d.localScale.x, t3d.localScale.y, t3d.localScale.z));

		// Parent transformation
		glm::mat4 parent_model = glm::mat4(1.0f);
		parent_model = glm::translate(parent_model, glm::vec3(parent.position.x, parent.position.y, parent.position.z));
		parent_model = glm::rotate(parent_model, glm::radians(parent.rotation.x), glm::vec3(1, 0, 0));
		parent_model = glm::rotate(parent_model, glm::radians(parent.rotation.y), glm::vec3(0, 1, 0));
		parent_model = glm::rotate(parent_model, glm::radians(parent.rotation.z), glm::vec3(0, 0, 1));
		parent_model = glm::scale(parent_model, glm::vec3(parent.scale.x, parent.scale.y, parent.scale.z));

		model = parent_model * model;

		// Material bind
		Shader *matShader = material->getShader();
		matShader->Bind();
		matShader->SetMVP(model, camera->getView(), camera->getProjection());
		SetUpLight(matShader, camera, directional, pointLights, spotLights);

		material->Bind();

		mesh->Render();

		material->UnBind();

		if (mesh->showNormals)
		{
			m_NormalDisplayShader->Bind();
			m_NormalDisplayShader->setUniform(m_NDSUniforms.Model, model);
			m_NormalDisplayShader->setUniform(m_NDSUniforms.View, camera->getView());
			m_NormalDisplayShader->setUniform(m_NDSUniforms.Projection, camera->getProjection());

			mesh->Render();
			m_NormalDisplayShader->UnBind();
		}
		if (camera->drawBoundingBoxes)
		{
			m_BBDisplayShader->Bind();
			m_BBDisplayShader->setUniform(m_BBDSUniforms.Model, model);
			m_BBDisplayShader->setUniform(m_BBDSUniforms.View, camera->getView());
			m_BBDisplayShader->setUniform(m_BBDSUniforms.Projection, camera->getProjection());
			mesh->DrawBoudingBox();
			m_BBDisplayShader->UnBind();
		}

		camera->frameBuffer->Unbind();
	}
	void Renderer3D::RenderMesh(Model *mesh, const glm::vec3 &position, const glm::vec3 &rotation, const glm::vec3 &scale, const size_t &directional,
								const std::vector<size_t> &pointLights, const std::vector<size_t> &spotLights, Material *material, bool clear, Camera *camera, bool cull)
	{
		if (!camera)
		{
			camera = SceneManager::getActiveScene()->GetCameraManager().getActiveCamera();
		}

		glViewport(0, 0, camera->frameBuffer->getWidth(), camera->frameBuffer->getHeight());

		camera->frameBuffer->Bind(clear);

		glm::mat4 model = glm::mat4(1.0f);
		model = glm::translate(model, position);
		model = glm::rotate(model, glm::radians(rotation.x), glm::vec3(1, 0, 0));
		model = glm::rotate(model, glm::radians(rotation.y), glm::vec3(0, 1, 0));
		model = glm::rotate(model, glm::radians(rotation.z), glm::vec3(0, 0, 1));
		model = glm::scale(model, scale);
		material->getShader()->Bind();
		material->getShader()->SetMVP(model, camera->getView(), camera->getProjection());

		Shader *matShader = material->getShader();
		matShader->Bind();
		matShader->SetMVP(model, camera->getView(), camera->getProjection());
		SetUpLight(matShader, camera, directional, pointLights, spotLights);

		mesh->Render();

		material->UnBind();

		if (mesh->showNormals)
		{
			m_NormalDisplayShader->Bind();
			m_NormalDisplayShader->setUniform(m_NDSUniforms.Model, model);
			m_NormalDisplayShader->setUniform(m_NDSUniforms.View, camera->getView());
			m_NormalDisplayShader->setUniform(m_NDSUniforms.Projection, camera->getProjection());

			mesh->Render();
			m_NormalDisplayShader->UnBind();
		}
		if (camera->drawBoundingBoxes)
		{
			m_BBDisplayShader->Bind();
			m_BBDisplayShader->setUniform(m_BBDSUniforms.Model, model);
			m_BBDisplayShader->setUniform(m_BBDSUniforms.View, camera->getView());
			m_BBDisplayShader->setUniform(m_BBDSUniforms.Projection, camera->getProjection());
			mesh->DrawBoudingBox();
			m_BBDisplayShader->UnBind();
		}

		camera->frameBuffer->Unbind();
	}
	void Renderer3D::RenderMesh(Model *mesh, const glm::mat4 &transform, Material *material, const size_t &directional,
								const std::vector<size_t> &pointLights, const std::vector<size_t> &spotLights, bool clear, Camera *camera, bool cull)
	{
		if (!camera)
		{
			camera = SceneManager::getActiveScene()->GetCameraManager().getActiveCamera();
		}
		////// Setting the shadow map buffer
		//camera->shadowBuffer->Bind(clear);

		//Wiwa::Transform3D *lightTrans = Wiwa::SceneManager::getActiveScene()->GetEntityManager().GetComponent<Wiwa::Transform3D>(directional);
		//glm::mat4 view;
		//glm::mat4 projection;

		//if (lightTrans)
		//{
		//	view = glm::lookAt(
		//		lightTrans->localPosition,
		//		lightTrans->localPosition + lightTrans->localRotation,
		//		glm::vec3(0.0f, 1.0f, 0.0f));
		//	projection = glm::ortho(-10.0f, 10.0f, -10.0f, 10.0f, 0.1f, 1000.0f);

		//	m_DepthShader->Bind();
		//	m_DepthShader->setUniform(m_DepthShaderUniforms.Projection, projection);
		//	m_DepthShader->setUniform(m_DepthShaderUniforms.View, view);

		//	m_DepthShader->setUniform(m_DepthShaderUniforms.Model, lightTrans->localMatrix);
		//}

		//mesh->Render();

		//m_DepthShader->UnBind();

		//camera->shadowBuffer->Unbind();

		// Set up color buffer


	/*	if (lightTrans)
		{
			glm::mat4 lightMVP = lightTrans->localMatrix * projection * view;

			matShader->setUniform(matShader->getUniformLocation("u_LightMVP"), lightMVP);
		}*/

		//camera->shadowBuffer->BindTexture();


		glViewport(0, 0, camera->frameBuffer->getWidth(), camera->frameBuffer->getHeight());

		camera->frameBuffer->Bind(clear);

		Shader *matShader = material->getShader();

		matShader->Bind();

		matShader->SetMVP(transform, camera->getView(), camera->getProjection());

		SetUpLight(matShader, camera, directional, pointLights, spotLights);

		material->Bind();

		mesh->Render();

		material->UnBind();

		// Debug

		if (mesh->showNormals)
		{
			m_NormalDisplayShader->Bind();
			m_NormalDisplayShader->setUniform(m_NDSUniforms.Model, transform);
			m_NormalDisplayShader->setUniform(m_NDSUniforms.View, camera->getView());
			m_NormalDisplayShader->setUniform(m_NDSUniforms.Projection, camera->getProjection());

			mesh->Render();
			m_NormalDisplayShader->UnBind();
		}
		if (camera->drawBoundingBoxes)
		{
			m_BBDisplayShader->Bind();
			m_BBDisplayShader->setUniform(m_BBDSUniforms.Model, transform);
			m_BBDisplayShader->setUniform(m_BBDSUniforms.View, camera->getView());
			m_BBDisplayShader->setUniform(m_BBDSUniforms.Projection, camera->getProjection());
			mesh->DrawBoudingBox();
			m_BBDisplayShader->UnBind();
		}

		camera->frameBuffer->Unbind();
	}
	void Renderer3D::RenderMesh(Model *mesh, const glm::mat4 &transform, Material *material, const size_t &directional, const std::vector<size_t> &pointLights, const std::vector<size_t> &spotLights, const std::vector<glm::mat4> &finalBoneMatrices, bool clear, Camera *camera, bool cull)
	{


		if (!camera)
		{
			camera = SceneManager::getActiveScene()->GetCameraManager().getActiveCamera();
		}

		glViewport(0, 0, camera->frameBuffer->getWidth(), camera->frameBuffer->getHeight());

		//prepare buffers

		glEnable(GL_DEPTH_TEST);
		glDepthFunc(GL_LESS);

		camera->frameBuffer->Bind(clear);

		Shader *matShader = material->getShader();
		matShader->Bind();

		matShader->SetMVP(transform, camera->getView(), camera->getProjection());

		matShader->SetBoneTransform(finalBoneMatrices);

		SetUpLight(matShader, camera, directional, pointLights, spotLights);
		material->Bind();
		mesh->Render();
		material->UnBind();

		if (mesh->showNormals)
		{
			m_NormalDisplayShader->Bind();
			m_NormalDisplayShader->setUniform(m_NDSUniforms.Model, transform);
			m_NormalDisplayShader->setUniform(m_NDSUniforms.View, camera->getView());
			m_NormalDisplayShader->setUniform(m_NDSUniforms.Projection, camera->getProjection());

			mesh->Render();
			m_NormalDisplayShader->UnBind();
		}
		if (camera->drawBoundingBoxes)
		{
			m_BBDisplayShader->Bind();
			m_BBDisplayShader->setUniform(m_BBDSUniforms.Model, transform);
			m_BBDisplayShader->setUniform(m_BBDSUniforms.View, camera->getView());
			m_BBDisplayShader->setUniform(m_BBDSUniforms.Projection, camera->getProjection());
			mesh->DrawBoudingBox();
			m_BBDisplayShader->UnBind();
		}

		camera->frameBuffer->Unbind();
	}

	void Renderer3D::RenderQuad(unsigned int vao, std::vector<int> ebo_data, const glm::vec3& position, const glm::vec3& rotation, const glm::vec3& scale, const size_t& directional,
		const std::vector<size_t>& pointLights, const std::vector<size_t>& spotLights, Material* material, bool clear, Camera* camera, bool cull, Image* texture, const Size2i& srcSize, float colorParticles[4], bool isColorRanged)
	{
		/*colorParticles[4] /= 255.0f;*/
		/*g /= 255.0f;
		b /= 255.0f;*/

		glViewport(0, 0, camera->frameBuffer->getWidth(), camera->frameBuffer->getHeight());

		camera->frameBuffer->Bind(clear);

		glm::mat4 model = glm::mat4(1.0f);
		model = glm::translate(model, position);
		model = glm::rotate(model, glm::radians(rotation.x), glm::vec3(1, 0, 0));
		model = glm::rotate(model, glm::radians(rotation.y), glm::vec3(0, 1, 0));
		model = glm::rotate(model, glm::radians(rotation.z), glm::vec3(0, 0, 1));
		model = glm::scale(model, scale);

		Shader* matShader = material->getShader();

		/*Shader* particleShader = Resources::GetResourceById<Shader>(m_ParticleShaderId);*/

		matShader->Bind();

		/*particleShader->setUniformVec3(particleShader->getUniform("u_Color")->location, glm::vec3(r, g ,b));*/

		matShader->SetMVP(model, camera->getView(), camera->getProjection());

		if (isColorRanged)
		{
			matShader->setUniformVec3(matShader->getUniform("u_ColorP")->location, glm::vec3(colorParticles[0], colorParticles[1], colorParticles[2]));
		}

		else
		{
			matShader->setUniformVec3(matShader->getUniform("u_ColorP")->location, glm::vec3(1.0, 1.0, 1.0));
		}

		/*matShader->setUniform(matShader->getUniform("u_ColorP")->location, 250);*/

		SetUpLight(matShader, camera, directional, pointLights, spotLights);

		material->Bind();

		/*glColor4f(r, g, b ,1);*/

		glDisable(GL_CULL_FACE);

		// make the drawing
		glBindVertexArray(vao);

		// Solution to the material.cpp GL_TEXTURE1 on the void Material::Bind() function
		glActiveTexture(GL_TEXTURE0);
		glBindTexture(GL_TEXTURE_2D, texture->GetTextureId());
		//------------------------------

		glDrawElements(GL_TRIANGLES, (GLsizei)ebo_data.size(), GL_UNSIGNED_INT, (GLsizei)0);
		glBindVertexArray(0);

		material->UnBind();
		glActiveTexture(GL_TEXTURE1);
		camera->frameBuffer->Unbind();
		glEnable(GL_CULL_FACE);
	}

	void Renderer3D::RenderSkybox()
	{
		{
			Camera *camera = SceneManager::getActiveScene()->GetCameraManager().getActiveCamera();
			if (camera)
			{
				glViewport(0, 0, camera->frameBuffer->getWidth(), camera->frameBuffer->getHeight());

				camera->frameBuffer->Bind(false);
				glDepthFunc(GL_ALWAYS);
				Shader* shader = m_DefaultSkybox.m_Material->getShader();
				shader->Bind();
				shader->setUniform(shader->getProjLoc(), camera->getProjection());
				glm::mat4 view = glm::mat4(glm::mat3(camera->getView()));
				shader->setUniform(shader->getViewLoc(), view);
				m_DefaultSkybox.Render();
				shader->UnBind();
				glDepthFunc(GL_LESS);

				camera->frameBuffer->Unbind();
			}
		}
		{
			Camera *camera = SceneManager::getActiveScene()->GetCameraManager().editorCamera;
			if (camera)
			{
				glViewport(0, 0, camera->frameBuffer->getWidth(), camera->frameBuffer->getHeight());

				camera->frameBuffer->Bind(false);
				glDepthFunc(GL_ALWAYS);
				Shader *shader = m_DefaultSkybox.m_Material->getShader();
				shader->Bind();
				shader->setUniform(shader->getProjLoc(), camera->getProjection());
				glm::mat4 view = glm::mat4(glm::mat3(camera->getView()));
				shader->setUniform(shader->getViewLoc(), view);
				m_DefaultSkybox.Render();
				shader->UnBind();
				glDepthFunc(GL_LESS);

				camera->frameBuffer->Unbind();
			}
		}
	}
	void Renderer3D::SetOption(Options option)
	{
		switch (option)
		{
		case Wiwa::Renderer3D::DEPTH_TEST:
			glEnable(GL_DEPTH_TEST);
			break;
		case Wiwa::Renderer3D::CULL_FACE:
			glEnable(GL_CULL_FACE);
			break;
		case Wiwa::Renderer3D::LIGHTING:
			glEnable(GL_LIGHTING);
			break;
		case Wiwa::Renderer3D::COLOR_MATERIAL:
			glEnable(GL_COLOR_MATERIAL);
			break;
		case Wiwa::Renderer3D::TEXTURE_2D:
			glEnable(GL_TEXTURE_2D);
			break;
		case Wiwa::Renderer3D::WIREFRAME:
			glPolygonMode(GL_FRONT_AND_BACK, GL_LINE);
			break;
		case Wiwa::Renderer3D::GAMMA_CORRECTION:
			glEnable(GL_FRAMEBUFFER_SRGB);
			break;
		default:
			break;
		}
	}
	void Renderer3D::DisableOption(Options option)
	{
		switch (option)
		{
		case Wiwa::Renderer3D::DEPTH_TEST:
			glDisable(GL_DEPTH_TEST);
			break;
		case Wiwa::Renderer3D::CULL_FACE:
			glDisable(GL_CULL_FACE);
			break;
		case Wiwa::Renderer3D::LIGHTING:
			glDisable(GL_LIGHTING);
			break;
		case Wiwa::Renderer3D::COLOR_MATERIAL:
			glDisable(GL_COLOR_MATERIAL);
			break;
		case Wiwa::Renderer3D::TEXTURE_2D:
			glDisable(GL_TEXTURE_2D);
			break;
		case Wiwa::Renderer3D::WIREFRAME:
			glPolygonMode(GL_FRONT_AND_BACK, GL_FILL);
			break;
		case Wiwa::Renderer3D::GAMMA_CORRECTION:
			glDisable(GL_FRAMEBUFFER_SRGB);
			break;
		default:
			break;
		}
	}
	void Renderer3D::Close()
	{
	}
	void Renderer3D::RenderFrustrums()
	{
		Camera *camera = SceneManager::getActiveScene()->GetCameraManager().editorCamera;
		glViewport(0, 0, camera->frameBuffer->getWidth(), camera->frameBuffer->getHeight());
		camera->frameBuffer->Bind(false);

		glMatrixMode(GL_PROJECTION);
		glLoadMatrixf(glm::value_ptr(camera->getProjection()));
		glMatrixMode(GL_MODELVIEW);
		glLoadMatrixf(glm::value_ptr(camera->getView()));

		CameraManager &cameraManager = SceneManager::getActiveScene()->GetCameraManager();

		size_t cameraCount = cameraManager.getCameraSize();
		std::vector<CameraId> &cameras = cameraManager.getCameras();
		for (size_t i = 0; i < cameraCount; i++)
		{
			Camera *cam = cameraManager.getCamera(cameras[i]);

			glUseProgram(0);
			glColor3f(0, 255, 0);
			glLineWidth(3.0f);
			glBegin(GL_LINES);

			glm::vec3 bb_frustum[8];
			cam->GetCornerPoints(bb_frustum);

			glVertex3f((GLfloat)bb_frustum[0].x, (GLfloat)bb_frustum[0].y, (GLfloat)bb_frustum[0].z);
			glVertex3f((GLfloat)bb_frustum[1].x, (GLfloat)bb_frustum[1].y, (GLfloat)bb_frustum[1].z);

			glVertex3f((GLfloat)bb_frustum[0].x, (GLfloat)bb_frustum[0].y, (GLfloat)bb_frustum[0].z);
			glVertex3f((GLfloat)bb_frustum[4].x, (GLfloat)bb_frustum[4].y, (GLfloat)bb_frustum[4].z);

			glVertex3f((GLfloat)bb_frustum[0].x, (GLfloat)bb_frustum[0].y, (GLfloat)bb_frustum[0].z);
			glVertex3f((GLfloat)bb_frustum[2].x, (GLfloat)bb_frustum[2].y, (GLfloat)bb_frustum[2].z);

			glVertex3f((GLfloat)bb_frustum[2].x, (GLfloat)bb_frustum[2].y, (GLfloat)bb_frustum[2].z);
			glVertex3f((GLfloat)bb_frustum[3].x, (GLfloat)bb_frustum[3].y, (GLfloat)bb_frustum[3].z);

			glVertex3f((GLfloat)bb_frustum[1].x, (GLfloat)bb_frustum[1].y, (GLfloat)bb_frustum[1].z);
			glVertex3f((GLfloat)bb_frustum[3].x, (GLfloat)bb_frustum[3].y, (GLfloat)bb_frustum[3].z);

			glVertex3f((GLfloat)bb_frustum[5].x, (GLfloat)bb_frustum[5].y, (GLfloat)bb_frustum[5].z);
			glVertex3f((GLfloat)bb_frustum[4].x, (GLfloat)bb_frustum[4].y, (GLfloat)bb_frustum[4].z);

			glVertex3f((GLfloat)bb_frustum[4].x, (GLfloat)bb_frustum[4].y, (GLfloat)bb_frustum[4].z);
			glVertex3f((GLfloat)bb_frustum[6].x, (GLfloat)bb_frustum[6].y, (GLfloat)bb_frustum[6].z);

			glVertex3f((GLfloat)bb_frustum[7].x, (GLfloat)bb_frustum[7].y, (GLfloat)bb_frustum[7].z);
			glVertex3f((GLfloat)bb_frustum[5].x, (GLfloat)bb_frustum[5].y, (GLfloat)bb_frustum[5].z);

			glVertex3f((GLfloat)bb_frustum[7].x, (GLfloat)bb_frustum[7].y, (GLfloat)bb_frustum[7].z);
			glVertex3f((GLfloat)bb_frustum[6].x, (GLfloat)bb_frustum[6].y, (GLfloat)bb_frustum[6].z);

			glVertex3f((GLfloat)bb_frustum[1].x, (GLfloat)bb_frustum[1].y, (GLfloat)bb_frustum[1].z);
			glVertex3f((GLfloat)bb_frustum[5].x, (GLfloat)bb_frustum[5].y, (GLfloat)bb_frustum[5].z);

			glVertex3f((GLfloat)bb_frustum[6].x, (GLfloat)bb_frustum[6].y, (GLfloat)bb_frustum[6].z);
			glVertex3f((GLfloat)bb_frustum[2].x, (GLfloat)bb_frustum[2].y, (GLfloat)bb_frustum[2].z);

			glVertex3f((GLfloat)bb_frustum[7].x, (GLfloat)bb_frustum[7].y, (GLfloat)bb_frustum[7].z);
			glVertex3f((GLfloat)bb_frustum[3].x, (GLfloat)bb_frustum[3].y, (GLfloat)bb_frustum[3].z);

			glEnd();
			glLineWidth(1.0f);
		}
		camera->frameBuffer->Unbind();
	}
	void Renderer3D::SetUpLight(Wiwa::Shader *matShader, Wiwa::Camera *camera, const size_t &directional, const std::vector<size_t> &pointLights, const std::vector<size_t> &spotLights)
	{
		matShader->setUniform(matShader->getUniformLocation("u_CameraPosition"), camera->getPosition());
		if (directional != -1)
		{
			DirectionalLight *dirLight = SceneManager::getActiveScene()->GetEntityManager().GetComponent<DirectionalLight>(directional);
			Transform3D *transform = SceneManager::getActiveScene()->GetEntityManager().GetComponent<Transform3D>(directional);
			if (dirLight && transform)
			{
				matShader->setUniform(matShader->getUniformLocation("u_DirectionalLight.Base.Color"), dirLight->Color);
				matShader->setUniform(matShader->getUniformLocation("u_DirectionalLight.Base.AmbientIntensity"), dirLight->AmbientIntensity);
				matShader->setUniform(matShader->getUniformLocation("u_DirectionalLight.Base.DiffuseIntensity"), dirLight->DiffuseIntensity);
				matShader->setUniform(matShader->getUniformLocation("u_DirectionalLight.Direction"), glm::radians(transform->localRotation));
			}
		}
		else
		{
			matShader->setUniform(matShader->getUniformLocation("u_DirectionalLight.Base.Color"), glm::vec3(0.0f));
			matShader->setUniform(matShader->getUniformLocation("u_DirectionalLight.Base.AmbientIntensity"), 0.0f);
			matShader->setUniform(matShader->getUniformLocation("u_DirectionalLight.Base.DiffuseIntensity"), 0.0f);
			matShader->setUniform(matShader->getUniformLocation("u_DirectionalLight.Direction"), glm::vec3(0.0f));
		}
		matShader->setUniform(matShader->getUniformLocation("u_NumPointLights"), (int)pointLights.size());
		for (size_t i = 0; i < pointLights.size(); i++)
		{
			Transform3D *transform = SceneManager::getActiveScene()->GetEntityManager().GetComponent<Transform3D>(pointLights[i]);
			PointLight *pointLight = SceneManager::getActiveScene()->GetEntityManager().GetComponent<PointLight>(pointLights[i]);
			std::string num = std::to_string(i);

			if (transform && pointLight)
			{

				matShader->setUniform(matShader->getUniformLocation(("u_PointLights[" + num + "].Base.Color").c_str()), pointLight->Color);
				matShader->setUniform(matShader->getUniformLocation(("u_PointLights[" + num + "].Base.AmbientIntensity").c_str()), pointLight->AmbientIntensity);
				matShader->setUniform(matShader->getUniformLocation(("u_PointLights[" + num + "].Base.DiffuseIntensity").c_str()), pointLight->DiffuseIntensity);
				matShader->setUniform(matShader->getUniformLocation(("u_PointLights[" + num + "].LocalPos").c_str()), transform->position);
				matShader->setUniform(matShader->getUniformLocation(("u_PointLights[" + num + "].Atten.Constant").c_str()), pointLight->Constant);
				matShader->setUniform(matShader->getUniformLocation(("u_PointLights[" + num + "].Atten.Linear").c_str()), pointLight->Linear);
				matShader->setUniform(matShader->getUniformLocation(("u_PointLights[" + num + "].Atten.Exp").c_str()), pointLight->Exp);
			}
		}

		matShader->setUniform(matShader->getUniformLocation("u_NumSpotLights"), (int)spotLights.size());
		for (size_t i = 0; i < pointLights.size(); i++)
		{
			Transform3D *transform = SceneManager::getActiveScene()->GetEntityManager().GetComponent<Transform3D>(pointLights[i]);
			SpotLight *spotLight = SceneManager::getActiveScene()->GetEntityManager().GetComponent<SpotLight>(pointLights[i]);
			std::string num = std::to_string(i);

			if (transform && spotLight)
			{

				matShader->setUniform(matShader->getUniformLocation(("u_SpotLights[" + num + "].Base.Base.Color").c_str()), spotLight->Color);
				matShader->setUniform(matShader->getUniformLocation(("u_SpotLights[" + num + "].Base.Base.AmbientIntensity").c_str()), spotLight->AmbientIntensity);
				matShader->setUniform(matShader->getUniformLocation(("u_SpotLights[" + num + "].Base.Base.DiffuseIntensity").c_str()), spotLight->DiffuseIntensity);
				matShader->setUniform(matShader->getUniformLocation(("u_SpotLights[" + num + "].Base.LocalPos").c_str()), transform->position);
				matShader->setUniform(matShader->getUniformLocation(("u_SpotLights[" + num + "].Base.Atten.Constant").c_str()), spotLight->Constant);
				matShader->setUniform(matShader->getUniformLocation(("u_SpotLights[" + num + "].Base.Atten.Linear").c_str()), spotLight->Linear);
				matShader->setUniform(matShader->getUniformLocation(("u_SpotLights[" + num + "].Base.Atten.Exp").c_str()), spotLight->Exp);
				matShader->setUniform(matShader->getUniformLocation(("u_SpotLights[" + num + "].Direction").c_str()), glm::radians(spotLight->Direction));
				matShader->setUniform(matShader->getUniformLocation(("u_SpotLights[" + num + "].Cuttoff").c_str()), spotLight->Cutoff);
			}
		}
	}
	void Renderer3D::RenderShadows(Camera *camera, const size_t &directional)
	{
	}
}