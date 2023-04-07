#pragma once
#include "Panel.h"
#include <map>
#include <string>

#include <Wiwa/game/Achievements/Property.h>
#include <Wiwa/game/Achievements/Achievement.h>
class AchievementsPanel : public Panel
{
public:
	AchievementsPanel(EditorLayer* instance);
	virtual ~AchievementsPanel();

	void Draw() override;

	void OnEvent(Wiwa::Event& e) override;
	void ShowPropertiesTable(const std::map < std::string, Wiwa::Property > & properties);
};

