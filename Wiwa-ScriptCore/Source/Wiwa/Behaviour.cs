namespace Wiwa
{
    using System.Reflection;
    using EntityId = System.UInt64;

    public class Behaviour
    {
        protected EntityId m_EntityId;

        public ref T GetComponent<T>(EntityId entity) where T : unmanaged
        {
            return ref InternalCalls.GetComponent<T>(entity, typeof(T));
        }

        public ref T AddComponent<T>(EntityId entity) where T : unmanaged
        {
            return ref InternalCalls.AddComponent<T>(entity, typeof(T));
        }
        public void ApplySystem<T>(EntityId entity)
        {
            InternalCalls.ApplySystem(entity, typeof(T));
        }
        public void DestroyEntity(EntityId entity)
        {
            InternalCalls.DestroyEntity(entity);
        }

        public ref T GetComponent<T>() where T : unmanaged
        {
            return ref InternalCalls.GetComponent<T>(m_EntityId, typeof(T));
        }

        public ref T AddComponent<T>() where T : unmanaged
        {
            return ref InternalCalls.AddComponent<T>(m_EntityId, typeof(T));
        }

        public void ApplySystem<T>()
        {
            InternalCalls.ApplySystem(m_EntityId, typeof(T));
        }
        public void DestroyEntity()
        {
            InternalCalls.DestroyEntity(m_EntityId);
        }

        public EntityId CreateEntity()
        {
            EntityId id = InternalCalls.CreateEntity();
            AddComponent<Transform3D>(id);
            return id;
        }

        public EntityId CreateEntityNamed(string name_entity)
        {
            EntityId id = InternalCalls.CreateEntityNamed(name_entity);
            AddComponent<Transform3D>(id);
            return id;
        }

        public static void AddMesh(EntityId eid, string model, string material)
        {
            InternalCalls.AddMeshToEntity(eid, model, material);
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
    }
}