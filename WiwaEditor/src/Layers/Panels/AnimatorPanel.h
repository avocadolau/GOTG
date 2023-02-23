#pragma once

#include "Panel.h"

class AnimatorPanel : public Panel
{
public:
	AnimatorPanel(EditorLayer* instance);
	virtual ~AnimatorPanel();

	void Draw() override;
};