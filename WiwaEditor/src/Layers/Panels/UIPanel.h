#pragma once

#include "Panel.h"
#include "Wiwa/scene/Scene.h"
#include <Wiwa/Ui/UiManager.h>
#include <Wiwa/scene/SceneManager.h>
#
class UIPanel : public Panel
{
public:
	UIPanel(EditorLayer* instance);
	virtual ~UIPanel();

	void Draw() override;

	int UI_element_selected = -1;
	int buttonID = 0;
	int sliderID = 0;
	int checkboxID = 0;
	int imageID = 0;

	int lastButtonID = 0;
	int lastSliderID = 0;
	int lastCheckboxID = 0;
	int lastImageID = 0;
};