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
}

void Wiwa::PlayerController::OnInit()
{
	m_StatsIt = GetComponentIterator<Character>();
	m_TransformIt = GetComponentIterator<Transform3D>();
	m_RigidbodyIt = GetComponentIterator<CollisionBody>();
	m_ShooterIt = GetComponentIterator<StarLordShooter>();
	m_RocketIt = GetComponentIterator<RocketShooter>();
	GameStateManager::LoadProgression();

	m_DashEnable = true;
}

void Wiwa::PlayerController::OnUpdate()
{
	m_MovementInput = GetMovementInput();
	m_ShootInput = GetShootingInput();
}

Wiwa::StarLordShooter* Wiwa::PlayerController::GetStarLord()
{
	return (StarLordShooter*)m_Scene->GetEntityManager().GetComponentByIterator(m_ShooterIt);
}

Wiwa::RocketShooter* Wiwa::PlayerController::GetRocket()
{
	return (RocketShooter*)m_Scene->GetEntityManager().GetComponentByIterator(m_StatsIt);
}

Wiwa::Character* Wiwa::PlayerController::GetCharacter()
{
	return (Character*)m_Scene->GetEntityManager().GetComponentByIterator(m_StatsIt);
}

Wiwa::Transform3D* Wiwa::PlayerController::GetTransform()
{
	return (Transform3D*)m_Scene->GetEntityManager().GetComponentByIterator(m_TransformIt);
}

Wiwa::CollisionBody* Wiwa::PlayerController::GetRigidBody()
{
	return (CollisionBody*)m_Scene->GetEntityManager().GetComponentByIterator(m_RigidbodyIt);
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

void Wiwa::PlayerController::TakeDamage(uint32_t damage)
{

}

void Wiwa::PlayerController::SpawnBullet(Transform3D& transform, const StarLordShooter& shooter, const RocketShooter& rocket, const Character& character, glm::vec3 bullDir)
{
	if (GameStateManager::s_PoolManager->s_StarLordBullets->getCountDisabled() <= 0)
		return;

	EntityManager& entityManager = m_Scene->GetEntityManager();

	GameStateManager::s_PoolManager->SetScene(m_Scene);
	EntityId newBulletId = EntityManager::INVALID_INDEX;
	newBulletId = GameStateManager::s_PoolManager->s_StarLordBullets->GetFromPool();
	EntityId bulletCollider = entityManager.GetChildByName(newBulletId, "Collider");
	if (newBulletId == EntityManager::INVALID_INDEX)
		return;

	SimpleBulletSystem* bulletSys = entityManager.GetSystem<SimpleBulletSystem>(bulletCollider);
	PhysicsSystem* physSys = entityManager.GetSystem<PhysicsSystem>(bulletCollider);
	physSys->DeleteBody();

	// Set intial positions
	Transform3D* playerTr = GetTransform();
	Transform3D* bulletTr = (Transform3D*)entityManager.GetComponentByIterator(entityManager.GetComponentIterator<Transform3D>(newBulletId));

	if (!bulletTr || !playerTr)
		return;

	bulletTr->localPosition = Math::GetWorldPosition(playerTr->worldMatrix);
	bulletTr->localRotation = glm::vec3(-90.0f, 0.0f, playerTr->localRotation.y + 90.0f);
	bulletTr->localScale = playerTr->localScale;
	SimpleBullet* bullet = (SimpleBullet*)entityManager.GetComponentByIterator(entityManager.GetComponentIterator<SimpleBullet>(bulletCollider));
	bullet->direction = bullDir;

	physSys->CreateBody();
	bulletSys->EnableBullet();
}

glm::vec3 Wiwa::PlayerController::GetMovementInput()
{
	//     z
	//     |
	//     |
	//-----|----- -x
	//     |
	//     |

	glm::vec3 input = glm::vec3(0.f);

	if (Input::IsKeyPressed(Key::A))
		input.x += 1;
	if (Input::IsKeyPressed(Key::D))
		input.x -= 1;
	if (Input::IsKeyPressed(Key::W))
		input.z += 1;
	if (Input::IsKeyPressed(Key::S))
		input.z -= 1;

	input.x -= Input::GetRawAxis(Gamepad::GamePad1, Gamepad::LeftX, GameStateManager::GetControllerDeadZone());
	input.z -= Input::GetRawAxis(Gamepad::GamePad1, Gamepad::LeftY, GameStateManager::GetControllerDeadZone());

	return input;
}

glm::vec3 Wiwa::PlayerController::GetShootingInput()
{
	//     z
	//     |
	//     |
	//-----|----- -x
	//     |
	//     |
	glm::vec3 input = glm::vec3(0.f);

	if (Input::IsKeyPressed(Key::Left))
		input.x -= 1;
	if (Input::IsKeyPressed(Key::Right))
		input.x += 1;
	if (Input::IsKeyPressed(Key::Up))
		input.z += 1;
	if (Input::IsKeyPressed(Key::Down))
		input.z -= 1;

	input.x -= Input::GetRawAxis(Gamepad::GamePad1, Gamepad::RightX, GameStateManager::GetControllerDeadZone());
	input.z -= Input::GetRawAxis(Gamepad::GamePad1, Gamepad::RightY, GameStateManager::GetControllerDeadZone());

	return input;
}

void Wiwa::PlayerController::SetPlayerRotation(const glm::vec3& input, const float rotationSpeed)
{
	Transform3D* transform = GetTransform();
	float angle = AngleFromVec2(glm::vec2(input.z, input.x));
	
	transform->localRotation.y = Math::LerpAngle(transform->localRotation.y, angle, rotationSpeed);

	if (transform->localRotation.y >= 360.f)
		transform->localRotation.y = 0.f;
}

float Wiwa::PlayerController::AngleFromVec2(const glm::vec2& vector)
{
	return glm::degrees(glm::atan(vector.y, vector.x));
}
