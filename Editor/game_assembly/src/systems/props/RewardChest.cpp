#include "RewardChest.h"
void Wiwa::RewardChest::OnAwake()
{
	m_Face1 = m_Scene->GetEntityManager().GetChildByName(m_EntityId, "Face1");
	m_Face2 = m_Scene->GetEntityManager().GetChildByName(m_EntityId, "Face2");
	m_Face3 = m_Scene->GetEntityManager().GetChildByName(m_EntityId, "Face3");

	m_Face1It = GetComponentIterator<Transform3D>(m_Face1);
	m_Face2It = GetComponentIterator<Transform3D>(m_Face2);
	m_Face3It = GetComponentIterator<Transform3D>(m_Face3);
}
void Wiwa::RewardChest::OnInit()
{
	if (!getInit())
		OnInit();

	Transform3D* face1 = GetComponentByIterator<Transform3D>(m_Face1It);
	Transform3D* face2 = GetComponentByIterator<Transform3D>(m_Face2It);
	Transform3D* face3 = GetComponentByIterator<Transform3D>(m_Face3It);

	m_Face1Start = face1->localRotation.x;
	m_Face2Start = face2->localRotation.x;
	m_Face3Start = face3->localRotation.x;

}
void Wiwa::RewardChest::OnUpdate()
{
	if (!getAwake())
		OnAwake();

	if (!getInit())
		OnInit();

	Transform3D* face1 = GetComponentByIterator<Transform3D>(m_Face1It);
	Transform3D* face2 = GetComponentByIterator<Transform3D>(m_Face2It);
	Transform3D* face3 = GetComponentByIterator<Transform3D>(m_Face3It);

	if (face1->localRotation.x <= m_Face1Start + m_FinalPos)
	{
		face1->localRotation.x += m_Velocity * Time::GetDeltaTimeSeconds();
	}
}
