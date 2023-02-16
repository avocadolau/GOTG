#include <wipch.h>

#include "UiManager.h"

#include "UiButton.h"
#include "UiCheckbox.h"
#include "UiSlider.h"
#include "UiText.h"
#include <Wiwa/core/Application.h>

namespace Wiwa
{
	GuiManager::GuiManager()
	{

	}

	GuiManager::~GuiManager() {}

	bool GuiManager::Init()
	{
		return true;
	}

	GuiControl* GuiManager::CreateGuiControl(GuiControlType type, unsigned int id, Rect2i bounds,Image* texture, Image* sliderTexture, Rect2i sliderBounds)
	{
		
		GuiControl* control = nullptr;

	
		switch (type)
		{
		case GuiControlType::BUTTON:
			control = new GuiButton(id, bounds, texture);
			break;

		case GuiControlType::CHECKBOX:
			control = new GuiCheckbox(id, bounds);
			break;
		case GuiControlType::SLIDER:
			control = new GuiSlider(id, bounds, sliderBounds, texture, sliderTexture);
			break;
		default:
			break;
		}
		//Set the observer
		//control->SetObserver(observer);

		// Created GuiControls are added to the list of controls
		if (control != nullptr) controls.push_back(control);
		return control;
	}

	bool GuiManager::Update()
	{
		
		Wiwa::List<GuiControl*> control = controls;
		for (int i = 0; i < control.size(); i++)
		{
			control.at(i)->Update();
		}
	}

	bool GuiManager::Draw()
	{
		Wiwa::List<GuiControl*> control = controls;
		
		for (int i = 0; i < control.size(); i++)
		{
			control.at(i)->Draw(&Wiwa::Application::Get().GetRenderer2D());
		}
		return true;

	}
	void GuiManager::DestroyGuiControl(int id)
	{
		Wiwa::List<GuiControl*> control = controls;

		for (int i = 0; i < control.size(); i++)
		{
			if (control.at(i) != nullptr)
			{
				if (control.at(i)->id == id)
				{
					control.erase(i);
				}
			}
			control.at(i)->Draw(&Wiwa::Application::Get().GetRenderer2D());
		}
	}
	bool GuiManager::CleanUp()
	{
		/*Wiwa::List<GuiControl*> control = controls;

		while (control != nullptr)
		{
			RELEASE(control);
		}

		return true;

		return false;*/
	}
}
