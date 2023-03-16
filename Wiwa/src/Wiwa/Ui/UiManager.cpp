#include <wipch.h>

#include "UiManager.h"

#include "UiButton.h"
#include "UiCheckbox.h"
#include "UiSlider.h"
#include "UiText.h"
#include "UiImage.h"
#include <Wiwa/core/Application.h>

#include <Wiwa/scene/Scene.h>

namespace Wiwa
{
	GuiManager::GuiManager()
	{

	}

	GuiManager::~GuiManager() {}

	bool GuiManager::Init(Scene* scene)
	{
		m_Scene = scene;
		InitFont("assets/arial.ttf");
		return true;
	}

	GuiControl* GuiManager::CreateGuiControl_Simple(GuiControlType type, unsigned int id, Rect2i bounds,const char* path,const char* extraPath)
	{
		GuiControl* control = nullptr;
	
			switch (type)
			{
			case GuiControlType::BUTTON:
				control = new GuiButton(m_Scene, id, bounds, path, extraPath);
				break;
			case GuiControlType::CHECKBOX:
				control = new GuiCheckbox(m_Scene, id, bounds, path, extraPath);
				break;
			case GuiControlType::IMAGE:
				control = new GuiImage(m_Scene, id, bounds, path);
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
				control = new GuiButton(m_Scene, id, bounds, path, slider_path);
				break;
			case GuiControlType::SLIDER:
				control = new GuiSlider(m_Scene, id, bounds, sliderBounds, path, slider_path);
				break;
			case GuiControlType::CHECKBOX:
				control = new GuiCheckbox(m_Scene, id, bounds, path, slider_path);
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
				control = new GuiText(m_Scene, id, bounds, string_text);
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
			if (control.at(i)->active)
			{
				control.at(i)->Update();
			}
		}

		size_t rsize = controlsToDestroy.size();

		for (size_t i = 0; i < rsize; i++) {
			RemoveControl(controlsToDestroy[i]);
		}

		controlsToDestroy.clear();

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
	void GuiManager::DestroyGuiControl(GuiControl* control)
	{
		controlsToDestroy.push_back(control);
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

	void GuiManager::RemoveControl(GuiControl* control)
	{
		size_t ealive = controls.size();
		Wiwa::Renderer2D& r2d = Wiwa::Application::Get().GetRenderer2D();
		for (size_t i = 0; i < ealive; i++) {
			if (controls[i] == control) {
				r2d.RemoveInstance(m_Scene, controls.at(i)->id_quad_disabled);
				r2d.RemoveInstance(m_Scene, controls.at(i)->id_quad_normal);
				r2d.RemoveInstance(m_Scene, controls.at(i)->id_quad_focused);
				r2d.RemoveInstance(m_Scene, controls.at(i)->id_quad_pressed);
				r2d.RemoveInstance(m_Scene, controls.at(i)->id_quad_selected);

				if (controls.at(i)->type == GuiControlType::SLIDER)
				{
					r2d.RemoveInstance(m_Scene, controls.at(i)->id_quad_extra);
				}
				controls.erase(controls.begin() + i);
				break;
			}
		}
	}

	bool GuiManager::InitFont(const char* path)
	{
		
	
		return true;
	}

	
}
