#pragma once

#include "Panel.h"
#include <glm/glm.hpp>

class EnemyPanel : public Panel
{
public:
	EnemyPanel(EditorLayer* instance);
	virtual ~EnemyPanel();

	void Init();
	void Draw() override;

	void DrawTable();

	/*void OnEvent(Wiwa::Event& e) override;
	bool OnSceneChange(Wiwa::SceneChangeEvent& e);*/
};