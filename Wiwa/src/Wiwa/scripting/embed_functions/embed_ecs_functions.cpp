#include "wipch.h"
#include "embed_ecs_functions.h"

#include <mono/metadata/object.h>
#include <mono/metadata/class.h>
#include <mono/metadata/reflection.h>
#include <mono/metadata/metadata.h>

#include <Wiwa/utilities/Reflection.h>
#include <Wiwa/utilities/Log.h>
#include <Wiwa/scene/SceneManager.h>
#include <Wiwa/core/Resources.h>
#include <Wiwa/core/Application.h>

#include <Wiwa/scripting/ScriptEngine.h>

#include <Wiwa/ecs/components/Mesh.h>

#include "../MonoWiwaTranslations.h"

byte* GetComponent(size_t id, MonoReflectionType* type, void* scene)
{
	Wiwa::Scene* _scene = (Wiwa::Scene*)scene;
	// Take monotype
	MonoType* compType = mono_reflection_type_get_type(type);

	// Take type name
	std::string typeName = mono_type_get_name(compType);

	// Clear type's name
	ClearName(typeName);

	// Generate hash of type
	size_t typeHash = FNV1A_HASH(typeName.c_str());

	Wiwa::Application& app = Wiwa::Application::Get();
	
	const Type* t = app.GetComponentTypeH(typeHash);

	// If reflected type is not a component, return NULL
	if (!t) return nullptr;

	Wiwa::EntityManager& em = _scene->GetEntityManager();

	ComponentId compID = em.GetComponentId(t);

	byte* comp = em.GetComponent(id, compID, t->size);

	return comp;
}

Wiwa::EntityManager::ComponentIterator GetComponentIterator(EntityId eid, MonoReflectionType* type, void* scene)
{
	Wiwa::Scene* _scene = (Wiwa::Scene*)scene;

	Wiwa::EntityManager::ComponentIterator c_it = { WI_INVALID_INDEX, WI_INVALID_INDEX };

	// Take monotype
	MonoType* compType = mono_reflection_type_get_type(type);

	// Take type name
	std::string typeName = mono_type_get_name(compType);

	// Clear type's name
	ClearName(typeName);

	// Generate hash of type
	size_t typeHash = FNV1A_HASH(typeName.c_str());

	Wiwa::Application& app = Wiwa::Application::Get();

	const Type* t = app.GetComponentTypeH(typeHash);

	// If reflected type is not a component, return invalid iterator
	if (!t) return c_it;

	Wiwa::EntityManager& em = _scene->GetEntityManager();

	c_it = em.GetComponentIterator(eid, typeHash);

	return c_it;
}

byte* GetComponentByIterator(Wiwa::EntityManager::ComponentIterator iterator, void* scene)
{
	Wiwa::Scene* _scene = (Wiwa::Scene*)scene;

	Wiwa::EntityManager& em = _scene->GetEntityManager();

	return em.GetComponentByIterator(iterator);
}

size_t CreateEntity(void* scene)
{
	Wiwa::Scene* _scene = (Wiwa::Scene*)scene;

	Wiwa::EntityManager& em = _scene->GetEntityManager();

	return em.CreateEntity();
}

size_t CreateEntityNamed(MonoString* name_entity, void* scene)
{
	Wiwa::Scene* _scene = (Wiwa::Scene*)scene;
	Wiwa::EntityManager& em = _scene->GetEntityManager();
	char* name_p = mono_string_to_utf8(name_entity);
	return em.CreateEntity(name_p);
}

MonoString* GetEntityName(size_t id, void* scene)
{
	Wiwa::Scene* _scene = (Wiwa::Scene*)scene;
	return 	Wiwa::ScriptEngine::CreateString(_scene->GetEntityManager().GetEntityName(id));
}

size_t GetEntityByName(MonoString* name_entity, void* scene)
{
	Wiwa::Scene* _scene = (Wiwa::Scene*)scene;
	Wiwa::EntityManager& em = _scene->GetEntityManager();
	const char* name = mono_string_to_utf8(name_entity);

	std::vector<EntityId>* ent = em.GetEntitiesAlive();
	size_t count = em.GetEntityCount();
	for (int i = 0; i < count; i++)
	{
		size_t id = ent->at(i);
		if (strcmp(name,em.GetEntityName(id)))
			return id;
	}
	return -1;
}

void DestroyEntity(size_t eid, void* scene)
{
	Wiwa::Scene* _scene = (Wiwa::Scene*)scene;
	Wiwa::EntityManager& em = _scene->GetEntityManager();

	em.DestroyEntity(eid);
}

size_t LoadPrefabIntr(const char* file)
{
	Wiwa::EntityManager& em = Wiwa::SceneManager::getActiveScene()->GetEntityManager();
	return em.LoadPrefab(file);
}

void SavePrefabIntr(size_t id, const char* file)
{
	Wiwa::EntityManager& em = Wiwa::SceneManager::getActiveScene()->GetEntityManager();
	em.SavePrefab(id, file);
}

size_t LoadResourceModel(MonoString* str)
{
	char* model = mono_string_to_utf8(str);

	return Wiwa::Resources::Load<Wiwa::Model>(model);
}

void AddMeshToEntity(size_t eid, MonoString* model, MonoString* mat, void* scene)
{
	Wiwa::Scene* _scene = (Wiwa::Scene*)scene;

	char* model_p = mono_string_to_utf8(model);
	char* mat_p = mono_string_to_utf8(mat);

	Wiwa::Mesh mesh;
	mesh.drawChildren = false;
	mesh.modelIndex = 0;
	sprintf_s(mesh.mesh_path, "%s", model_p);
	sprintf_s(mesh.mat_path, "%s", mat_p);

	mesh.meshId = Wiwa::Resources::Load<Wiwa::Model>(mesh.mesh_path);
	mesh.materialId = Wiwa::Resources::Load<Wiwa::Material>(mesh.mat_path);

	Wiwa::EntityManager& em = _scene->GetEntityManager();

	em.AddComponent<Wiwa::Mesh>(eid, mesh);
}

byte* AddComponent(size_t id, MonoReflectionType* type, void* scene)
{
	Wiwa::Scene* _scene = (Wiwa::Scene*)scene;

	static std::unordered_map<size_t, Type*> s_ConvertedTypes;

	MonoType* compType = mono_reflection_type_get_type(type);
	std::string typeName = mono_type_get_name(compType);
	ClearName(typeName);
	size_t typeHash = FNV1A_HASH(typeName.c_str());

	std::unordered_map<size_t, Type*>::iterator converted_type = s_ConvertedTypes.find(typeHash);

	Type* t = NULL;

	if (converted_type == s_ConvertedTypes.end())
	{
		t = ConvertType(compType);

		s_ConvertedTypes[typeHash] = t;
	}
	else
	{
		t = converted_type->second;
	}

	Wiwa::EntityManager& em = _scene->GetEntityManager();

	ComponentId compID = em.GetComponentId(t);

	byte* comp = em.AddComponent(id, t, NULL);

	return comp;
}

void ApplySystem(size_t id, MonoReflectionType* type, void* scene)
{
	Wiwa::Scene* _scene = (Wiwa::Scene*)scene;

	MonoType* compType = mono_reflection_type_get_type(type);
	std::string typeName = mono_type_get_name(compType);
	ClearName(typeName);
	size_t typeHash = FNV1A_HASH(typeName.c_str());

	Wiwa::EntityManager& em = _scene->GetEntityManager();

	em.ApplySystem(id, typeHash);
}
