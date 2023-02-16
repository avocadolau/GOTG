#include <wipch.h>

//#include "UiManager.h"
//
//#include "UiButton.h"
//#include "UiCheckbox.h"
//#include "UiSlider.h"
//
//namespace Wiwa
//{
//	GuiManager::GuiManager(bool start_enabled)
//	{
//
//	}
//
//	GuiManager::~GuiManager() {}
//
//	bool GuiManager::Start()
//	{
//		return true;
//	}
//
//	GuiControl* GuiManager::CreateGuiControl(GuiControlType type, unsigned int id, const char* text, Rect2i bounds, Module* observer, Image* texture, Image* sliderTexture, Rect2i sliderBounds)
//	{
//		// L14: TODO1_D: Create a GUI control and add it to the list of controls
//
//		GuiControl* control = nullptr;
//
//		//Call the constructor according to the GuiControlType
//		switch (type)
//		{
//		case GuiControlType::BUTTON:
//			control = new GuiButton(id, bounds, text, texture);
//			break;
//
//		case GuiControlType::CHECKBOX:
//			control = new GuiCheckbox(id, bounds, text);
//			break;
//		case GuiControlType::SLIDER:
//			control = new GuiSlider(id, bounds, sliderBounds, texture, sliderTexture);
//			break;
//		default:
//			break;
//		}
//		//Set the observer
//		control->SetObserver(observer);
//
//		// Created GuiControls are added to the list of controls
//		if (control != nullptr) controls.add(control);
//		return control;
//	}
//
//	bool GuiManager::Update(float dt)
//	{
//		accumulatedTime += dt;
//		if (accumulatedTime >= updateMsCycle) doLogic = true;
//
//		UpdateAll(dt, doLogic);
//
//		if (doLogic == true)
//		{
//			accumulatedTime = 0.0f;
//			doLogic = false;
//		}
//
//		return true;
//	}
//
//	bool GuiManager::UpdateAll(float dt, bool doLogic) {
//
//		if (doLogic) {
//
//			ListItem<GuiControl*>* control = controls.start;
//
//			while (control != nullptr)
//			{
//				control->data->Update(dt);
//				control = control->next;
//			}
//
//		}
//		return true;
//
//	}
//
//	bool GuiManager::Draw()
//	{
//
//		ListItem<GuiControl*>* control = controls.start;
//
//		while (control != nullptr)
//		{
//			control->data->Draw(app->render);
//			control = control->next;
//		}
//
//		return true;
//
//	}
//	void GuiManager::DestroyGuiControl(int id)
//	{
//		ListItem<GuiControl*>* control = controls.start;
//		while (control != nullptr)
//		{
//			if (control->data->id == id)
//			{
//				controls.del(control);
//			}
//			control = control->next;
//		}
//
//	}
//	bool GuiManager::CleanUp()
//	{
//		ListItem<GuiControl*>* control = controls.start;
//
//		while (control != nullptr)
//		{
//			RELEASE(control);
//		}
//
//		return true;
//
//		return false;
//	}
//}
