#include <wipch.h>
#include "Renderer3D.h"

#include <Wiwa/core/Application.h>
#include <Wiwa/utilities/Log.h>
#include <Wiwa/utilities/render/CameraManager.h>
#include <Wiwa/scene/SceneManager.h>
#include <glew.h>
#include <Wiwa/utilities/render/shaders/Shader.h>

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

		m_DepthShaderId = Resources::Load<Shader>("resources/shaders/light/depth");
		m_DepthShader = Resources::GetResourceById<Shader>(m_DepthShaderId);
		m_DepthShader->Compile("resources/shaders/light/depth");
		m_DepthShaderUniforms.Model = m_DepthShader->getUniformLocation("u_Model");
		m_DepthShaderUniforms.View = m_DepthShader->getUniformLocation("u_View");
		m_DepthShaderUniforms.Projection = m_DepthShader->getUniformLocation("u_Proj");

		std::vector<const char *> faces = {
			"resources/images/skybox/right.jpg",
			"resources/images/skybox/left.jpg",
			"resources/images/skybox/top.jpg",
			"resources/images/skybox/bottom.jpg",
			"resources/images/skybox/front.jpg",
			"resources/images/skybox/back.jpg"};

		m_DefaultSkybox.LoadCubemap(faces);

		return true;
	}

	void Renderer3D::Update()
	{
		OPTICK_EVENT("Renderer 3D Update");
		RenderSkybox();
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
			;
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
		//// Setting the shadow map buffer
		// camera->frameBuffer->BindDepth(true);

		// Wiwa::Transform3D* lightTrans = Wiwa::SceneManager::getActiveScene()->GetEntityManager().GetComponent<Wiwa::Transform3D>(directional);
		// glm::mat4 view;
		// glm::mat4 projection;
		// if (lightTrans)
		//{
		//	view = glm::lookAt(
		//		lightTrans->localPosition,
		//		lightTrans->localPosition + glm::radians(lightTrans->localRotation),
		//		glm::vec3(0.0f, 1.0f, 0.0f));
		//	projection = glm::ortho(-10.0f, 10.0f, -10.0f, 10.0f, 0.1f, 1000.0f);

		//	m_DepthShader->Bind();
		//	m_DepthShader->setUniform(m_DepthShaderUniforms.Projection, projection);
		//	m_DepthShader->setUniform(m_DepthShaderUniforms.View, view);

		//	m_DepthShader->setUniform(m_DepthShaderUniforms.Model, lightTrans->localMatrix);
		//}

		// mesh->Render();

		// m_DepthShader->UnBind();

		// camera->frameBuffer->UnbindDepth();

		// Set up color buffer

		glViewport(0, 0, camera->frameBuffer->getWidth(), camera->frameBuffer->getHeight());

		camera->frameBuffer->Bind(clear);

		Shader *matShader = material->getShader();
		matShader->Bind();
		matShader->SetMVP(transform, camera->getView(), camera->getProjection());

		// if (lightTrans)
		//{
		//	glm::mat4 lightMVP = lightTrans->localMatrix * projection * view;

		//	matShader->setUniform(matShader->getUniformLocation("u_Light"), lightMVP);
		//}
		// camera->frameBuffer->SetupDepth(GL_TEXTURE1);

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
	void Renderer3D::RenderSkybox()
	{
		{
			Camera *camera = SceneManager::getActiveScene()->GetCameraManager().getActiveCamera();
			if (camera)
			{
				glViewport(0, 0, camera->frameBuffer->getWidth(), camera->frameBuffer->getHeight());
				camera->frameBuffer->Bind(false);
				glDepthFunc(GL_LEQUAL);
				Shader *shader = m_DefaultSkybox.m_Material->getShader();
				shader->Bind();
				shader->setUniform(shader->getProjLoc(), camera->getProjection());
				shader->setUniform(shader->getViewLoc(), camera->getView());
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
				glDepthFunc(GL_LEQUAL);
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