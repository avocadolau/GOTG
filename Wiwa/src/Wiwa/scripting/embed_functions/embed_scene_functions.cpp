#include <wipch.h>
#include "embed_scene_functions.h"
#include <Wiwa/scene/SceneManager.h>

#include <mono/metadata/reflection.h>

void LoadSceneByIndex(size_t index, int flags)
{
	Wiwa::SceneManager::LoadSceneByIndex(index, flags);
}

void LoadSceneByName(MonoString* scene_name, int flags)
{
	char* scene = mono_string_to_utf8(scene_name);

	Wiwa::SceneManager::LoadSceneByName(scene, flags);
}
