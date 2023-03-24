#include <wipch.h>
#include <Wiwa/core/Renderer3D.h>
#include "ParticleEmitterExecutor.h"
#include "Wiwa/core/Resources.h"
#include "Wiwa/ecs/components/ParticleEmitter.h"
#include <Wiwa/ecs/EntityManager.h>
#include <Wiwa/utilities/render/Material.h>
#include <Wiwa/utilities/render/CameraManager.h>
#include <Wiwa/utilities/render/LightManager.h>
#include <Wiwa/utilities/math/Math.h>
#include <glew.h>

namespace Wiwa {
	ParticleEmitterExecutor::ParticleEmitterExecutor()
	{
	}

	ParticleEmitterExecutor::~ParticleEmitterExecutor()
	{

	}

	void ParticleEmitterExecutor::OnAwake()
	{
		m_emitterComponent = GetComponentIterator<Wiwa::ParticleEmitter>();

		if (m_emitterComponent.c_id != 18446744073709551615)
		{
			m_emitter = GetComponentByIterator<Wiwa::ParticleEmitter>(m_emitterComponent);

			if (m_emitter && !m_emitter->texturePath.empty())
			{
				m_emitter->textureId = Wiwa::Resources::Load<Wiwa::Image>(m_emitter->texturePath.c_str());
			}
		}
		
	}

	void ParticleEmitterExecutor::OnInit()
	{
		WI_CORE_INFO("Init");

		ParticleManager& pman = m_Scene->GetParticleManager();

		m_emitterComponent = GetComponentIterator<ParticleEmitter>();

		if (m_emitterComponent.c_id != 18446744073709551615)
		{
			Wiwa::ParticleEmitter* emitter = GetComponentByIterator<Wiwa::ParticleEmitter>(m_emitterComponent);


			if (emitter && !pman.ContainsEmitter(emitter))
			{
				pman.AddEmitter(emitter);

				emitter->entityOwnerId = m_EntityId;
				if (!emitter->texturePath.empty())
				{
					emitter->textureId = Wiwa::Resources::Load<Wiwa::Image>(emitter->texturePath.c_str());
				}
			}
		}
	}

	void ParticleEmitterExecutor::OnUpdate()
	{
		
	}

	void ParticleEmitterExecutor::OnDestroy()
	{
		ParticleManager& pman = m_Scene->GetParticleManager();

		m_emitterComponent = GetComponentIterator<ParticleEmitter>();
		Wiwa::ParticleEmitter* emitter = GetComponentByIterator<Wiwa::ParticleEmitter>(m_emitterComponent);

		if (emitter)
		{
			pman.RemoveEmitter(emitter);
		}
	}

	
	void ParticleEmitterExecutor::OnSystemAdded() // Called when system added to the editor
	{
		ParticleManager& pman = m_Scene->GetParticleManager();

		m_emitterComponent = GetComponentIterator<ParticleEmitter>();

		if (m_emitterComponent.c_id != 18446744073709551615)
		{
			Wiwa::ParticleEmitter* emitter = GetComponentByIterator<Wiwa::ParticleEmitter>(m_emitterComponent);


			if (emitter)
			{
				pman.AddEmitter(emitter);

				emitter->entityOwnerId = m_EntityId;
				if (!emitter->texturePath.empty())
				{
					emitter->textureId = Wiwa::Resources::Load<Wiwa::Image>(emitter->texturePath.c_str());
				}
			}

		}
		

	}
	
	void ParticleEmitterExecutor::OnSystemRemoved() // Called when system removed to the editor
	{

	}

	void ParticleEmitterExecutor::EmitBatch(EntityId eid)
	{
		ParticleManager& pman = m_Scene->GetParticleManager();
		pman.EmitBatch(eid);
	}

	void ParticleEmitterExecutor::Play(EntityId eid)
	{
		ParticleManager& pman = m_Scene->GetParticleManager();
		pman.Play(eid);
	}

	void ParticleEmitterExecutor::Stop(EntityId eid)
	{
		ParticleManager& pman = m_Scene->GetParticleManager();
		pman.EmitBatch(eid);
	}
	
}


