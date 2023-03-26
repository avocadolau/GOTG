#pragma once

#include <Wiwa/core/Application.h>
#include <Wiwa/ecs/EntityManager.h>
#include <Wiwa/scene/SceneManager.h>

namespace Wiwa
{
	struct Object;
	class WI_API System
	{
	private:
		bool m_Awaken;
		bool m_Inited;
	protected:
		EntityId m_EntityId;
		Scene *m_Scene;

		template <class T>
		T* GetComponent();

		template<class T>
		EntityManager::ComponentIterator GetComponentIterator();

		template<class T>
		EntityManager::ComponentIterator GetComponentIterator(size_t id);

		template<class T>
		T* GetComponentByIterator(const EntityManager::ComponentIterator& c_it);

		virtual void OnAwake() {}

		virtual void OnInit() {}

		virtual void OnUpdate() {}

		virtual void OnDestroy() {}

		virtual void OnEntitySet() {}

		virtual void OnSceneSet() {}

	public:
		System();
		virtual ~System(); // Virtual destructor, so that child destructor is called

		void SetEntity(EntityId entity)
		{
			m_EntityId = entity;
			OnEntitySet();
		}
		void SetScene(Scene *scene)
		{
			m_Scene = scene;
			OnSceneSet();
		}

		EntityId GetEntity() { return m_EntityId; }

		void Awake();
		void Init();
		void Update();

		virtual void OnSystemAdded() {}

		virtual void OnSystemRemoved() {}

		void Destroy();

		virtual void OnCollisionEnter(Object *body1, Object *body2){}

		virtual void OnCollision(Object *body1, Object *body2){}

		virtual void OnCollisionExit(Object *body1, Object *body2){}

		virtual bool OnComponentAdded(byte *data, const Type *type) { return true; }

		virtual bool OnComponentRemoved(byte *data, const Type *type) { return true; }
	};

	template <class T>
	inline T* System::GetComponent()
	{
		Wiwa::EntityManager &em = m_Scene->GetEntityManager();

		T *component = em.GetComponent<T>(m_EntityId);

		return component;
	}

	template<class T>
	inline EntityManager::ComponentIterator System::GetComponentIterator()
	{
		Wiwa::EntityManager& em = m_Scene->GetEntityManager();

		return em.GetComponentIterator<T>(m_EntityId);
	}

	template<class T>
	inline EntityManager::ComponentIterator System::GetComponentIterator(size_t id)
	{
		Wiwa::EntityManager& em = m_Scene->GetEntityManager();

		return em.GetComponentIterator<T>(id);
	}

	template<class T>
	inline T* System::GetComponentByIterator(const EntityManager::ComponentIterator& c_it) {
		Wiwa::EntityManager& em = m_Scene->GetEntityManager();

		T* component = (T*)em.GetComponentByIterator(c_it);

		return component;
	}

}