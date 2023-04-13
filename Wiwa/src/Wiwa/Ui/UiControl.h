#pragma once

#include <Wiwa/core/Core.h>
#include <Wiwa/core/Renderer2D.h>
#include <Wiwa/utilities/functions/Callback.h>
#include <Wiwa/utilities/render/Text.h>
#include <vector>
#include <string>
#include <Wiwa/utilities/easings.h>
#include <Wiwa/utilities/time/Time.h>
#include <Wiwa/scene/Scene.h>

namespace Wiwa
{
	enum class GuiControlEasing
	{
		LinearIn,
		LinearOut,
		LinearInOut,
		SineIn,
		SineOut,
		SineInOut,
		CubicIn,
		CubicOut,
		CubicInOut,
		QuadIn,
		QuadOut,
		QuadInOut,
		ExpoIn,
		ExpoOut,
		ExpoInOut,
		EaseBackIn,
		EaseBackOut,
		EaseBackInOut,
		EaseBounceIn,
		EaseBounceOut,
		EaseBounceInOut,
		EaseElasticIn,
		EaseElasticOut,
		EaseElasticInOut,
	};
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
		ABILITY,
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

		Renderer2D::InstanceData GetExtraIdQuad()
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
				if (type != GuiControlType::BAR)
				{
					r2d.EnableInstance(m_Scene, id_quad_normal);
				}
				

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
				if (type != GuiControlType::BAR)
				{
					r2d.DisableInstance(m_Scene, id_quad_normal);
				}

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

		void SwapText(const char* word, Wiwa::Renderer2D& r2d,Text* newText)
		{
			text = word;
			r2d.UpdateInstancedQuadTexTexture(m_Scene, id_quad_normal, newText->GetTextureId());
		}

		void SetValueForUIbar(float valueHealth,float valueMaxHealth)
		{
			if (type == GuiControlType::BAR)
			{
				float proportion = valueHealth / valueMaxHealth;
				extraPosition.width = (int)(proportion * position.width);
				extraTexturePosition.width = extraPosition.width;
			}
		}

		//Used for abilites,buffs,etc 
		void SetNextFrame(int value, Wiwa::Renderer2D* r2d)
		{
			if (animatedControl && type == GuiControlType::ABILITY)
			{
				r2d->UpdateInstancedQuadTexClip(m_Scene, id_quad_normal, texture->GetSize(), positionsForAnimations.at(value));
			}
		}

		void HandleAnim(Wiwa::Renderer2D* r2d)
		{
			if (animatedControl)
			{
				timeForAnim += 1.0f;
				if (timeForAnim >= animSpeed)
				{
					if (framesAnimation < positionsForAnimations.size() - 1)
					{
						framesAnimation++;
					}
					else
					{
						framesAnimation = 0;
					}
					timeForAnim = 0.0f;
					r2d->UpdateInstancedQuadTexClip(m_Scene, id_quad_normal, texture->GetSize(), positionsForAnimations.at(framesAnimation));

				}
			}
		}

		void MoveGUIElement(Vector2i desired_position,int duration,Vector2i starting_position,GuiControlEasing easingType)
		{
			switch (easingType)
			{
			case Wiwa::GuiControlEasing::LinearIn:
				if (position.x < desired_position.x)
				{
					position.x = EaseLinearIn(Wiwa::Time::GetDeltaTime(), starting_position.x, desired_position.x - starting_position.x, duration);
				}
				if (position.y < desired_position.y)
				{
					position.y = EaseLinearIn(Wiwa::Time::GetDeltaTime(), starting_position.y, desired_position.y - starting_position.y, duration);
				}
				break;
			case Wiwa::GuiControlEasing::LinearOut:
				if (position.x < desired_position.x)
				{
					position.x = EaseLinearOut(Wiwa::Time::GetDeltaTime(), starting_position.x, desired_position.x - starting_position.x, duration);
				}
				if (position.y < desired_position.y)
				{
					position.y = EaseLinearOut(Wiwa::Time::GetDeltaTime(), starting_position.y, desired_position.y - starting_position.y, duration);
				}
				break;
			case Wiwa::GuiControlEasing::LinearInOut:
				if (position.x < desired_position.x)
				{
					position.x = EaseLinearInOut(Wiwa::Time::GetDeltaTime(), starting_position.x, desired_position.x - starting_position.x, duration);
				}
				if (position.y < desired_position.y)
				{
					position.y = EaseLinearInOut(Wiwa::Time::GetDeltaTime(), starting_position.y, desired_position.y - starting_position.y, duration);
				}
				break;
			case Wiwa::GuiControlEasing::SineIn:
				if (position.x < desired_position.x)
				{
					position.x = EaseSineIn(Wiwa::Time::GetDeltaTime(), starting_position.x, desired_position.x - starting_position.x, duration);
				}
				if (position.y < desired_position.y)
				{
					position.y = EaseSineIn(Wiwa::Time::GetDeltaTime(), starting_position.y, desired_position.y - starting_position.y, duration);
				}
				break;
			case Wiwa::GuiControlEasing::SineOut:
				if (position.x < desired_position.x)
				{
					position.x = EaseSineOut(Wiwa::Time::GetDeltaTime(), starting_position.x, desired_position.x - starting_position.x, duration);
				}
				if (position.y < desired_position.y)
				{
					position.y = EaseSineOut(Wiwa::Time::GetDeltaTime(), starting_position.y, desired_position.y - starting_position.y, duration);
				}
				break;
			case Wiwa::GuiControlEasing::SineInOut:
				if (position.x < desired_position.x)
				{
					position.x = EaseSineInOut(Wiwa::Time::GetDeltaTime(), starting_position.x, desired_position.x - starting_position.x, duration);
				}
				if (position.y < desired_position.y)
				{
					position.y = EaseSineInOut(Wiwa::Time::GetDeltaTime(), starting_position.y, desired_position.y - starting_position.y, duration);
				}
				break;
			case Wiwa::GuiControlEasing::CubicIn:
				if (position.x < desired_position.x)
				{
					position.x = EaseCubicIn(Wiwa::Time::GetDeltaTime(), starting_position.x, desired_position.x - starting_position.x, duration);
				}
				if (position.y < desired_position.y)
				{
					position.y = EaseCubicIn(Wiwa::Time::GetDeltaTime(), starting_position.y, desired_position.y - starting_position.y, duration);
				}
				break;
			case Wiwa::GuiControlEasing::CubicOut:
				if (position.x < desired_position.x)
				{
					position.x = EaseCubicOut(Wiwa::Time::GetDeltaTime(), starting_position.x, desired_position.x - starting_position.x, duration);
				}
				if (position.y < desired_position.y)
				{
					position.y = EaseCubicOut(Wiwa::Time::GetDeltaTime(), starting_position.y, desired_position.y - starting_position.y, duration);
				}
				break;
			case Wiwa::GuiControlEasing::CubicInOut:
				if (position.x < desired_position.x)
				{
					position.x = EaseCubicInOut(Wiwa::Time::GetDeltaTime(), starting_position.x, desired_position.x - starting_position.x, duration);
				}
				if (position.y < desired_position.y)
				{
					position.y = EaseCubicInOut(Wiwa::Time::GetDeltaTime(), starting_position.y, desired_position.y - starting_position.y, duration);
				}
				break;
			case Wiwa::GuiControlEasing::QuadIn:
				if (position.x < desired_position.x)
				{
					position.x = EaseQuadIn(Wiwa::Time::GetDeltaTime(), starting_position.x, desired_position.x - starting_position.x, duration);
				}
				if (position.y < desired_position.y)
				{
					position.y = EaseQuadIn(Wiwa::Time::GetDeltaTime(), starting_position.y, desired_position.y - starting_position.y, duration);
				}
				break;
			case Wiwa::GuiControlEasing::QuadOut:
				if (position.x < desired_position.x)
				{
					position.x = EaseQuadOut(Wiwa::Time::GetDeltaTime(), starting_position.x, desired_position.x - starting_position.x, duration);
				}
				if (position.y < desired_position.y)
				{
					position.y = EaseQuadOut(Wiwa::Time::GetDeltaTime(), starting_position.y, desired_position.y - starting_position.y, duration);
				}
				break;
			case Wiwa::GuiControlEasing::QuadInOut:
				if (position.x < desired_position.x)
				{
					position.x = EaseQuadInOut(Wiwa::Time::GetDeltaTime(), starting_position.x, desired_position.x - starting_position.x, duration);
				}
				if (position.y < desired_position.y)
				{
					position.y = EaseQuadInOut(Wiwa::Time::GetDeltaTime(), starting_position.y, desired_position.y - starting_position.y, duration);
				}
				break;
			case Wiwa::GuiControlEasing::ExpoIn:
				if (position.x < desired_position.x)
				{
					position.x = EaseExpoIn(Wiwa::Time::GetDeltaTime(), starting_position.x, desired_position.x - starting_position.x, duration);
				}
				if (position.y < desired_position.y)
				{
					position.y = EaseExpoIn(Wiwa::Time::GetDeltaTime(), starting_position.y, desired_position.y - starting_position.y, duration);
				}
				break;
			case Wiwa::GuiControlEasing::ExpoOut:
				if (position.x < desired_position.x)
				{
					position.x = EaseExpoOut(Wiwa::Time::GetDeltaTime(), starting_position.x, desired_position.x - starting_position.x, duration);
				}
				if (position.y < desired_position.y)
				{
					position.y = EaseExpoOut(Wiwa::Time::GetDeltaTime(), starting_position.y, desired_position.y - starting_position.y, duration);
				}
				break;
			case Wiwa::GuiControlEasing::ExpoInOut:
				if (position.x < desired_position.x)
				{
					position.x = EaseExpoInOut(Wiwa::Time::GetDeltaTime(), starting_position.x, desired_position.x - starting_position.x, duration);
				}
				if (position.y < desired_position.y)
				{
					position.y = EaseExpoInOut(Wiwa::Time::GetDeltaTime(), starting_position.y, desired_position.y - starting_position.y, duration);
				}
				break;
			case Wiwa::GuiControlEasing::EaseBackIn:
				if (position.x < desired_position.x)
				{
					position.x = EaseBackIn(Wiwa::Time::GetDeltaTime(), starting_position.x, desired_position.x - starting_position.x, duration);
				}
				if (position.y < desired_position.y)
				{
					position.y = EaseBackIn(Wiwa::Time::GetDeltaTime(), starting_position.y, desired_position.y - starting_position.y, duration);
				}
				break;
			case Wiwa::GuiControlEasing::EaseBackOut:
				if (position.x < desired_position.x)
				{
					position.x = EaseBackOut(Wiwa::Time::GetDeltaTime(), starting_position.x, desired_position.x - starting_position.x, duration);
				}
				if (position.y < desired_position.y)
				{
					position.y = EaseBackOut(Wiwa::Time::GetDeltaTime(), starting_position.y, desired_position.y - starting_position.y, duration);
				}
				break;
			case Wiwa::GuiControlEasing::EaseBackInOut:
				if (position.x < desired_position.x)
				{
					position.x = EaseBackInOut(Wiwa::Time::GetDeltaTime(), starting_position.x, desired_position.x - starting_position.x, duration);
				}
				if (position.y < desired_position.y)
				{
					position.y = EaseBackInOut(Wiwa::Time::GetDeltaTime(), starting_position.y, desired_position.y - starting_position.y, duration);
				}
				break;
			case Wiwa::GuiControlEasing::EaseBounceIn:
				if (position.x < desired_position.x)
				{
					position.x = EaseBounceIn(Wiwa::Time::GetDeltaTime(), starting_position.x, desired_position.x - starting_position.x, duration);
				}
				if (position.y < desired_position.y)
				{
					position.y = EaseBounceIn(Wiwa::Time::GetDeltaTime(), starting_position.y, desired_position.y - starting_position.y, duration);
				}
				break;
			case Wiwa::GuiControlEasing::EaseBounceOut:
				if (position.x < desired_position.x)
				{
					position.x = EaseBounceOut(Wiwa::Time::GetDeltaTime(), starting_position.x, desired_position.x - starting_position.x, duration);
				}
				if (position.y < desired_position.y)
				{
					position.y = EaseBounceOut(Wiwa::Time::GetDeltaTime(), starting_position.y, desired_position.y - starting_position.y, duration);
				}
				break;
			case Wiwa::GuiControlEasing::EaseBounceInOut:
				if (position.x < desired_position.x)
				{
					position.x = EaseBounceInOut(Wiwa::Time::GetDeltaTime(), starting_position.x, desired_position.x - starting_position.x, duration);
				}
				if (position.y < desired_position.y)
				{
					position.y = EaseBounceInOut(Wiwa::Time::GetDeltaTime(), starting_position.y, desired_position.y - starting_position.y, duration);
				}
				break;
			case Wiwa::GuiControlEasing::EaseElasticIn:
				if (position.x < desired_position.x)
				{
					position.x = EaseElasticIn(Wiwa::Time::GetDeltaTime(), starting_position.x, desired_position.x - starting_position.x, duration);
				}
				if (position.y < desired_position.y)
				{
					position.y = EaseElasticIn(Wiwa::Time::GetDeltaTime(), starting_position.y, desired_position.y - starting_position.y, duration);
				}
				break;
			case Wiwa::GuiControlEasing::EaseElasticOut:
				if (position.x < desired_position.x)
				{
					position.x = EaseElasticOut(Wiwa::Time::GetDeltaTime(), starting_position.x, desired_position.x - starting_position.x, duration);
				}
				if (position.y < desired_position.y)
				{
					position.y = EaseElasticOut(Wiwa::Time::GetDeltaTime(), starting_position.y, desired_position.y - starting_position.y, duration);
				}
				break;
			case Wiwa::GuiControlEasing::EaseElasticInOut:
				if (position.x < desired_position.x)
				{
					position.x = EaseElasticInOut(Wiwa::Time::GetDeltaTime(), starting_position.x, desired_position.x - starting_position.x, duration);
				}
				if (position.y < desired_position.y)
				{
					position.y = EaseElasticInOut(Wiwa::Time::GetDeltaTime(), starting_position.y, desired_position.y - starting_position.y, duration);
				}
				break;
			default:
				break;
			}
		}
		
		void ScaleGUIElement(Size2i desired_size, int duration, Size2i starting_size,GuiControlEasing easingType)
		{
			switch (easingType)
			{
			case Wiwa::GuiControlEasing::LinearIn:
				if (position.width < desired_size.w)
				{
					position.width = EaseLinearIn(Wiwa::Time::GetDeltaTime(), starting_size.w, desired_size.w - starting_size.w, duration);
				}
				if (position.height < desired_size.h)
				{
					position.height = EaseLinearIn(Wiwa::Time::GetDeltaTime(), starting_size.w, desired_size.w - starting_size.w, duration);
				}
				break;
			case Wiwa::GuiControlEasing::LinearOut:
				if (position.width < desired_size.w)
				{
					position.width = EaseLinearOut(Wiwa::Time::GetDeltaTime(), starting_size.w, desired_size.w - starting_size.w, duration);
				}
				if (position.height < desired_size.h)
				{
					position.height = EaseLinearOut(Wiwa::Time::GetDeltaTime(), starting_size.w, desired_size.w - starting_size.w, duration);
				}
				break;
			case Wiwa::GuiControlEasing::LinearInOut:
				if (position.width < desired_size.w)
				{
					position.width = EaseLinearInOut(Wiwa::Time::GetDeltaTime(), starting_size.w, desired_size.w - starting_size.w, duration);
				}
				if (position.height < desired_size.h)
				{
					position.height = EaseLinearInOut(Wiwa::Time::GetDeltaTime(), starting_size.w, desired_size.w - starting_size.w, duration);
				}
				break;
			case Wiwa::GuiControlEasing::SineIn:
				if (position.width < desired_size.w)
				{
					position.width = EaseSineIn(Wiwa::Time::GetDeltaTime(), starting_size.w, desired_size.w - starting_size.w, duration);
				}
				if (position.height < desired_size.h)
				{
					position.height = EaseSineIn(Wiwa::Time::GetDeltaTime(), starting_size.w, desired_size.w - starting_size.w, duration);
				}
				break;
			case Wiwa::GuiControlEasing::SineOut:
				if (position.width < desired_size.w)
				{
					position.width = EaseSineOut(Wiwa::Time::GetDeltaTime(), starting_size.w, desired_size.w - starting_size.w, duration);
				}
				if (position.height < desired_size.h)
				{
					position.height = EaseSineOut(Wiwa::Time::GetDeltaTime(), starting_size.w, desired_size.w - starting_size.w, duration);
				}
				break;
			case Wiwa::GuiControlEasing::SineInOut:
				if (position.width < desired_size.w)
				{
					position.width = EaseSineInOut(Wiwa::Time::GetDeltaTime(), starting_size.w, desired_size.w - starting_size.w, duration);
				}
				if (position.height < desired_size.h)
				{
					position.height = EaseSineInOut(Wiwa::Time::GetDeltaTime(), starting_size.w, desired_size.w - starting_size.w, duration);
				}
				break;
			case Wiwa::GuiControlEasing::CubicIn:
				if (position.width < desired_size.w)
				{
					position.width = EaseCubicIn(Wiwa::Time::GetDeltaTime(), starting_size.w, desired_size.w - starting_size.w, duration);
				}
				if (position.height < desired_size.h)
				{
					position.height = EaseCubicIn(Wiwa::Time::GetDeltaTime(), starting_size.w, desired_size.w - starting_size.w, duration);
				}
				break;
			case Wiwa::GuiControlEasing::CubicOut:
				if (position.width < desired_size.w)
				{
					position.width = EaseCubicOut(Wiwa::Time::GetDeltaTime(), starting_size.w, desired_size.w - starting_size.w, duration);
				}
				if (position.height < desired_size.h)
				{
					position.height = EaseCubicOut(Wiwa::Time::GetDeltaTime(), starting_size.w, desired_size.w - starting_size.w, duration);
				}
				break;
			case Wiwa::GuiControlEasing::CubicInOut:
				if (position.width < desired_size.w)
				{
					position.width = EaseCubicInOut(Wiwa::Time::GetDeltaTime(), starting_size.w, desired_size.w - starting_size.w, duration);
				}
				if (position.height < desired_size.h)
				{
					position.height = EaseCubicInOut(Wiwa::Time::GetDeltaTime(), starting_size.w, desired_size.w - starting_size.w, duration);
				}
				break;
			case Wiwa::GuiControlEasing::QuadIn:
				if (position.width < desired_size.w)
				{
					position.width = EaseQuadIn(Wiwa::Time::GetDeltaTime(), starting_size.w, desired_size.w - starting_size.w, duration);
				}
				if (position.height < desired_size.h)
				{
					position.height = EaseQuadIn(Wiwa::Time::GetDeltaTime(), starting_size.w, desired_size.w - starting_size.w, duration);
				}
				break;
			case Wiwa::GuiControlEasing::QuadOut:
				if (position.width < desired_size.w)
				{
					position.width = EaseQuadOut(Wiwa::Time::GetDeltaTime(), starting_size.w, desired_size.w - starting_size.w, duration);
				}
				if (position.height < desired_size.h)
				{
					position.height = EaseQuadOut(Wiwa::Time::GetDeltaTime(), starting_size.w, desired_size.w - starting_size.w, duration);
				}
				break;
			case Wiwa::GuiControlEasing::QuadInOut:
				if (position.width < desired_size.w)
				{
					position.width = EaseQuadInOut(Wiwa::Time::GetDeltaTime(), starting_size.w, desired_size.w - starting_size.w, duration);
				}
				if (position.height < desired_size.h)
				{
					position.height = EaseQuadInOut(Wiwa::Time::GetDeltaTime(), starting_size.w, desired_size.w - starting_size.w, duration);
				}
				break;
			case Wiwa::GuiControlEasing::ExpoIn:
				if (position.width < desired_size.w)
				{
					position.width = EaseExpoIn(Wiwa::Time::GetDeltaTime(), starting_size.w, desired_size.w - starting_size.w, duration);
				}
				if (position.height < desired_size.h)
				{
					position.height = EaseExpoIn(Wiwa::Time::GetDeltaTime(), starting_size.w, desired_size.w - starting_size.w, duration);
				}
				break;
			case Wiwa::GuiControlEasing::ExpoOut:
				if (position.width < desired_size.w)
				{
					position.width = EaseExpoOut(Wiwa::Time::GetDeltaTime(), starting_size.w, desired_size.w - starting_size.w, duration);
				}
				if (position.height < desired_size.h)
				{
					position.height = EaseExpoOut(Wiwa::Time::GetDeltaTime(), starting_size.w, desired_size.w - starting_size.w, duration);
				}
				break;
			case Wiwa::GuiControlEasing::ExpoInOut:
				if (position.width < desired_size.w)
				{
					position.width = EaseExpoInOut(Wiwa::Time::GetDeltaTime(), starting_size.w, desired_size.w - starting_size.w, duration);
				}
				if (position.height < desired_size.h)
				{
					position.height = EaseExpoInOut(Wiwa::Time::GetDeltaTime(), starting_size.w, desired_size.w - starting_size.w, duration);
				}
				break;
			case Wiwa::GuiControlEasing::EaseBackIn:
				if (position.width < desired_size.w)
				{
					position.width = EaseBackIn(Wiwa::Time::GetDeltaTime(), starting_size.w, desired_size.w - starting_size.w, duration);
				}
				if (position.height < desired_size.h)
				{
					position.height = EaseBackIn(Wiwa::Time::GetDeltaTime(), starting_size.w, desired_size.w - starting_size.w, duration);
				}
				break;
			case Wiwa::GuiControlEasing::EaseBackOut:
				if (position.width < desired_size.w)
				{
					position.width = EaseBackOut(Wiwa::Time::GetDeltaTime(), starting_size.w, desired_size.w - starting_size.w, duration);
				}
				if (position.height < desired_size.h)
				{
					position.height = EaseBackOut(Wiwa::Time::GetDeltaTime(), starting_size.w, desired_size.w - starting_size.w, duration);
				}
				break;
			case Wiwa::GuiControlEasing::EaseBackInOut:
				if (position.width < desired_size.w)
				{
					position.width = EaseBackInOut(Wiwa::Time::GetDeltaTime(), starting_size.w, desired_size.w - starting_size.w, duration);
				}
				if (position.height < desired_size.h)
				{
					position.height = EaseBackInOut(Wiwa::Time::GetDeltaTime(), starting_size.w, desired_size.w - starting_size.w, duration);
				}
				break;
			case Wiwa::GuiControlEasing::EaseBounceIn:
				if (position.width < desired_size.w)
				{
					position.width = EaseBounceIn(Wiwa::Time::GetDeltaTime(), starting_size.w, desired_size.w - starting_size.w, duration);
				}
				if (position.height < desired_size.h)
				{
					position.height = EaseBounceIn(Wiwa::Time::GetDeltaTime(), starting_size.w, desired_size.w - starting_size.w, duration);
				}
				break;
			case Wiwa::GuiControlEasing::EaseBounceOut:
				if (position.width < desired_size.w)
				{
					position.width = EaseBounceOut(Wiwa::Time::GetDeltaTime(), starting_size.w, desired_size.w - starting_size.w, duration);
				}
				if (position.height < desired_size.h)
				{
					position.height = EaseBounceOut(Wiwa::Time::GetDeltaTime(), starting_size.w, desired_size.w - starting_size.w, duration);
				}
				break;
			case Wiwa::GuiControlEasing::EaseBounceInOut:
				if (position.width < desired_size.w)
				{
					position.width = EaseBounceInOut(Wiwa::Time::GetDeltaTime(), starting_size.w, desired_size.w - starting_size.w, duration);
				}
				if (position.height < desired_size.h)
				{
					position.height = EaseBounceInOut(Wiwa::Time::GetDeltaTime(), starting_size.w, desired_size.w - starting_size.w, duration);
				}
				break;
			case Wiwa::GuiControlEasing::EaseElasticIn:
				if (position.width < desired_size.w)
				{
					position.width = EaseElasticIn(Wiwa::Time::GetDeltaTime(), starting_size.w, desired_size.w - starting_size.w, duration);
				}
				if (position.height < desired_size.h)
				{
					position.height = EaseElasticIn(Wiwa::Time::GetDeltaTime(), starting_size.w, desired_size.w - starting_size.w, duration);
				}
				break;
			case Wiwa::GuiControlEasing::EaseElasticOut:
				if (position.width < desired_size.w)
				{
					position.width = EaseElasticOut(Wiwa::Time::GetDeltaTime(), starting_size.w, desired_size.w - starting_size.w, duration);
				}
				if (position.height < desired_size.h)
				{
					position.height = EaseElasticOut(Wiwa::Time::GetDeltaTime(), starting_size.w, desired_size.w - starting_size.w, duration);
				}
				break;
			case Wiwa::GuiControlEasing::EaseElasticInOut:
				if (position.width < desired_size.w)
				{
					position.width = EaseElasticInOut(Wiwa::Time::GetDeltaTime(), starting_size.w, desired_size.w - starting_size.w, duration);
				}
				if (position.height < desired_size.h)
				{
					position.height = EaseElasticInOut(Wiwa::Time::GetDeltaTime(), starting_size.w, desired_size.w - starting_size.w, duration);
				}
				break;
			default:
				break;
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

		Renderer2D::InstanceData id_quad_normal;

		Renderer2D::InstanceData id_quad_extra;

		size_t callbackID;

		std::string name;
		Callback* callback;

		//THINGS FOR ANIMATIONS
		std::vector<Rect2i> positionsForAnimations;
		bool animatedControl = false; 
		size_t framesAnimation;
		float animSpeed;
		float timeForAnim = 0;

	};
}