#pragma once

#include "../Panel.h"

#include <string>

#include <Wiwa/animation/OzzAnimator.h>

#include <Wiwa/animation/animations/OzzAnimationSimple.h>

class OzzAnimationPanel : public Panel
{
private:
	std::string m_ActiveAnimatorPath;
	Wiwa::OzzAnimator* m_ActiveAnimator;

	void DrawTopbar();
	void DrawBody();

	void DrawMeshContainer();
	void DrawSkeletonContainer();
	void DrawAnimations();

	void DrawPartialBlendingAnimation(Wiwa::OzzAnimationPartialBlending* partial_animation);
	void DrawSimpleAnimation(Wiwa::OzzAnimationSimple* simple_animation);
public:
	OzzAnimationPanel(EditorLayer* instance);
	virtual ~OzzAnimationPanel();

	void Draw() override;
};