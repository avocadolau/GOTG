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
	}
	void AnimatorSystem::OnInit()
	{

	}
	void AnimatorSystem::OnUpdate()
	{
		Wiwa::AnimatorComponent* anim = GetComponentByIterator<Wiwa::AnimatorComponent>(m_AnimatorComponent);


		//anim->animator->Update(Time::GetRealTimeSinceStartup());

		anim->animator->m_AnimationTime = anim->animationTime;

		if (!anim->Play) return;	

		if (!anim->Blend)
		{
				anim->animator->UpdateAnimation(Time::GetRealTimeSinceStartup());
		}
		else if(anim->animator->GetCurrentAnimation()&& anim->animator->GetTargetAnimation()){
			anim->animator->m_BlendWeight = anim->weight;
			anim->animator->BlendTwoAnimations(anim->animator->GetCurrentAnimation(),anim->animator->GetTargetAnimation(),anim->weight, Time::GetRealTimeSinceStartup());
		}		
	}
	void AnimatorSystem::OnDestroy()
	{
		Wiwa::AnimatorComponent* anim = GetComponentByIterator<Wiwa::AnimatorComponent>(m_AnimatorComponent);
		delete anim->animator;
	}
	void AnimatorSystem::OnEventFinish()
	{

	}
	void AnimatorSystem::OnSystemAdded()
	{
		m_AnimatorComponent = GetComponentIterator<AnimatorComponent>();

		if (m_AnimatorComponent.c_id == WI_INVALID_INDEX) return;

		Wiwa::AnimatorComponent* anim = GetComponentByIterator<Wiwa::AnimatorComponent>(m_AnimatorComponent);
		
		anim->animator = Animator::LoadWiAnimator(anim->filePath);
	}
	void AnimatorSystem::OnSystemRemoved()
	{
		Wiwa::AnimatorComponent* anim = GetComponentByIterator<Wiwa::AnimatorComponent>(m_AnimatorComponent);
		if (anim->animator != nullptr)
			delete anim->animator;
	}

	void AnimatorSystem::Pause()
	{
		m_AnimatorComponent = GetComponentIterator<AnimatorComponent>();
		Wiwa::AnimatorComponent* anim = GetComponentByIterator<Wiwa::AnimatorComponent>(m_AnimatorComponent);
		anim->animator->PauseAnimation();
	}
	void AnimatorSystem::Play()
	{
		m_AnimatorComponent = GetComponentIterator<AnimatorComponent>();
		Wiwa::AnimatorComponent* anim = GetComponentByIterator<Wiwa::AnimatorComponent>(m_AnimatorComponent);
		anim->animator->PlayAnimation();
	}
	void AnimatorSystem::PlayAnimation(std::string name)
	{
		m_AnimatorComponent = GetComponentIterator<AnimatorComponent>();
		Wiwa::AnimatorComponent* anim = GetComponentByIterator<Wiwa::AnimatorComponent>(m_AnimatorComponent);
		anim->animator->PlayAnimationName(name);
	}
	void AnimatorSystem::Blend(std::string targetAnim, float blendDuration)
	{
		m_AnimatorComponent = GetComponentIterator<AnimatorComponent>();
		Wiwa::AnimatorComponent* anim = GetComponentByIterator<Wiwa::AnimatorComponent>(m_AnimatorComponent);
		anim->animator->Blend(targetAnim, blendDuration);
	}
	void AnimatorSystem::SetAnimationSate(Wiwa::AnimationState state)
	{
		m_AnimatorComponent = GetComponentIterator<AnimatorComponent>();
		Wiwa::AnimatorComponent* anim = GetComponentByIterator<Wiwa::AnimatorComponent>(m_AnimatorComponent);
		anim->animator->SetAnimationSatate(state);
	}
	void AnimatorSystem::Restart()
	{
		m_AnimatorComponent = GetComponentIterator<AnimatorComponent>();
		Wiwa::AnimatorComponent* anim = GetComponentByIterator<Wiwa::AnimatorComponent>(m_AnimatorComponent);
		anim->animator->ResetTime();
	}
}