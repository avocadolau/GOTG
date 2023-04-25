#include <wipch.h>
#include <Wiwa/core/Input.h>
#include "UiVideo.h"
#include "Wiwa/ui/UiManager.h"
#include <Wiwa/ecs/systems/System.h>
#include <Wiwa/core/Renderer2D.h>
//#include <Wiwa/core/Resources.h>
namespace Wiwa
{
	GuiVideo::GuiVideo(Scene* scene, unsigned int id, Rect2i bounds, const char* pathVideo, bool active) : GuiControl(scene, GuiControlType::VIDEO, id)
	{
		this->position = bounds;
		name = "Video";
		text = "none";
		this->active = active;
		audioEventForButton = "none";
		callbackID = WI_INVALID_INDEX;

		Wiwa::GuiManager& gm = Wiwa::SceneManager::getActiveScene()->GetGuiManager();
		textId1 = Wiwa::Resources::Load<Wiwa::Video>(pathVideo);
		video_res = Wiwa::Resources::GetResourceById<Wiwa::Video>(textId1);
	/*	video_res = new Video();
		video_res = gm.InitVideo(pathVideo);*/
		m_Scene = scene;
		Wiwa::Renderer2D& r2d = Wiwa::Application::Get().GetRenderer2D();
		id_quad_normal = r2d.CreateInstancedQuadTex(m_Scene, video_res->GetTextureId(), video_res->GetSize(), { position.x,position.y }, { position.width,position.height }, Wiwa::Renderer2D::Pivot::UPLEFT);

		if (!active)
		{
			r2d.DisableInstance(m_Scene, id_quad_normal);

		}
		video_res->Replay();

		this->rotation = rotation;
	}

	GuiVideo::~GuiVideo()
	{

	}


	bool GuiVideo::Update()
	{
		video_res->Update();
		return true;
	}

	bool GuiVideo::Draw(Renderer2D* render)
	{
		render->UpdateInstancedQuadTexSize(m_Scene, id_quad_normal, { position.x,position.y }, { position.width,position.height }, Wiwa::Renderer2D::Pivot::CENTER);
		render->UpdateInstancedQuadTexPosition(m_Scene, id_quad_normal, { position.x,position.y }, Wiwa::Renderer2D::Pivot::CENTER);
		render->UpdateInstancedQuadTexTexture(m_Scene, id_quad_normal, video_res->GetTextureId());

		return false;
	}
}