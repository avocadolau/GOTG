#include <wipch.h>
#include <Wiwa/core/Input.h>
#include "UiText.h"
#include "Wiwa/ui/UiManager.h"
#include <Wiwa/ecs/systems/System.h>
#include <Wiwa/core/Renderer2D.h>
namespace Wiwa
{
	GuiText::GuiText(Scene* scene, unsigned int id, Rect2i bounds, const char* string_text) : GuiControl(scene, GuiControlType::TEXT, id)
	{
		this->position = bounds;
		name = "Text";
		active = true;

		Wiwa::GuiManager& gm = Wiwa::SceneManager::getActiveScene()->GetGuiManager();
		text = new Text();
		text = gm.InitFont("assets/Fonts/BADABB_.TTF", (char*)string_text);
		m_Scene = scene;
		Wiwa::Renderer2D& r2d = Wiwa::Application::Get().GetRenderer2D();
		id_quad_normal = r2d.CreateInstancedQuadTex(m_Scene, text->GetTextureId(), text->GetSize(), { position.x,position.y }, { position.width,position.height }, Wiwa::Renderer2D::Pivot::UPLEFT);
		
		
	}

	GuiText::~GuiText()
	{

	}


	bool GuiText::Draw(Renderer2D* render)
	{
		// Draw the right button depending on state
		Wiwa::Renderer2D& r2d_1 = Wiwa::Application::Get().GetRenderer2D();

		Vector2i newPosition;
		newPosition.x = this->position.x;
		newPosition.y = this->position.y;
		Size2i newSize;
		newSize.w = this->position.width;
		newSize.h = this->position.height;
		render->UpdateInstancedQuad(m_Scene, id_quad_normal, newPosition, newSize, color);
	

		return false;
	}
}