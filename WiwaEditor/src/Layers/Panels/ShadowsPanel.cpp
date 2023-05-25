#include "ShadowsPanel.h"

#include <Wiwa/core/Application.h>
#include <Wiwa/core/Renderer3D.h>
#include <Wiwa/utilities/render/LightManager.h>

ShadowsPanel::ShadowsPanel(EditorLayer* instance)
	: Panel("Shadows panel", "", instance)
{
}

ShadowsPanel::~ShadowsPanel()
{
}

void ShadowsPanel::Draw()
{


	ImGui::Begin(iconName.c_str(), &active);
	float zoom = 1.f / m_ZoomLevel;
	ImGui::Image(
		(ImTextureID)(intptr_t)Wiwa::SceneManager::getActiveScene()->GetCameraManager().getActiveCamera()->shadowBuffer->GetShadowMap(),
		{
			(float)Wiwa::SceneManager::getActiveScene()->GetCameraManager().getActiveCamera()->shadowBuffer->GetWidth() / zoom,
			(float)Wiwa::SceneManager::getActiveScene()->GetCameraManager().getActiveCamera()->shadowBuffer->GetHeight() / zoom
		}
	);

	ImGui::SliderFloat("Zoom", &m_ZoomLevel, 0.5f, 1.f, "%.1f");

	ImGui::End();
}


void ShadowsPanel::Update()
{
	
}
