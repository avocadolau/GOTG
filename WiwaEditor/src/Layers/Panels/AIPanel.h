#pragma once

#include "Panel.h"
#include <glm/glm.hpp>
#include <Wiwa/ecs/components/Mesh.h>


typedef size_t EntityId;

class AIPanel : public Panel
{
public:
	AIPanel(EditorLayer* instance);
	virtual ~AIPanel();

	void Draw() override;
	void HandleDragAndDrop();
	void HandleDebugSettings();
	void HandleRender();
	void HandleCleanUp();
	//void HandleCreationObj();
	//void HandleCreationRecast();

	//void HandleRenderRecast();
	void RefreshManager();
	
private:
	
	bool m_DebugDraw;
	EntityId m_Id = -1;
	//RecastCommon* m_RecastMesh;
	//InputGeom* m_Geom;
	//BuildContext ctx;
};