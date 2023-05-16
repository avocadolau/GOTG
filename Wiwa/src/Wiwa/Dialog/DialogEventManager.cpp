#include <wipch.h>

#include "DialogEventManager.h"
#include "DialogManager.h"

#include <Wiwa/ecs/components/game/Character.h>

#include "Wiwa/audio/Audio.h"

#include <Wiwa/core/Application.h>
//#include <Wiwa/scene/SceneManager.h>
#include <Wiwa/scene/Scene.h>

//#define STB_IMAGE_WRITE_IMPLEMENTATION
#include "../vendor/stb/stb_image_write.h"

//#define STB_TRUETYPE_IMPLEMENTATION 
#include "../vendor/stb/stb_truetype.h"

#include <Wiwa/scene/SceneManager.h>

#include <Wiwa/utilities/json/JSONDocument.h>

#include <Wiwa/utilities/easings.h>

namespace Wiwa
{
	DialogEventManager::DialogEventManager()
	{

	}

	DialogEventManager::~DialogEventManager()
	{

	}

	bool DialogEventManager::Init(Scene* scene)
	{
		m_Scene = scene;

		Renderer2D& render = Wiwa::Application::Get().GetRenderer2D();



		ResourceId textID = Wiwa::Resources::Load<Wiwa::Image>("assets/HUDImages/Menus/SpeechMenu/Choice_menu/aron_reward_menu_v1.png");
		Image* choice2Opt1Img = Wiwa::Resources::GetResourceById<Wiwa::Image>(textID);

		choice2Opt1ImgID = render.CreateInstancedQuadTex(m_Scene, choice2Opt1Img->GetTextureId(), choice2Opt1Img->GetSize(), { 300,250 }, { 400,400 }, Wiwa::Renderer2D::Pivot::UPLEFT);
		render.DisableInstance(m_Scene, choice2Opt1ImgID);

		ResourceId textID2 = Wiwa::Resources::Load<Wiwa::Image>("assets/HUDImages/Menus/SpeechMenu/Choice_menu/aron_reward_menu_v1.png");
		Image* choice2Opt2Img = Wiwa::Resources::GetResourceById<Wiwa::Image>(textID2);

		choice2Opt2ImgID = render.CreateInstancedQuadTex(m_Scene, choice2Opt2Img->GetTextureId(), choice2Opt2Img->GetSize(), { 800,250 }, { 400,400 }, Wiwa::Renderer2D::Pivot::UPLEFT);
		render.DisableInstance(m_Scene, choice2Opt2ImgID);

		ResourceId textID3 = Wiwa::Resources::Load<Wiwa::Image>("assets/HUDImages/Menus/SpeechMenu/Choice_menu/aron_reward_menu_v1.png");
		Image* choice2Opt3Img = Wiwa::Resources::GetResourceById<Wiwa::Image>(textID3);

		choice2Opt3ImgID = render.CreateInstancedQuadTex(m_Scene, choice2Opt3Img->GetTextureId(), choice2Opt3Img->GetSize(), { 1300,250 }, { 400,400 }, Wiwa::Renderer2D::Pivot::UPLEFT);
		render.DisableInstance(m_Scene, choice2Opt3ImgID);

		eventStarted = false;
		eventFinished = false;
		selector = 0;
		eventState = 2;
		keyPressTimer = 0;

		return true;
	}

	bool DialogEventManager::Update()
	{
		Renderer2D& render = Wiwa::Application::Get().GetRenderer2D();

		//DialogManager* dm = Wiwa::Application::Get();
		Wiwa::Scene * _scene = (Wiwa::Scene*)m_Scene;

		if (_scene->GetDialogManager().triggerEvent == true && eventState == 2)
		{
			eventState = 0;
		}

		if (eventState != 2)
		{
			if (eventState == 0)
			{
				eventStarted = true;

				eventState = 1;
			}

			if (!strcmp(_scene->GetDialogManager().dialogEventToTrigger.c_str(), "Choice_Uatu"))
			{
				
			}
			else if (!strcmp(_scene->GetDialogManager().dialogEventToTrigger.c_str(), "Choice_Aron"))
			{
				if (eventStarted == true)
				{
					render.EnableInstance(m_Scene, choice2Opt1ImgID);
					render.EnableInstance(m_Scene, choice2Opt2ImgID);
					render.EnableInstance(m_Scene, choice2Opt3ImgID);


					eventStarted = false;
				}

				if (selector == 0)
				{
					render.UpdateInstancedQuadTexClip(m_Scene, choice2Opt1ImgID, { 4500, 4500 }, { 1500, 0, 1500, 1500 });
					render.UpdateInstancedQuadTexClip(m_Scene, choice2Opt2ImgID, { 4500, 4500 }, { 0, 1500, 1500, 1500 });
					render.UpdateInstancedQuadTexClip(m_Scene, choice2Opt3ImgID, { 4500, 4500 }, { 0, 3000, 1500, 1500 });
				}
				else if (selector == 1)
				{
					render.UpdateInstancedQuadTexClip(m_Scene, choice2Opt1ImgID, { 4500, 4500 }, { 0, 0, 1500, 1500 });
					render.UpdateInstancedQuadTexClip(m_Scene, choice2Opt2ImgID, { 4500, 4500 }, { 1500, 1500, 1500, 1500 });
					render.UpdateInstancedQuadTexClip(m_Scene, choice2Opt3ImgID, { 4500, 4500 }, { 0, 3000, 1500, 1500 });
				}
				else if (selector == 2)
				{
					render.UpdateInstancedQuadTexClip(m_Scene, choice2Opt1ImgID, { 4500, 4500 }, { 0, 0, 1500, 1500 });
					render.UpdateInstancedQuadTexClip(m_Scene, choice2Opt2ImgID, { 4500, 4500 }, { 0, 1500, 1500, 1500 });
					render.UpdateInstancedQuadTexClip(m_Scene, choice2Opt3ImgID, { 4500, 4500 }, { 1500, 3000, 1500, 1500 });
				}

				if ((Wiwa::Input::IsKeyPressed(Wiwa::Key::Space) || Wiwa::Input::IsButtonPressed(Gamepad::GamePad1, Key::GamepadDPadLeft)) && keyPressTimer > 250) // Gamepad DPad reversed?
				{
					keyPressTimer = 0;

					selector++;

					if (selector >= 3) selector = 0;
				}

				if ((Wiwa::Input::IsButtonPressed(Gamepad::GamePad1, Key::GamepadDPadRight)) && keyPressTimer > 250) // Gamepad DPad reversed? | GamepadDPadRight does not work?
				{
					keyPressTimer = 0;

					selector--;

					if (selector < 0) selector = 2;
				}

				if ((Wiwa::Input::IsKeyPressed(Wiwa::Key::Enter) || Wiwa::Input::IsButtonPressed(0, 3)))
				{
					if (selector == 0)
					{
						
					}
					else if (selector == 1)
					{
						
					}
					else if (selector == 2)
					{
					
					}

					eventFinished = true;
				}

				if (eventFinished == true)
				{
					render.DisableInstance(m_Scene, choice2Opt1ImgID);
					render.DisableInstance(m_Scene, choice2Opt2ImgID);
					render.DisableInstance(m_Scene, choice2Opt3ImgID);

					eventState = 2;

					eventFinished = false;
				}
			}
			else if (!strcmp(_scene->GetDialogManager().dialogEventToTrigger.c_str(), "Choice_Ulana"))
			{

			}
			else
			{
				eventState = 2;
			}
		}

		if (eventState == 2)
		{
			_scene->GetDialogManager().triggerEvent = false;
		}

		keyPressTimer += Time::GetDeltaTime();

		return true;
	}

	bool DialogEventManager::Draw()
	{


		return false;
	}

	void DialogEventManager::OnDestroy()
	{

	}

	bool DialogEventManager::CleanUp()
	{


		return true;
	}
}