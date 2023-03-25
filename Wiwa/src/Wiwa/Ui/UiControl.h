#pragma once

#include <Wiwa/core/Core.h>
#include <Wiwa/core/Renderer2D.h>
#include <Wiwa/utilities/functions/Callback.h>
#include <Wiwa/utilities/render/Text.h>
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
		IMAGE,
		BAR,
	};

	enum class GuiControlState
	{
		DISABLED,
		NORMAL,
		FOCUSED,
		PRESSED,
		SELECTED
	};

	enum class GuiEvent
	{
		PLAY,
		TEST,
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
				
				r2d.EnableInstance(m_Scene, id_quad_normal);
				

				if (type == GuiControlType::SLIDER)
				{
					r2d.EnableInstance(m_Scene, id_quad_extra);
				}
				if (type == GuiControlType::BAR)
				{
					r2d.EnableInstance(m_Scene, id_quad_extra);
				}
				
			}
			else
			{
				r2d.DisableInstance(m_Scene, id_quad_normal);

				if (type == GuiControlType::SLIDER)
				{
					r2d.DisableInstance(m_Scene, id_quad_extra);
				}
				if (type == GuiControlType::BAR)
				{
					r2d.DisableInstance(m_Scene, id_quad_extra);
				}
			}
		}
		bool SwapToNewTexture(const char* path, Wiwa::Renderer2D& r2d)
		{
			
			textId1 = Wiwa::Resources::Load<Wiwa::Image>(path);
			texture = Wiwa::Resources::GetResourceById<Wiwa::Image>(textId1);

			r2d.UpdateInstancedQuadTexTexture(m_Scene, id_quad_normal, texture->GetTextureId()); 
			return true;
		}

		void SwapText(const char* word, Wiwa::Renderer2D& r2d)
		{
			/*text = word;
			Text* newText = new Text();
			newText = Text::InitFont("assets/Fonts/arial.ttf", (char*)word);
			
			r2d.UpdateInstancedQuadTexTexture(m_Scene, id_quad_normal, newText->GetTextureId());*/
		}

		void SetValueForUIbar(float value)
		{
			if (type == GuiControlType::BAR)
			{
				extraPosition.width = ((value * (float)position.width) / 100);
			}
		}
		
	public:

		ResourceId textId1;
		ResourceId textId2;

		bool active;
		int id;
		GuiControlType type;
		GuiControlState state;
		GuiEvent event;

		std::string text;           // Control text (if required)
		std::string audioEventForButton;           // Control text (if required)
		Rect2i position;        // Position and size
		Rect2i texturePosition;        
		Rect2i extraPosition;	// Position and size for the bar
		Rect2i extraTexturePosition;        
		Color4f color = Color::WHITE;        // Tint color

		Image* texture;
		Image* extraTexture;	// Texture atlas reference

		uint32_t id_quad_normal;

		uint32_t id_quad_extra;

		size_t callbackID;

		std::string name;
		Callback* callback;

		//THINGS FOR ANIMATIONS
		List<Rect2i> positionsForAnimations;
		bool animated; 
		size_t framesAnimation;
		float animSpeed;
	};
}