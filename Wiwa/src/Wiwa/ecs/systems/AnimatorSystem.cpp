#include "wipch.h"
#include "AnimatorSystem.h"

namespace Wiwa {
	AnimatorSystem::AnimatorSystem()
	{
		m_AnimatorComponent = { WI_INVALID_INDEX, WI_INVALID_INDEX };
	}
	AnimatorSystem::~AnimatorSystem()
	{
	}
	void AnimatorSystem::OnAwake()
	{
		m_AnimatorComponent = GetComponentIterator<AnimatorComponent>();
	}
	void AnimatorSystem::OnInit()
	{
		//Preload Known Matrices
		//Animator->LoadMatrices
	}
	void AnimatorSystem::OnUpdate()
	{
		Wiwa::AnimatorComponent* anim = GetComponentByIterator<Wiwa::AnimatorComponent>(m_AnimatorComponent);
		if(anim->Play)
			anim->animator->UpdateAnimation(Time::GetRealTimeSinceStartup());
	}
	void AnimatorSystem::OnDestroy()
	{
	}
	void AnimatorSystem::OnEventFinish()
	{
	}
	void AnimatorSystem::OnSystemAdded()
	{
	}
	void AnimatorSystem::OnSystemRemoved()
	{
	}
}