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
		m_AnimatorComponent = GetComponentIterator<AnimatorComponent>();

		if (m_AnimatorComponent.c_id == WI_INVALID_INDEX) return;

		Wiwa::AnimatorComponent* anim = GetComponentByIterator<Wiwa::AnimatorComponent>(m_AnimatorComponent);

		if (anim == nullptr && anim->animator != nullptr) return;
		
		//update animator
		anim->animator->Update(Time::GetRealDeltaTime());

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
		m_AnimatorComponent = GetComponentIterator<AnimatorComponent>();

		if (m_AnimatorComponent.c_id == WI_INVALID_INDEX) return;

		Wiwa::AnimatorComponent* anim = GetComponentByIterator<Wiwa::AnimatorComponent>(m_AnimatorComponent);
		
		if (!anim) return;
		
		if (anim->animator != nullptr)
			delete anim->animator;
	}

	void AnimatorSystem::Pause()
	{
		m_AnimatorComponent = GetComponentIterator<AnimatorComponent>();

		if (m_AnimatorComponent.c_id == WI_INVALID_INDEX) return;

		Wiwa::AnimatorComponent* anim = GetComponentByIterator<Wiwa::AnimatorComponent>(m_AnimatorComponent);
		
		if (!anim) return;
		
		anim->animator->PauseAnimation();
	}
	void AnimatorSystem::Play()
	{
		m_AnimatorComponent = GetComponentIterator<AnimatorComponent>();

		if (m_AnimatorComponent.c_id == WI_INVALID_INDEX) return;

		Wiwa::AnimatorComponent* anim = GetComponentByIterator<Wiwa::AnimatorComponent>(m_AnimatorComponent);
		
		if (!anim) return;
		
		anim->animator->PlayAnimation();
	}
	void AnimatorSystem::PlayAnimation(std::string name)
	{
		m_AnimatorComponent = GetComponentIterator<AnimatorComponent>();

		if (m_AnimatorComponent.c_id == WI_INVALID_INDEX) return;

		Wiwa::AnimatorComponent* anim = GetComponentByIterator<Wiwa::AnimatorComponent>(m_AnimatorComponent);
		
		if (!anim) return;
		
		anim->animator->PlayAnimationName(name);
	}
	void AnimatorSystem::Blend(std::string targetAnim, float blendDuration)
	{
		m_AnimatorComponent = GetComponentIterator<AnimatorComponent>();

		if (m_AnimatorComponent.c_id == WI_INVALID_INDEX) return;

		Wiwa::AnimatorComponent* anim = GetComponentByIterator<Wiwa::AnimatorComponent>(m_AnimatorComponent);
		
		if (!anim) return;
		
		anim->animator->Blend(targetAnim, blendDuration);
	}
	void AnimatorSystem::SetAnimationSate(Wiwa::AnimationState state)
	{
		m_AnimatorComponent = GetComponentIterator<AnimatorComponent>();

		if (m_AnimatorComponent.c_id == WI_INVALID_INDEX) return;

		Wiwa::AnimatorComponent* anim = GetComponentByIterator<Wiwa::AnimatorComponent>(m_AnimatorComponent);
		
		if (!anim) return;
		
		anim->animator->SetAnimationState(state);
	}
	void AnimatorSystem::Restart()
	{
		m_AnimatorComponent = GetComponentIterator<AnimatorComponent>();

		if (m_AnimatorComponent.c_id == WI_INVALID_INDEX) return;

		Wiwa::AnimatorComponent* anim = GetComponentByIterator<Wiwa::AnimatorComponent>(m_AnimatorComponent);
		
		if (!anim) return;
		
		anim->animator->ResetTime();
	}
}