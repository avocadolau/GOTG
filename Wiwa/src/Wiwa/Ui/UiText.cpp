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
		Wiwa::GuiManager& gm = Wiwa::SceneManager::getActiveScene()->GetGuiManager();
		this->position = bounds;
		this->texture = texture;
		gm.InitFont("assets/arial.ttf", (char*)string_text);
		std::string path = "assets/";
		path.append(string_text);
		path.append(".png");
		Wiwa::Resources::CheckImport<Image>(path.c_str());
		
		textId1 = Wiwa::Resources::Load<Wiwa::Image>(path.c_str());
		texture = Wiwa::Resources::GetResourceById<Wiwa::Image>(textId1);

		Wiwa::Renderer2D& r2d = Wiwa::Application::Get().GetRenderer2D();
		id_quad_normal = r2d.CreateInstancedQuadTex(m_Scene, texture->GetTextureId(), texture->GetSize(), { position.x,position.y }, { position.width,position.height }, Wiwa::Renderer2D::Pivot::CENTER);
		
		
	}

	GuiText::~GuiText()
	{

	}


	bool GuiText::Draw(Renderer2D* render)
	{
		// Draw the right button depending on state
		Wiwa::Renderer2D& r2d_1 = Wiwa::Application::Get().GetRenderer2D();

		Vector2i positionForUpdate_1;
		
		positionForUpdate_1.x = position.x;
		positionForUpdate_1.y = position.y;
		positionForUpdate_1.w = position.width;
		positionForUpdate_1.h = position.height;
		r2d_1.UpdateInstancedQuadTex(m_Scene, id_quad_normal, positionForUpdate_1, Wiwa::Renderer2D::Pivot::UPLEFT);

		return false;
	}
}