#include "wipch.h"
#include "AnimatorSystem.h"
#include "Wiwa/utilities/render/Animator.h"
#include "Wiwa/utilities/render/Model.h"
namespace Wiwa {
	AnimatorSystem::AnimatorSystem()
	{
	}
	AnimatorSystem::~AnimatorSystem()
	{
	}
	void AnimatorSystem::OnAwake()
	{
		animator = GetComponent<Animator>();
		Mesh* mesh = GetComponent<Mesh>();

	}
	void AnimatorSystem::OnInit()
	{
		//Preload Known Matrices
		//Animator->LoadMatrices
	}
	void AnimatorSystem::OnUpdate()
	{
		animator->UpdateAnimation(Time::GetRealTimeSinceStartup());
	}
	void AnimatorSystem::OnDestroy()
	{
	}
	void AnimatorSystem::OnEventFinish()
	{
	}
}