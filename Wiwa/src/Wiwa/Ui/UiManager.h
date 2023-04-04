#pragma once

#include "Wiwa/Ui/UiControl.h"
#include "Wiwa/Ui/UiCanvas.h"
#include <Wiwa/core/Core.h>
#include <Wiwa/core/Renderer2D.h>
#include <Wiwa/utilities/render/Text.h>
#include <vector>
#include <string>
#include <Wiwa/core/Input.h>



namespace Wiwa
{
	class WI_API GuiManager
	{
	private:
		Scene* m_Scene;
	public:

		// Constructor
		GuiManager();

		// Destructor
		virtual ~GuiManager();

	
		// Init gui manager
		bool Init(Scene* scene);

		// Update gui manager
		bool Update();

		bool Draw();

		// Called before quitting
		bool CleanUp();

		//Method for canvas
		GuiCanvas* CreateGuiCanvas(unsigned int id,bool active);
		//Method for sliders / UI_BARS
		GuiControl* CreateGuiControl(GuiControlType type,unsigned int id,Rect2i bounds,const char* path, const char* slider_path, Rect2i sliderBounds,unsigned int canvas_id, int callbackID, Rect2i boundsOriginTex, Rect2i sliderOriginTex, const char* audioEventName,bool active);
		//Method for buttons and checkboxes
		GuiControl* CreateGuiControl_Simple(GuiControlType type, unsigned int id, Rect2i bounds, const char* path, const char* extraPath, unsigned int canvas_id, int callbackID, Rect2i boundsOriginTex,const char* audioEventName, bool active, bool animated, float framesAnim, std::vector<Rect2i> animRects);
		//Method for strings
		GuiControl* CreateGuiControl_Text(GuiControlType type, unsigned int id, Rect2i bounds, const char* string_text, unsigned int canvas_id, bool active);
		//Method for abilities
		GuiControl* CreateGuiControl_Ability(GuiControlType type, unsigned int id,unsigned int canvas_id, Rect2i bounds, const char* path, size_t callbackID, Rect2i boundsOriginTex, bool active,bool animated, std::vector<Rect2i> animationRects);

		void SwapSelectedCanvas(GuiCanvas* canvasToSelect);
		void DestroyGuiControl(GuiControl* control, GuiCanvas* canvas);
		void DestroyCanvas(GuiCanvas* canvas);
		void RemoveCanvas(GuiCanvas* canvasToDestroy);
		void RemoveControl(GuiControl* control);

		Text* InitFont(const char* path,char* word);

		
		std::vector<GuiCanvas*>& ReturnCanvas() { return canvas; }

		bool _saveGUIImpl(File& file);
		bool _loadGUIImpl(File& file);

		void SaveWiUI(const char* path);

		void LoadWiUI(const char* path);

	public:
		std::vector<GuiCanvas*> canvas;
		std::vector<GuiCanvas*> canvasToDestroy;
		float accumulatedTime = 0.0f;
		float updateMsCycle = 0.0f;
		bool doLogic = false;

		Image* texture;

	};
}