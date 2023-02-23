#pragma once

#include "Panel.h"

#include <Wiwa/events/ApplicationEvent.h>

class AnimationPanel : public Panel
{
public:
	AnimationPanel(EditorLayer* instance);
	virtual ~AnimationPanel();


	bool OnEntityChange(EntityChangeEvent& e);
	bool OnSceneChange(Wiwa::SceneChangeEvent& e);
	void OnEvent(Wiwa::Event& e) override;

	void Draw() override;


private:

	int m_CurrentID = 0;
	bool m_EntitySet = false;
};