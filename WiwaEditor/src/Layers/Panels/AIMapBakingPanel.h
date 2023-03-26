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
	glm::vec2 m_StartingPosition = { 0.0f, 0.0f };

	const std::string m_NumLabel = "Num of tiles:";
	const std::string m_SizeLabel = "Size of the tiles:";
	const std::string m_PosLabel = "Starting pos offset:";

};