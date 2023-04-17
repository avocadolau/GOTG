#pragma once

#include "Panel.h"

#include "Wiwa/scene/Scene.h"
#include <Wiwa/Dialog/DialogManager.h>
#include <Wiwa/scene/SceneManager.h>
#include "../../Utils/EditorUtils.h"
#include "Wiwa/Events/ApplicationEvent.h"


class DialogPanel : public Panel
{
public:
	DialogPanel(EditorLayer* instance);
	virtual ~DialogPanel();

	void Draw() override;

	bool creatingNewDialog = false;
	int numNodesOnCurrentCreation;
	int currentCreationStep;

	int currentConversation = 0;
	int currentNode = 0;
};