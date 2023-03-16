using System;

namespace Wiwa
{
    using System.Reflection;
    using EntityId = System.UInt64;

    public class Behaviour
    {
        protected EntityId m_EntityId;
        private System.Int64 m_Scene;

        public ref T GetComponent<T>(EntityId entity) where T : unmanaged
        {
            return ref InternalCalls.GetComponent<T>(entity, typeof(T), m_Scene);
        }

        public ComponentIterator GetComponentIterator<T>() where T : unmanaged
        {
            return InternalCalls.GetComponentIterator(m_EntityId, typeof(T), m_Scene);
        }
        public ComponentIterator GetComponentIterator<T>(EntityId id) where T : unmanaged
        {
            return InternalCalls.GetComponentIterator(id, typeof(T), m_Scene);
        }
        public ref T GetComponentByIterator<T>(ComponentIterator iterator) where T : unmanaged
        {
            return ref InternalCalls.GetComponentByIterator<T>(iterator, m_Scene);
        }

        public ref T AddComponent<T>(EntityId entity) where T : unmanaged
        {
            return ref InternalCalls.AddComponent<T>(entity, typeof(T), m_Scene);
        }
        public void ApplySystem<T>(EntityId entity)
        {
            InternalCalls.ApplySystem(entity, typeof(T), m_Scene);
        }
        public void DestroyEntity(EntityId entity)
        {
            InternalCalls.DestroyEntity(entity, m_Scene);
        }

        public ref T GetComponent<T>() where T : unmanaged
        {
            return ref InternalCalls.GetComponent<T>(m_EntityId, typeof(T), m_Scene);
        }

        public ref T AddComponent<T>() where T : unmanaged
        {
            return ref InternalCalls.AddComponent<T>(m_EntityId, typeof(T), m_Scene);
        }

        public void ApplySystem<T>()
        {
            InternalCalls.ApplySystem(m_EntityId, typeof(T), m_Scene);
        }
        public void DestroyEntity()
        {
            InternalCalls.DestroyEntity(m_EntityId, m_Scene);
        }

        public EntityId CreateEntity()
        {
            EntityId id = InternalCalls.CreateEntity(m_Scene);
            AddComponent<Transform3D>(id);
            return id;
        }

        public EntityId CreateEntityNamed(string name_entity)
        {
            EntityId id = InternalCalls.CreateEntityNamed(name_entity, m_Scene);
            AddComponent<Transform3D>(id);
            return id;
        }
        public string GetEntityName(EntityId eid)
        {
            return InternalCalls.GetEntityName(eid, m_Scene);
        }
        public void AddMesh(EntityId eid, string model, string material)
        {
            InternalCalls.AddMeshToEntity(eid, model, material, m_Scene);
        }

        public void PlayMusic(string ev_name)
        {
            InternalCalls.PlayMusic(ev_name);
        }
        public void StopMusic(string ev_name)
        {
            InternalCalls.StopMusic(ev_name);
        }

        public void PlayAudioEvent()
        {
            InternalCalls.PlaySoundEntity(m_EntityId);
        }

        public bool IsPlaying()
        {
            return InternalCalls.IsPlaying(m_EntityId);
        }

        public bool IsPlaying(EntityId entity)
        {
            return InternalCalls.IsPlaying(entity);
        }

        public void PlayAudioEvent(string ev_name)
        {
            InternalCalls.PlaySound(ev_name, m_EntityId);
        }

        public void StopAudioEvent()
        {
            InternalCalls.StopSoundEntity(m_EntityId);
        }
        public void StopAudioEvent(string ev_name)
        {
            InternalCalls.StopSound(ev_name, m_EntityId);
        }

        public void StopAllAudio()
        {
            InternalCalls.StopAllEvents();
        }

        public void AddAudioSource(EntityId entity, string ev_name, bool playOnAwake, bool isDefaultListener)
        {
            InternalCalls.AddAudioSource(entity, ev_name, playOnAwake, isDefaultListener);
        }

        public void AddAudioSource(string ev_name, bool playOnAwake, bool isDefaultListener)
        {
            InternalCalls.AddAudioSource(m_EntityId, ev_name, playOnAwake, isDefaultListener);
        }
    }
}