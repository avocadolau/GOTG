#include "wipch.h"
#include "embed_animator_functions.h"

#include <mono/metadata/object.h>
#include <Wiwa/ecs/systems/AnimatorSystem.h>

void Blend(MonoString* targetAnim, bool loop,float blendTime, size_t entity)
{
	Wiwa::EntityManager& em = Wiwa::SceneManager::getActiveScene()->GetEntityManager();

	Wiwa::AnimatorSystem* animator = em.GetSystem<Wiwa::AnimatorSystem>(entity);

	const char* animName = mono_string_to_utf8(targetAnim);


	animator->Blend(animName, loop, blendTime);
}

void PlayAnimationName(MonoString* name, bool loop, size_t entity)
{
	Wiwa::EntityManager& em = Wiwa::SceneManager::getActiveScene()->GetEntityManager();

	Wiwa::AnimatorSystem* animator = em.GetSystem<Wiwa::AnimatorSystem>(entity);

	const char* animName = mono_string_to_utf8(name);

	animator->PlayAnimation(animName,loop);
}

void PlayAnimation(size_t entity)
{
	Wiwa::EntityManager& em = Wiwa::SceneManager::getActiveScene()->GetEntityManager();

	Wiwa::AnimatorSystem* animator = em.GetSystem<Wiwa::AnimatorSystem>(entity);

	animator->Play();
}

void PauseAnimation(size_t entity)
{
	Wiwa::EntityManager& em = Wiwa::SceneManager::getActiveScene()->GetEntityManager();

	Wiwa::AnimatorSystem* animator = em.GetSystem<Wiwa::AnimatorSystem>(entity);

	animator->Pause();
}

void RestartAnimation(size_t entity)
{
	Wiwa::EntityManager& em = Wiwa::SceneManager::getActiveScene()->GetEntityManager();

	Wiwa::AnimatorSystem* animator = em.GetSystem<Wiwa::AnimatorSystem>(entity);

	animator->Restart();
}

bool HasFinished(size_t entity)
{
	Wiwa::EntityManager& em = Wiwa::SceneManager::getActiveScene()->GetEntityManager();

	Wiwa::AnimatorSystem* animator = em.GetSystem<Wiwa::AnimatorSystem>(entity);

	return animator->HasFinished();
}
