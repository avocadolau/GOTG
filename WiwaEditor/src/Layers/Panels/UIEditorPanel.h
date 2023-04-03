#pragma once

#include "Panel.h"
#include <Wiwa/Ui/UiManager.h>
#include <Wiwa/scene/SceneManager.h>
#include "../../Utils/EditorUtils.h"
#include <Wiwa/Events/ApplicationEvent.h>


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


	void OnEvent(Wiwa::Event& e) override;
	bool OnSceneChange(Wiwa::SceneChangeEvent& e);
	void VectorEdit(std::vector<Wiwa::Rect2i> list);

	int elementSelected;

	//Variables to change
	int pos[2];
	int size[2];
	int originPos[2];
	int originSize[2];
	int extraOriginPos[2];
	int extraOriginSize[2];
	size_t callbackID;
	std::string pathForAsset;
	std::string pathForExtraAsset;
	std::string audioEventForButton;

	bool animated = false;
	float animSpeed = 0;
	std::vector<Wiwa::Rect2i> animationRects;

};