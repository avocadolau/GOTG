#include <wipch.h>
#include <Wiwa/core/Input.h>
#include "UiBar.h"
#include "Wiwa/ui/UiManager.h"
#include <Wiwa/ecs/systems/System.h>
#include <Wiwa/core/Renderer2D.h>
namespace Wiwa
{
	GuiBar::GuiBar(Scene* scene, unsigned int id, Rect2i bounds, Rect2i sliderBounds, const char* path, const char* slider_path, Rect2i boundsOriginTex, Rect2i sliderOriginTex, bool active) : GuiControl(scene, GuiControlType::BAR, id)
	{
		this->position = bounds;
		this->extraPosition = sliderBounds;
		texturePosition = boundsOriginTex;
		extraTexturePosition = sliderOriginTex;
		name = "UI_bar";
		m_Scene = scene;
		this->active = active;
		text = "none";
		callbackID = WI_INVALID_INDEX;
		Wiwa::Renderer2D& r2d = Wiwa::Application::Get().GetRenderer2D();
		textId2 = Wiwa::Resources::Load<Wiwa::Image>(slider_path);
		extraTexture = Wiwa::Resources::GetResourceById<Wiwa::Image>(textId2);
		/*textId1 = Wiwa::Resources::Load<Wiwa::Image>(path);
		texture = Wiwa::Resources::GetResourceById<Wiwa::Image>(textId1);*/

		//id_quad_normal = r2d.CreateInstancedQuadTex(m_Scene, texture->GetTextureId(), texture->GetSize(), { position.x,position.y }, { position.width,position.height }, texturePosition, Wiwa::Renderer2D::Pivot::UPLEFT);
		id_quad_extra = r2d.CreateInstancedQuadTex(m_Scene, extraTexture->GetTextureId(), extraTexture->GetSize(), { extraPosition.x,extraPosition.y }, { extraPosition.width,extraPosition.height }, extraTexturePosition, Wiwa::Renderer2D::Pivot::UPLEFT);

		state = GuiControlState::NORMAL;

		if (!active)
		{
			//r2d.DisableInstance(m_Scene, id_quad_normal);
			r2d.DisableInstance(m_Scene, id_quad_extra);

		}
	}

	GuiBar::~GuiBar()
	{

	}


	bool GuiBar::Draw(Renderer2D* render)
	{
		
		render->UpdateInstancedQuadTexPosition(m_Scene, id_quad_extra, { position.x,position.y }, Wiwa::Renderer2D::Pivot::UPLEFT);
		render->UpdateInstancedQuadTexClip(m_Scene, id_quad_extra, extraTexture->GetSize(),texturePosition);
		render->UpdateInstancedQuadTexSize(m_Scene, id_quad_extra, { position.x,position.y }, { position.width,position.height }, Wiwa::Renderer2D::Pivot::UPLEFT);
		return false;
	}
}