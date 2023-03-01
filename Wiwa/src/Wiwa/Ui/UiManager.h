#pragma once

#include "Wiwa/Ui/UiControl.h"
#include <Wiwa/core/Core.h>
#include <Wiwa/core/Renderer2D.h>
#include <Wiwa/scene/SceneManager.h>
#include <vector>
#include <string>

namespace Wiwa
{
	class WI_API GuiManager
	{
	public:

		// Constructor
		GuiManager();

		// Destructor
		virtual ~GuiManager();

	
		// Init audio engine
		static bool Init();

		// Update audio engine events
		bool Update();

		// Terminate audio engine
		static bool Terminate();

		bool Draw();

		// Called before quitting
		bool CleanUp();

		//Method for sliders
		GuiControl* CreateGuiControl(GuiControlType type,unsigned int id,Rect2i bounds,const char* path, const char* slider_path, Rect2i sliderBounds);
		//Method for buttons and checkboxes
		GuiControl* CreateGuiControl_Simple(GuiControlType type, unsigned int id, Rect2i bounds, const char* path, const char* extraPath);
		//Method for strings
		GuiControl* CreateGuiControl_Text(GuiControlType type, unsigned int id, Rect2i bounds, const char* string_text);

		void DestroyGuiControl(GuiControl* control);
		void RemoveControl(GuiControl* control);

		static bool InitFont(const char* path);

		std::vector<GuiControl*> ReturnControls() { return controls; }

		bool CheckId(int id);

	public:

		std::vector<GuiControl*> controls;
		std::vector<GuiControl*> controlsToDestroy;
		float accumulatedTime = 0.0f;
		float updateMsCycle = 0.0f;
		bool doLogic = false;

		Image* texture;


	};
}