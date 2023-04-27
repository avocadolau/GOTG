#include "ship_main_menu.h"
#include "../../components/ship/ship_main_menu_data.h"
#include <Wiwa/utilities/easings.h>


Wiwa::ShipMainMenu::ShipMainMenu()
{
}

Wiwa::ShipMainMenu::~ShipMainMenu()
{
}

void Wiwa::ShipMainMenu::OnAwake()
{
	m_Time = 0.f;
	m_StartPos = GetTransform()->position;
	m_ShipDataIt = GetComponentIterator<ShipMainMenuData>();
}

void Wiwa::ShipMainMenu::OnInit()
{

}

void Wiwa::ShipMainMenu::OnUpdate()
{
	ShipMainMenuData* data = GetComponentByIterator<ShipMainMenuData>(m_ShipDataIt);
	m_Time += Time::GetDeltaTimeSeconds();
	float posY = GetTransform()->position.y;
	float move = sin(m_Time) * data->VelocityMove * Time::GetDeltaTimeSeconds();
	float rot = cos(m_Time) * data->VelocityRot * Time::GetDeltaTimeSeconds();
	GetTransform()->localPosition.y += move;
	GetTransform()->localRotation.x += rot;

}