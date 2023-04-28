#pragma once
#include "wipch.h"
#include "UltronGUISystem.h"
#include <Wiwa/core/Core.h>
#include <Wiwa/ecs/Systems.h>
#include "UltronGUISystem.h"
void Wiwa::UltronGUISystem::OnUpdate()
{
	if (!bossIsDead)
	{
		Wiwa::GuiManager& gm = m_Scene->GetGuiManager();
		Character* character = Wiwa::GameStateManager::GetPlayerCharacterComp();
		Wiwa::Renderer2D& r2d = Wiwa::Application::Get().GetRenderer2D();
		if (introBossFight)
		{
			if (!activeIntroCanvas)
			{
				m_Scene->SwapPauseActive();
				gm.canvas.at(0)->SwapActive();
				gm.canvas.at(6)->SwapActive();
				activeIntroCanvas = true;
			}
			introBossFight = AnimationIntroBoss(gm);
		}
		if (!introBossFight)
		{
			if (!activeBossCanvas)
			{
				gm.canvas.at(6)->SwapActive();
				gm.canvas.at(5)->SwapActive();
				gm.canvas.at(0)->SwapActive();
				activeBossCanvas = true;
			}
			if (activeBossCanvas)
			{
				if (character->Health >= 0)
				{
					gm.canvas.at(5)->controls.at(1)->SetValueForUIbar(character->Health, character->MaxHealth);
				}
				else
				{
					gm.canvas.at(5)->SwapActive();
					activeBossCanvas = false;
					bossIsDead = true;
				}
			}
		}
	}
	
}

bool Wiwa::UltronGUISystem::AnimationIntroBoss(Wiwa::GuiManager& gm)
{
	counterIntroAnim += Wiwa::Time::GetDeltaTime();
	//gm.canvas.at(6)->controls.at(8)->ScaleGUIElement({ 1024.0f,1024.0f }, 10, { 640.0f,640.0f }, GuiControlEasing::SineInOut);

	if (counterIntroAnim/1000 <= timeIntroAnim) 
	{
		return true;
	}
	else
	{
		m_Scene->SwapPauseActive();
		return false;
	}
}

void Wiwa::UltronGUISystem::OnCollisionEnter(Object* body1, Object* body2)
{
}
