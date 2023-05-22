#include "PopUpItem.h"
#include <Wiwa/game/GameStateManager.h>
#include <Wiwa/core/Input.h>
#include <Wiwa/Ui/UiManager.h>


namespace Wiwa
{

	Wiwa::PopUpItem::PopUpItem()
	{

	}

	PopUpItem::~PopUpItem()
	{
	}

	void PopUpItem::OnAwake()
	{
	}

	void PopUpItem::OnInit()
	{
		m_Animate = false;
		m_PopUp = false;
		m_PopOut = false;
		m_TimerIn = 0.0f;
		m_TimerAnim = 5.0f;
	}

	void PopUpItem::OnUpdate()
	{
		Wiwa::GuiManager& gm = m_Scene->GetGuiManager();
		if (m_Animate)
		{
			m_TimerIn += Time::GetDeltaTime();

			if (m_TimerIn >= m_TimerAnim)
			{
				m_PopOut = false;
			}
				

			if (m_PopUp)
			{
				gm.canvas.at(11)->controls.at(0)->MoveGUIElement({312.0f,855.0f}, m_TimerIn, {312.0f,2000.0f}, GuiControlEasing::EaseBounceIn);
				gm.canvas.at(11)->controls.at(1)->MoveGUIElement({600.0f,895.0f}, m_TimerIn, {600.0f,2000.0f}, GuiControlEasing::EaseBounceIn);
				gm.canvas.at(11)->controls.at(2)->MoveGUIElement({615.0f,1101.0f}, m_TimerIn, {615.0f,2000.0f}, GuiControlEasing::EaseBounceIn);
				m_PopUp = false;
			}
			if (m_PopOut)
			{
				gm.canvas.at(11)->controls.at(0)->MoveGUIElement({ 312.0f,2000.0f }, m_TimerIn, { 312.0f,855.0f }, GuiControlEasing::EaseBounceIn);
				gm.canvas.at(11)->controls.at(1)->MoveGUIElement({ 600.0f,2000.0f }, m_TimerIn, { 600.0f,895.0f }, GuiControlEasing::EaseBounceIn);
				gm.canvas.at(11)->controls.at(2)->MoveGUIElement({ 615.0f,2000.0f }, m_TimerIn, { 615.0f,1101.0f }, GuiControlEasing::EaseBounceIn);
			}
		}
	}

	void PopUpItem::OnDestroy()
	{
	}

	void PopUpItem::SetPopUpParameters()
	{
		Wiwa::GuiManager& gm = m_Scene->GetGuiManager();
		Wiwa::Renderer2D& r2d = Wiwa::Application::Get().GetRenderer2D();
		m_PopUp = true;
		m_PopOut = false;
		m_TimerIn = 0.0f;
		m_Animate = true;
		gm.canvas.at(11)->controls.at(2)->text = item.itemName;
		Text* newText = gm.InitFont("library/Fonts/Jade_Smile.ttf", item.itemName.c_str());
		r2d.UpdateInstancedQuadTexPriority(m_Scene, gm.canvas.at(11)->controls.at(2)->id_quad_normal, 1);
		r2d.UpdateInstancedQuadTexTexture(m_Scene, gm.canvas.at(11)->controls.at(2)->id_quad_normal, newText->GetTextureId());
		r2d.UpdateInstancedQuadTexClip(m_Scene, gm.canvas.at(11)->controls.at(2)->id_quad_normal, newText->GetSize(), { 0,0,512,512 });
	}

	void PopUpItem::OnCollisionEnter(Object* body1, Object* body2)
	{
		Wiwa::GuiManager& gm = m_Scene->GetGuiManager();
		Wiwa::EntityManager& em = m_Scene->GetEntityManager();
		std::string itemStr = "ITEM";
		if (body1->id == m_EntityId && itemStr == body2->selfTagStr)
		{
			gm.canvas.at(11)->SwapActive();
			Item* newItem = em.GetComponent<Item>(body2->id);
			item.itemName = newItem->Name;
			item.type = (Wiwa::ItemType)newItem->item_type;
			SetPopUpParameters();
		}
	}
}