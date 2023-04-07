#pragma once
#include "Panel.h"
class AchievementsPanel : public Panel
{
public:
	AchievementsPanel(EditorLayer* instance);
	virtual ~AchievementsPanel();

	void Draw() override;

	void OnEvent(Wiwa::Event& e) override;
};

