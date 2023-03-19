#pragma once

#include "Panel.h"

#include "Wiwa/scene/Scene.h"
#include <Wiwa/Ui/UiManager.h>
#include <Wiwa/Ui/UiCanvas.h>
#include <Wiwa/Ui/UiManager.h>
#include <Wiwa/scene/SceneManager.h>
#include "../../Utils/EditorUtils.h"
#include "Wiwa/Events/ApplicationEvent.h"

enum class GuiType
{
		BUTTON,
		TEXT,
		CHECKBOX,
		SLIDER,
		IMAGE
};
class UIPanel : public Panel
{
public:
	UIPanel(EditorLayer* instance);
	virtual ~UIPanel();

	void Draw() override;


	void OnEvent(Wiwa::Event& e) override;
	bool OnSceneChange(Wiwa::SceneChangeEvent& e);

	void DrawGuiElementSelection();

	void DrawGuiElementCreation(const char* current_item);

	GuiType GetSelectedElementType(const char* current_item);

	//SPECIFIC DRAWINGS FOR EACH TYPE
	void DrawButtonCreation(int canvas_id, Wiwa::GuiManager& m_GuiManager);
	void DrawCheckboxCreation(int canvas_id, Wiwa::GuiManager& m_GuiManager);
	void DrawSliderCreation(int canvas_id, Wiwa::GuiManager& m_GuiManager);
	void DrawImageCreation(int canvas_id, Wiwa::GuiManager& m_GuiManager);
	void DrawTextCreation(int canvas_id, Wiwa::GuiManager& m_GuiManager);

	GuiType type;

	int canvasSelected = -1;
	int elementSelected = -1;
	int position[2] = { 0,0 };
	int size[2] = { 0,0 };
	int originPos[2] = { 0,0 };
	int originSize[2] = { 0,0 };
	int sliderOriginPos[2] = { 0,0 };
	int sliderOriginSize[2] = { 0,0 };
	int callbackID;
	std::string pathForAsset;
	std::string pathForExtraAsset;
};