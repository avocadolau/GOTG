#include <wipch.h>

#include "UiManager.h"

#include "UiButton.h"
#include "UiCheckbox.h"
#include "UiSlider.h"
#include "UiText.h"
#include "UiImage.h"
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

	GuiControl* GuiManager::CreateGuiControl_Simple(GuiControlType type, unsigned int id, Rect2i bounds,const char* path,const char* extraPath)
	{
		
		GuiControl* control = nullptr;
		switch (type)
		{
		case GuiControlType::BUTTON:
			control = new GuiButton(id, bounds, path,extraPath);
			break;
		case GuiControlType::CHECKBOX:
			control = new GuiCheckbox(id, bounds, path,extraPath);
			break;
		case GuiControlType::IMAGE:
			control = new GuiImage(id, bounds, path);
			break;
		default:
			break;
		}
		if (control != nullptr) controls.push_back(control);
		return control;
	}

	GuiControl* GuiManager::CreateGuiControl(GuiControlType type, unsigned int id, Rect2i bounds, const char* path, const char* slider_path, Rect2i sliderBounds)
	{

		GuiControl* control = nullptr;


		switch (type)
		{
		case GuiControlType::BUTTON:
			control = new GuiButton(id, bounds, path, slider_path);
			break;
		case GuiControlType::SLIDER:
			control = new GuiSlider(id, bounds, sliderBounds, path, slider_path);
			break;
		case GuiControlType::CHECKBOX:
			control = new GuiCheckbox(id, bounds, path, slider_path);
			break;
		default:
			break;
		}
		if (control != nullptr) controls.push_back(control);
		return control;
	}

	GuiControl* GuiManager::CreateGuiControl_Text(GuiControlType type, unsigned int id, Rect2i bounds, const char* string_text)
	{
		GuiControl* control = nullptr;
		switch (type)
		{
		case GuiControlType::TEXT:
			control = new GuiText(id, bounds, string_text);
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
		
		std::vector<GuiControl*> control = controls;
		for (int i = 0; i < control.size(); i++)
		{
				control.at(i)->Update();
		}

		return true;
	
	}

	bool GuiManager::Draw()
	{
		std::vector<GuiControl*> control = controls;
		
		for (int i = 0; i < control.size(); i++)
		{
			control.at(i)->Draw(&Wiwa::Application::Get().GetRenderer2D());
		}
		return true;

	}
	void GuiManager::DestroyGuiControl(int id)
	{
		std::vector<GuiControl*> control = controls;

		for (int i = 0; i < control.size(); i++)
		{
			if (control.at(i) != nullptr)
			{
				if (control.at(i)->id == id)
				{
					//control.erase(i);
				}
			}
		}
	}
	bool GuiManager::CleanUp()
	{
		std::vector<GuiControl*> control = controls;

		for (int i = 0; i < control.size(); i++)
		{
			//control.erase(i);
		}

		return true;
	}
}
