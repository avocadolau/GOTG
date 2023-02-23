#pragma once

#include "Panel.h"

class AnimationPanel : public Panel
{
public:
	AnimationPanel(EditorLayer* instance);
	virtual ~AnimationPanel();

	void Draw() override;
};