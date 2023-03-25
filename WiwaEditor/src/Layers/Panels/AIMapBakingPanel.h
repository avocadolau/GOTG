#pragma once

#include "Panel.h"
#include <glm/glm.hpp>
class AIMapBakingPanel : public Panel
{
public:
	AIMapBakingPanel(EditorLayer* instance);
	virtual ~AIMapBakingPanel();

	void Draw() override;

	void RefreshData();
private:
	bool m_DrawGrid;
	glm::ivec2 m_MapSizeInTiles = { 10, 10 };
	glm::vec2 m_SizeOfTiles = { 2.0f, 2.0f };

};