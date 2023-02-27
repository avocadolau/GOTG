#pragma once
#include <btBulletDynamicsCommon.h>
#include <glm/glm.hpp>

void SetLinearVelocity(btRigidBody* body, const glm::vec3 velocity)
{
	body->applyCentralImpulse(btVector3(velocity.x, velocity.y, velocity.z));
}
