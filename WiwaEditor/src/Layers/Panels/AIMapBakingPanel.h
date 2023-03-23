#pragma once

#include "Panel.h"

class AIMapBakingPanel : public Panel
{
public:
	AIMapBakingPanel(EditorLayer* instance);
	virtual ~AIMapBakingPanel();

	void Draw() override;

private:
	bool m_drawGrid;
};