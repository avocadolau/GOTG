#pragma once

#include "Panel.h"

#include <Wiwa/events/ApplicationEvent.h>

class AnimatorPanel : public Panel
{
public:
	AnimatorPanel(EditorLayer* instance);
	virtual ~AnimatorPanel();

	void Draw() override;


	void OnEvent(Wiwa::Event&) override;
	bool OnEntityChangeEvent(EntityChangeEvent& e);
	bool OnSceneChangeEvent(Wiwa::SceneChangeEvent& e);

private:

	int m_CurrentID = 0;
	bool m_EntitySet = false;

};