#pragma once

#include "Panel.h"
#include <Wiwa/Ui/UiManager.h>
#include <Wiwa/scene/SceneManager.h>
#include "../../Utils/EditorUtils.h"

class UIEditorPanel : public Panel
{
public:
	UIEditorPanel(EditorLayer* instance);
	virtual ~UIEditorPanel();

	void Draw() override;

	int canvasSelectedID = -1;
	
	void GetSelectedCanvas();

	void DrawCanvasItems();

	void OpenEditGuiControl(Wiwa::GuiControl* control);

	void SetInitialValues(Wiwa::GuiControl* control);

	void UpdateElements(Wiwa::GuiControl* control);
	int elementSelected;

	//Variables to change
	float pos[2];
	float size[2];
	std::string pathForAsset;
	std::string pathForExtraAsset;

};