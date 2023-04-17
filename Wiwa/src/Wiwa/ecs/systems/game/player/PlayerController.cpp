#include "PlayerController.h"

#include <Wiwa/game/GameStateManager.h>
#include <Wiwa/ecs/Components.h>

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
	//m_ShooterIt = GetComponent<StarLordShooter>();
	GameStateManager::LoadProgression();
}

void Wiwa::PlayerController::OnUpdate()
{
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



Wiwa::Transform3D* Wiwa::PlayerController::GetFirePosition(const char* name)
{
	EntityId objectId = m_Scene->GetEntityManager().GetChildByName(m_EntityId, name);
	EntityManager::ComponentIterator it = GetComponentIterator<Transform3D>(objectId);
	return (Transform3D*)m_Scene->GetEntityManager().GetComponentByIterator(it);
}

void Wiwa::PlayerController::TakeDamage(uint32_t damage)
{

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

	input.x -= Input::GetRawAxis(Gamepad::GamePad1, Gamepad::LeftX, GameStateManager::GetControllerDeadZone());
	input.z -= Input::GetRawAxis(Gamepad::GamePad1, Gamepad::LeftY, GameStateManager::GetControllerDeadZone());

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
