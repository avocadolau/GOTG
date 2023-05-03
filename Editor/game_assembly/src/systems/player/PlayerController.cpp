#include "PlayerController.h"

#include <Wiwa/game/GameStateManager.h>
#include <Wiwa/ecs/Components.h>

#include <Wiwa/utilities/math/Math.h>
#include <Wiwa/audio/Audio.h>
#include <Wiwa/ecs/EntityManager.h>

#include "../../Components.h"
#include <Wiwa/ecs/components/game/Character.h>
#include "../../Systems.h"

Wiwa::PlayerController::PlayerController()
{
}

Wiwa::PlayerController::~PlayerController()
{
}

void Wiwa::PlayerController::OnAwake()
{
	m_StatsIt.c_id = WI_INVALID_INDEX;
	m_TransformIt.c_id = WI_INVALID_INDEX;
	m_RigidbodyIt.c_id = WI_INVALID_INDEX;
	m_ShooterIt.c_id = WI_INVALID_INDEX;

	GameStateManager::SetPlayerId(m_EntityId, m_Scene);
	GameStateManager::LoadProgression();

	if (GameStateManager::GetType() == RoomType::ROOM_HUB)
	{
		Wiwa::GameStateManager::InitPlayerData();
		Wiwa::GameStateManager::SaveProgression();
	}
	m_Direction = 0.f;
	m_CurrentVelocity = glm::vec2(0.f);
}

void Wiwa::PlayerController::OnInit()
{
	if (!getAwake())
		OnAwake();


	m_StatsIt = GetComponentIterator<Character>();
	m_TransformIt = GetComponentIterator<Transform3D>();
	m_RigidbodyIt = GetComponentIterator<CollisionBody>();
	m_ShooterIt = GetComponentIterator<StarLordShooter>();
	m_RocketIt = GetComponentIterator<RocketShooter>();
	

	m_DashEnable = true;
	IsDashing = false;
	GameStateManager::s_PoolManager->LoadPool(Pool_Type::STARLORD_BULLET, m_Scene);
}

void Wiwa::PlayerController::OnDestroy()
{
	GameStateManager::s_PoolManager->UnloadPool(Pool_Type::STARLORD_BULLET);
}

void Wiwa::PlayerController::OnUpdate()
{
	if (!getInit())
		OnInit();

	m_MovementInput = GetMovementInput();
	m_ShootInput = GetShootingInput();

	if (m_MovementInput != glm::vec2(0.f))
		SetDirection(Math::AngleFromVec2(m_MovementInput));
	if (m_ShootInput != glm::vec2(0.f))
		SetDirection(Math::AngleFromVec2(m_ShootInput));

	if (!IsDashing)
		SetPlayerRotation(GetDirection());
}

Wiwa::StarLordShooter* Wiwa::PlayerController::GetStarLord()
{
	return GetComponent<StarLordShooter>();
}

Wiwa::RocketShooter* Wiwa::PlayerController::GetRocket()
{
	return GetComponent<RocketShooter>();
}

Wiwa::Character* Wiwa::PlayerController::GetCharacter()
{
	return GetComponent<Character>();
}

Wiwa::Transform3D* Wiwa::PlayerController::GetTransform()
{
	return System::GetTransform();
}

Wiwa::CollisionBody* Wiwa::PlayerController::GetRigidBody()
{
	return GetComponent<CollisionBody>();
}



Wiwa::AnimatorSystem* Wiwa::PlayerController::GetAnimator()
{
	return m_Scene->GetEntityManager().GetSystem<Wiwa::AnimatorSystem>(m_EntityId);
}

Wiwa::PhysicsSystem* Wiwa::PlayerController::GetPhysics()
{
	return m_Scene->GetEntityManager().GetSystem<Wiwa::PhysicsSystem>(m_EntityId);
}

Wiwa::Transform3D* Wiwa::PlayerController::GetFirePosition(const char* name)
{
	EntityId objectId = m_Scene->GetEntityManager().GetChildByName(m_EntityId, name);
	EntityManager::ComponentIterator it = GetComponentIterator<Transform3D>(objectId);
	return (Transform3D*)m_Scene->GetEntityManager().GetComponentByIterator(it);
}

Wiwa::AudioSystem* Wiwa::PlayerController::GetAudio()
{
	return m_Scene->GetEntityManager().GetSystem<Wiwa::AudioSystem>(m_EntityId);
}

void Wiwa::PlayerController::SpawnStarLordBullet(Transform3D& transform, const Character& character)
{
	if (GameStateManager::s_PoolManager->s_StarLordBullets->getCountDisabled() <= 0)
		return;

	EntityManager& entityManager = m_Scene->GetEntityManager();

	GameStateManager::s_PoolManager->SetScene(m_Scene);
	EntityId newBulletId = EntityManager::INVALID_INDEX;
	newBulletId = GameStateManager::s_PoolManager->s_StarLordBullets->GetFromPool();
	if (newBulletId == EntityManager::INVALID_INDEX)
		return;

	StarLordBulletSystem* bulletSys = entityManager.GetSystem<StarLordBulletSystem>(newBulletId);
	PhysicsSystem* physSys = entityManager.GetSystem<PhysicsSystem>(newBulletId);
	physSys->DeleteBody();

	// Set intial positions
	Transform3D* playerTr = GetTransform();
	Transform3D* bulletTr = (Transform3D*)entityManager.GetComponentByIterator(entityManager.GetComponentIterator<Transform3D>(newBulletId));

	if (!bulletTr || !playerTr)
		return;

	bulletTr->localPosition = Math::GetWorldPosition(transform.worldMatrix);
	bulletTr->localRotation = glm::vec3(0.f, playerTr->localRotation.y + 90.0f, 0.f);
	bulletTr->localScale = glm::vec3(0.1f, 0.1f, 0.1f);
	
	SimpleBullet* bullet = (SimpleBullet*)entityManager.GetComponentByIterator(entityManager.GetComponentIterator<SimpleBullet>(newBulletId));

	bullet->direction = Math::Vec3FromAngle(m_Direction);

	bullet->damage = this->GetCharacter()->Damage;
	
	WI_INFO("Direction {}", m_Direction);
	WI_INFO("Dir {}x {}y {}z", bullet->direction.x, bullet->direction.y, bullet->direction.z);

	physSys->CreateBody();
	bulletSys->EnableBullet();


	if (transform.localPosition.x > 0)
	{
		//emit left muzzle
		EntityId shotMuzzleLeft = entityManager.GetChildByName(m_EntityId, "p_muzzleLeft");
		EntityId shotMuzzleLeftImpact = entityManager.GetChildByName(shotMuzzleLeft, "vfx_impact");
		EntityId shotMuzzleLeftFlash = entityManager.GetChildByName(shotMuzzleLeft, "vfx_flash");

		entityManager.SetActive(shotMuzzleLeft, true);

		ParticleSystem* sys_shotMuzzleLeftImpact = entityManager.GetSystem<ParticleSystem>(shotMuzzleLeftImpact);
		ParticleSystem* sys_shotMuzzleLeftFlash = entityManager.GetSystem<ParticleSystem>(shotMuzzleLeftFlash);

		if (sys_shotMuzzleLeftImpact != nullptr && sys_shotMuzzleLeftFlash != nullptr)
		{
			sys_shotMuzzleLeftImpact->EmitParticleBatch(1);
			sys_shotMuzzleLeftFlash->EmitParticleBatch(1);
		}
	}
	else
	{
		//emit right muzzle
		EntityId shotMuzzleRight = entityManager.GetChildByName(m_EntityId, "p_muzzleRight");
		EntityId shotMuzzleRightImpact = entityManager.GetChildByName(shotMuzzleRight, "vfx_impact");
		EntityId shotMuzzleRightFlash = entityManager.GetChildByName(shotMuzzleRight, "vfx_flash");

		entityManager.SetActive(shotMuzzleRight, true);

		ParticleSystem* sys_shotMuzzleRightImpact = entityManager.GetSystem<ParticleSystem>(shotMuzzleRightImpact);
		ParticleSystem* sys_shotMuzzleRightFlash = entityManager.GetSystem<ParticleSystem>(shotMuzzleRightFlash);

		if (sys_shotMuzzleRightImpact != nullptr && sys_shotMuzzleRightFlash != nullptr)
		{
			sys_shotMuzzleRightImpact->EmitParticleBatch(1);
			sys_shotMuzzleRightFlash->EmitParticleBatch(1);
		}
	}
}

void Wiwa::PlayerController::SpawnRocketBullet(Transform3D& transform, const Character& character)
{
	if (GameStateManager::s_PoolManager->s_StarLordBullets->getCountDisabled() <= 0)
		return;

	EntityManager& entityManager = m_Scene->GetEntityManager();

	GameStateManager::s_PoolManager->SetScene(m_Scene);
	EntityId newBulletId = EntityManager::INVALID_INDEX;
	newBulletId = GameStateManager::s_PoolManager->s_StarLordBullets->GetFromPool();
	if (newBulletId == EntityManager::INVALID_INDEX)
		return;

	StarLordBulletSystem* bulletSys = entityManager.GetSystem<StarLordBulletSystem>(newBulletId);
	PhysicsSystem* physSys = entityManager.GetSystem<PhysicsSystem>(newBulletId);
	physSys->DeleteBody();

	// Set intial positions
	Transform3D* playerTr = GetTransform();
	Transform3D* bulletTr = (Transform3D*)entityManager.GetComponentByIterator(entityManager.GetComponentIterator<Transform3D>(newBulletId));

	if (!bulletTr || !playerTr)
		return;

	bulletTr->localPosition = Math::GetWorldPosition(transform.worldMatrix);
	bulletTr->localRotation = glm::vec3(0.f, playerTr->localRotation.y + 90.0f, 0.f);
	bulletTr->localScale = glm::vec3(0.1f, 0.1f, 0.1f);

	SimpleBullet* bullet = (SimpleBullet*)entityManager.GetComponentByIterator(entityManager.GetComponentIterator<SimpleBullet>(newBulletId));
	
	bullet->direction = Math::Vec3FromAngle(m_Direction);
	bullet->damage = this->GetCharacter()->Damage;

	physSys->CreateBody();
	bulletSys->EnableBullet();

	//emit left muzzle
	EntityId shotMuzzleLeft = entityManager.GetChildByName(m_EntityId, "p_muzzleLeft");
	EntityId shotMuzzleLeftImpact = entityManager.GetChildByName(shotMuzzleLeft, "vfx_impact");
	EntityId shotMuzzleLeftFlash = entityManager.GetChildByName(shotMuzzleLeft, "vfx_flash");

	entityManager.SetActive(shotMuzzleLeft, true);

	ParticleSystem* sys_shotMuzzleLeftImpact = entityManager.GetSystem<ParticleSystem>(shotMuzzleLeftImpact);
	ParticleSystem* sys_shotMuzzleLeftFlash = entityManager.GetSystem<ParticleSystem>(shotMuzzleLeftFlash);

	if (sys_shotMuzzleLeftImpact != nullptr && sys_shotMuzzleLeftFlash != nullptr)
	{
		sys_shotMuzzleLeftImpact->EmitParticleBatch(1);
		sys_shotMuzzleLeftFlash->EmitParticleBatch(1);
	}
}

glm::vec2 Wiwa::PlayerController::GetMovementInput()
{
	//     z
	//     |
	//     |
	//-----|----- -x
	//     |
	//     |

	glm::vec2 input = glm::vec3(0.f);

	if (Input::IsKeyPressed(Key::A))
		input.x += 1;
	if (Input::IsKeyPressed(Key::D))
		input.x -= 1;
	if (Input::IsKeyPressed(Key::W))
		input.y += 1;
	if (Input::IsKeyPressed(Key::S))
		input.y -= 1;

	input.x -= Input::GetRawAxis(Gamepad::GamePad1, Gamepad::LeftX, GameStateManager::GetControllerDeadZone());
	input.y -= Input::GetRawAxis(Gamepad::GamePad1, Gamepad::LeftY, GameStateManager::GetControllerDeadZone());

	return input;
}

glm::vec2 Wiwa::PlayerController::GetShootingInput()
{
	//     z
	//     |
	//     |
	//-----|----- -x
	//     |
	//     |
	glm::vec2 input = glm::vec3(0.f);

	if (Input::IsKeyPressed(Key::Left))
		input.x += 1;
	if (Input::IsKeyPressed(Key::Right))
		input.x -= 1;
	if (Input::IsKeyPressed(Key::Up))
		input.y += 1;
	if (Input::IsKeyPressed(Key::Down))
		input.y -= 1;

	input.x -= Input::GetRawAxis(Gamepad::GamePad1, Gamepad::RightX, GameStateManager::GetControllerDeadZone());
	input.y -= Input::GetRawAxis(Gamepad::GamePad1, Gamepad::RightY, GameStateManager::GetControllerDeadZone());

	return input;
}

void Wiwa::PlayerController::SetPlayerRotation(const float angle)
{
	Transform3D* transform = GetTransform();

	transform->localRotation.y = angle;
	WI_INFO("Angle {}", angle);
	if (transform->localRotation.y > 360.f)
		transform->localRotation.y = 0.f;
}
