#pragma once

#include <Wiwa/core/Core.h>
#include <Wiwa/core/Renderer2D.h>

#include <vector>
#include <string>

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
	public:

		GuiControl(GuiControlType type, unsigned int id) : type(type), id(id), state(GuiControlState::NORMAL) {}

		GuiControl(GuiControlType type, Rect2i bounds, char text[32]) :type(type), state(GuiControlState::NORMAL)
		{
			//color.r = 255; color.g = 255; color.b = 255;
			texture = nullptr;
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
		uint32_t GetIdQuad()
		{
			return id_quad;
		}
		uint32_t GetExtraIdQuad()
		{
			return id_quad2;
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
				r2d.EnableInstance(id_quad);
				if (type == GuiControlType::SLIDER)
				{
					r2d.EnableInstance(id_quad2);
				}
			}
			else
			{
				r2d.DisableInstance(id_quad);
				if (type == GuiControlType::SLIDER)
				{
					r2d.DisableInstance(id_quad2);
				}
			}
		}

	public:

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
		uint32_t id_quad;

		uint32_t id_quad2;
	};
}