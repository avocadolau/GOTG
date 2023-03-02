#pragma once

#include <Wiwa/core/Layer.h>
#include <Wiwa/Events/Event.h>
#include <Wiwa/Events/ApplicationEvent.h>

class GameLayer : public Wiwa::Layer {
public:
	GameLayer();
	~GameLayer();

	void OnAttach() override;

	void OnDetach() override;

	void OnUpdate() override;

	void OnImGuiRender() override;

	void OnEvent(Wiwa::Event& e) override;

	bool OnGameSave(Wiwa::OnSaveEvent& e);

	bool OnGameLoad(Wiwa::OnLoadEvent& e);
};