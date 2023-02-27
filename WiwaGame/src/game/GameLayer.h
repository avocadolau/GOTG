#pragma once

#include <Wiwa/core/Layer.h>

class GameLayer : public Wiwa::Layer {
public:
	GameLayer();
	~GameLayer();

	void OnAttach() override;

	void OnDetach() override;

	void OnUpdate() override;

	void OnImGuiRender() override;

	void OnEvent(Wiwa::Event& e) override;
};