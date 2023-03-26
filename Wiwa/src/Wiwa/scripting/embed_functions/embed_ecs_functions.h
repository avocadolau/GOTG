#pragma once
#include "../MonoDefinitions.h"
#include <Wiwa/ecs/EntityManager.h>
#include <Wiwa/scene/Scene.h>

typedef unsigned char byte;

byte* GetComponent(size_t id, MonoReflectionType* type, void* scene);

Wiwa::EntityManager::ComponentIterator GetComponentIterator(EntityId eid, MonoReflectionType* type, void* scene);

byte* GetComponentByIterator(Wiwa::EntityManager::ComponentIterator iterator, void* scene);

byte* AddComponent(size_t id, MonoReflectionType* type, void* scene);

void ApplySystem(size_t id, MonoReflectionType* type, void* scene);

size_t CreateEntity(void* scene);

size_t CreateEntityNamed(MonoString* name_entity, void* scene);

MonoString* GetEntityName(size_t id, void* scene);

EntityId GetEntityByName(MonoString* name_entity, void* scene);

EntityId GetChildByName(EntityId parent, MonoString* name, void* scene);

void DestroyEntity(size_t eid, void* scene);

size_t LoadPrefabIntr(MonoString* file);
void SavePrefabIntr(size_t id, MonoString* file);

// Help functions

size_t LoadResourceModel(MonoString* str);

void AddMeshToEntity(size_t eid, MonoString* model, MonoString* mat, void* scene);

/*MonoArray* GetComponent(EntityId id, MonoReflectionType* type)
	{
		static std::unordered_map<size_t, Type*> s_ConvertedTypes;

		MonoType* compType = mono_reflection_type_get_type(type);
		std::string typeName = mono_type_get_name(compType);
		ClearName(typeName);
		size_t typeHash = FNV1A_HASH(typeName.c_str());

		std::unordered_map<size_t, Type*>::iterator converted_type = s_ConvertedTypes.find(typeHash);

		Type* t = NULL;

		if (converted_type == s_ConvertedTypes.end()) {
			t = ConvertType(compType);

			s_ConvertedTypes[typeHash] = t;
		}
		else {
			t = converted_type->second;
		}

		int alingment;

		Wiwa::EntityManager& em = Wiwa::SceneManager::getActiveScene()->GetEntityManager();

		ComponentId compID = em.GetComponentId(t);
		byte* comp = em.GetComponent(id, compID, t->size);

		MonoArray* byteArray = NULL;

		if (comp) {
			SystemClass tmp("System", "Byte");

			byteArray = ScriptEngine::CreateArray(tmp.m_MonoClass, t->size);

			for (int i = 0; i < t->size; i++)
			{
				mono_array_set(byteArray, byte, i, comp[i]);
			}
		}

		return byteArray;
	}*/