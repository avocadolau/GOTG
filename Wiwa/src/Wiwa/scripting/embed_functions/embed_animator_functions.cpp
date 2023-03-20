#include "wipch.h"
#include "embed_animator_functions.h"

#include <mono/metadata/object.h>
#include <Wiwa/ecs/systems/AnimatorSystem.h>

void PlayAnimationName(MonoString* name, size_t entity)
{
	Wiwa::EntityManager& em = Wiwa::SceneManager::getActiveScene()->GetEntityManager();

	Wiwa::AnimatorSystem* animator = em.GetSystem<Wiwa::AnimatorSystem>(entity);

	const char* ev_p = mono_string_to_utf8(name);

	animator->PlayAnimation(ev_p);
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
