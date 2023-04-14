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
	void HandleCreationObj();
private:
	Wiwa::Mesh* m_Mesh;
	EntityId m_Id = -1;
};