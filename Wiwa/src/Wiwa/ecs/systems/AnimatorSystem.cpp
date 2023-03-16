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
		//AnimatorComponent* animatorComponent = GetComponentByIterator<AnimatorComponent>(AnimatorComponent);
		//animator->PlayAnimationIndex()

	}
	void AnimatorSystem::OnDestroy()
	{
		delete bone;
	}
	void AnimatorSystem::OnEventFinish()
	{
	}
}