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
		if(anim->Play)
			anim->animator->UpdateAnimation(Time::GetRealTimeSinceStartup());
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
		Wiwa::AnimatorComponent* anim = GetComponentByIterator<Wiwa::AnimatorComponent>(m_AnimatorComponent);
		anim->animator = Animator::LoadWiAnimator(anim->filePath);
	}
	void AnimatorSystem::OnSystemRemoved()
	{
		Wiwa::AnimatorComponent* anim = GetComponentByIterator<Wiwa::AnimatorComponent>(m_AnimatorComponent);
		if (anim->animator != nullptr)
			delete anim->animator;
	}
	void AnimatorSystem::PlayAnimation(std::string name)
	{
		Wiwa::AnimatorComponent* anim = GetComponentByIterator<Wiwa::AnimatorComponent>(m_AnimatorComponent);
		anim->animator->PlayAnimationName(name);
		anim->Play = true;
	}
	void AnimatorSystem::Restart()
	{
		Wiwa::AnimatorComponent* anim = GetComponentByIterator<Wiwa::AnimatorComponent>(m_AnimatorComponent);
		anim->animator->ResetTime();
	}
	void AnimatorSystem::Pause()
	{
		m_AnimatorComponent = GetComponentIterator<AnimatorComponent>();
		Wiwa::AnimatorComponent* anim = GetComponentByIterator<Wiwa::AnimatorComponent>(m_AnimatorComponent);
		anim->Play = false;
	}
	void AnimatorSystem::Play()
	{
		m_AnimatorComponent = GetComponentIterator<AnimatorComponent>();
		Wiwa::AnimatorComponent* anim = GetComponentByIterator<Wiwa::AnimatorComponent>(m_AnimatorComponent);
		anim->Play = true;
	}
	void AnimatorSystem::PlayAnimation(std::string name)
	{
		m_AnimatorComponent = GetComponentIterator<AnimatorComponent>();
		Wiwa::AnimatorComponent* anim = GetComponentByIterator<Wiwa::AnimatorComponent>(m_AnimatorComponent);
		anim->animator->PlayAnimationName(name);
		anim->Play = true;
	}
	void AnimatorSystem::Restart()
	{
		m_AnimatorComponent = GetComponentIterator<AnimatorComponent>();
		Wiwa::AnimatorComponent* anim = GetComponentByIterator<Wiwa::AnimatorComponent>(m_AnimatorComponent);
		anim->animator->ResetTime();
	}
}