#pragma once

#include "Panel.h"

#include <Wiwa/events/ApplicationEvent.h>

struct buttonInfo {
	std::string name;
	int x;
	int y;
	// conections? maybe we save them in a map or smthing
	buttonInfo(std::string name)
	{
		this->name = name;
		x = 0;
		y = 0;
	}
};

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
	std::vector<buttonInfo> animButtons;

	int m_CurrentID = 0;
	bool m_EntitySet = false;

};