#pragma once

#include "Panel.h"
#include <glm/glm.hpp>
class AIMapBakingPanel : public Panel
{
public:
	AIMapBakingPanel(EditorLayer* instance);
	virtual ~AIMapBakingPanel();

	void Draw() override;

private:
	bool m_drawGrid;
	glm::ivec2 mapSizeInTiles = { 10, 10 };
	glm::vec2 sizeOfTiles = { 2.0f, 2.0f };

};