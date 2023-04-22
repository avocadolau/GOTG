#include <wipch.h>
#include "PlayerController.h"

#include <Wiwa/game/GameStateManager.h>
#include <Wiwa/ecs/Components.h>

#include <Wiwa/utilities/math/Math.h>
#include <Wiwa/audio/Audio.h>
#include <Wiwa/ecs/EntityManager.h>

#include "../../Components.h"

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

void Wiwa::PlayerController::SpawnBullet(Transform3D& transform, const StarLordShooter& shooter, const Character& character, glm::vec3 bullDir)
{
	EntityId bullet = GetEntityManager().LoadPrefab("Prefabs/StarLordBullet.wiprefab");
	Transform3D* bulletTransform = GetEntityManager().GetComponent<Transform3D>(bullet);
	Transform3D* playerTransform = GetComponentByIterator<Transform3D>(m_TransformIt);

	bulletTransform->localPosition = Math::GetWorldPosition(transform.worldMatrix);
	bulletTransform->localRotation = glm::vec3(-90.f, playerTransform->localRotation.y - 90.f, 0.f);
	
	// TODO: Play bullet sound here
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
		input.x += 1;
	if (Input::IsKeyPressed(Key::Right))
		input.x -= 1;
	if (Input::IsKeyPressed(Key::Up))
		input.z += 1;
	if (Input::IsKeyPressed(Key::Down))
		input.z -= 1;

	input.x -= Input::GetRawAxis(Gamepad::GamePad1, Gamepad::RightX, GameStateManager::GetControllerDeadZone());
	input.z -= Input::GetRawAxis(Gamepad::GamePad1, Gamepad::RightY, GameStateManager::GetControllerDeadZone());

	return input;
}

void Wiwa::PlayerController::SetPlayerRotation(glm::vec3 currentRotation, const glm::vec3& input, const float rotationSpeed)
{
	float angle = AngleFromVec2(input);

	currentRotation.y = Math::LerpAngle(currentRotation.y, angle, rotationSpeed);

	if (currentRotation.y >= 360.f)
		currentRotation.y = 0.f;
}

float Wiwa::PlayerController::AngleFromVec2(const glm::vec2& vector)
{
	return glm::degrees(glm::atan(vector.y, vector.x));
}
