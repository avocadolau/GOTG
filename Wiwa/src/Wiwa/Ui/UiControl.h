#pragma once

#include <Wiwa/core/Core.h>
#include <Wiwa/core/Renderer2D.h>

#include <vector>
#include <string>

#include <Wiwa/scene/Scene.h>

namespace Wiwa
{
	enum class GuiControlType
	{
		BUTTON,
		TEXT,
		TOGGLE,
		CHECKBOX,
		SLIDER,
		INPUTBOX,
		IMAGE
	};

	enum class GuiControlState
	{
		DISABLED,
		NORMAL,
		FOCUSED,
		PRESSED,
		SELECTED
	};

	class WI_API GuiControl
	{
	protected:
		Scene* m_Scene;
	public:

		GuiControl(Scene* scene, GuiControlType type, unsigned int id) : m_Scene(scene), type(type), id(id), state(GuiControlState::NORMAL) {}

		GuiControl(Scene* scene, GuiControlType type, Rect2i bounds, char text[32]) : m_Scene(scene), type(type), state(GuiControlState::NORMAL)
		{
		}
		



		virtual bool Update()
		{
			return true;
		}

		virtual bool Draw(Renderer2D* render)
		{
			return true;
		}

		void SetTexture(Image* tex)
		{
			texture = tex;

		}
		void SetSliderTexture(Image* tex)
		{
			extraTexture = tex;

		}
		bool GetActive()
		{
			return active;
		}
		Rect2i GetPosition()
		{
			return position;
		}
		Rect2i GetExtraPosition()
		{
			return extraPosition;
		}
		GuiControlType GetType()
		{
			return type;
		}
		GuiControlState GetState()
		{
			return state;
		}
		Image* GetTexture()
		{
			return texture;
		}
		Image* GetExtraTexture()
		{
			return extraTexture;
		}
		uint32_t GetDisabledIdQuad()
		{
			return id_quad_disabled;
		}
		uint32_t GetNormalIdQuad()
		{
			return id_quad_normal;
		}
		uint32_t GetFocusedIdQuad()
		{
			return id_quad_focused;
		}
		uint32_t GetPressedIdQuad()
		{
			return id_quad_pressed;
		}
		uint32_t GetSelectedIdQuad()
		{
			return id_quad_selected;
		}
		uint32_t GetExtraIdQuad()
		{
			return id_quad_extra;
		}

		void SetPosition_x(int position_x)
		{
			position.x = position_x;
			
		}
		void SetPosition_y(int position_y)
		{
			position.y = position_y;

		}
		void SetExtraPosition_x(int position_x)
		{
			extraPosition.x = position_x;

		}
		void SetExtraPosition_y(int position_y)
		{
			extraPosition.y = position_y;

		}
		void SetPosition_width(int position_width)
		{
			position.width = position_width;

		}
		void SetPosition_height(int position_height)
		{
			position.height = position_height;

		}

		void SwapActive(Wiwa::Renderer2D& r2d)
		{
			active = !active;

			if (active)
			{
				r2d.EnableInstance(m_Scene, id_quad_disabled);
				r2d.EnableInstance(m_Scene, id_quad_normal);
				r2d.EnableInstance(m_Scene, id_quad_focused);
				r2d.EnableInstance(m_Scene, id_quad_pressed);
				r2d.EnableInstance(m_Scene, id_quad_selected);

				if (type == GuiControlType::SLIDER)
				{
					r2d.EnableInstance(m_Scene, id_quad_extra);
				}
			}
			else
			{
				r2d.DisableInstance(m_Scene, id_quad_disabled);
				r2d.DisableInstance(m_Scene, id_quad_normal);
				r2d.DisableInstance(m_Scene, id_quad_focused);
				r2d.DisableInstance(m_Scene, id_quad_pressed);
				r2d.DisableInstance(m_Scene, id_quad_selected);

				if (type == GuiControlType::SLIDER)
				{
					r2d.DisableInstance(m_Scene, id_quad_extra);
				}
			}
		}

	public:

		ResourceId textId1;
		ResourceId textId2;

		bool active;
		int id;
		GuiControlType type;
		GuiControlState state;

		char text[32];           // Control text (if required)
		Rect2i position;        // Position and size
		Rect2i extraPosition;	// Position and size for the bar
		Color4f color;        // Tint color

		Image* texture;
		Image* extraTexture;	// Texture atlas reference
		uint32_t id_quad_disabled;
		uint32_t id_quad_normal;
		uint32_t id_quad_focused;
		uint32_t id_quad_pressed;
		uint32_t id_quad_selected;

		uint32_t id_quad_extra;
	};
}