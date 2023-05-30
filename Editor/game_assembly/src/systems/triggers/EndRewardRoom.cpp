#include "EndRewardRoom.h"
#include "../../components/trigger/RewardRoomData.h"

void Wiwa::EndRewardRoom::OnAwake()
{
	m_Activated = false;
}
void Wiwa::EndRewardRoom::OnInit()
{
}
void Wiwa::EndRewardRoom::OnUpdate()
{
	if (!m_Activated)
		return;

	if (Wiwa::Input::IsButtonPressed(Gamepad::GamePad1, Key::GamepadA)
		|| Wiwa::Input::IsKeyPressed(Key::Enter))
	{
		RewardRoomData* data = GetComponentByIterator<RewardRoomData>(m_DataIt);
		GameStateManager::setPlayerTriggerNextRoom(true);
		GameStateManager::s_NextRewardRoomReward = data->num;
	}
}

void Wiwa::EndRewardRoom::OnDestroy()
{
	GameStateManager::setPlayerTriggerNextRoom(false);
}

void Wiwa::EndRewardRoom::OnCollisionEnter(Object* body1, Object* body2)
{
	Wiwa::GuiManager& gm = m_Scene->GetGuiManager();
	std::string playerBulletStr = "PLAYER";
	if (body1->id == m_EntityId && playerBulletStr == body2->selfTagStr)
	{
		gm.canvas.at(9)->SwapActive();
		m_Activated = true;
	}
}

void Wiwa::EndRewardRoom::OnCollisionExit(Object* body1, Object* body2)
{
	Wiwa::GuiManager& gm = m_Scene->GetGuiManager();
	std::string playerBulletStr = "PLAYER";
	if (body1->id == m_EntityId && playerBulletStr == body2->selfTagStr)
	{
		gm.canvas.at(9)->SwapActive();
		m_Activated = false;
	}
	GameStateManager::setPlayerTriggerNextRoom(false);
}