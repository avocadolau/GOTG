#pragma once

#include "Panel.h"
#include "Wiwa/scene/Scene.h"
#include <Wiwa/Ui/UiManager.h>
#include <Wiwa/scene/SceneManager.h>
#include "../../Utils/EditorUtils.h"

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

	int position[2] = {0,0};
	int size[2] = {0,0};
	std::string tex_path = "";
	std::string tex2_path = "";
};