#include "pch.h"
#include "rigidBody.h"
#include "physicsCore.h"

using namespace sle;

physics::RigidBody::RigidBody(GameObject& gameObject, float mass, bool isStatic) :
	Component(gameObject),
	m_mass{ mass },
	m_isStatic{ isStatic }
{
	physics::CreateRigidBody(this, gameObject.GetPosition(), gameObject.GetRotation(), mass, isStatic);
}

void physics::RigidBody::AddBoxCollider(float width, float height, float depth, math::Vector3 relativePosition)
{
	physics::RigidBodyCreateBoxCollider(this, width, height, depth, relativePosition);
}

void physics::RigidBody::AddCapsuleCollider(float radius, float halfHeight)
{
	physics::CreateCapsuleCollider(this, radius, halfHeight);
}

void physics::RigidBody::AddPlaneCollider(math::Vector3 position, math::Quaternion rotation)
{
	physics::RigidBodyCreatePlaneCollider(this, position, rotation);
}

void physics::RigidBody::AddPlaneCollider(math::Vector3 position, float distance)
{
	physics::RigidBodyCreatePlaneCollider(this, position, distance);
}

void physics::RigidBody::AddPlaneCollider(math::Vector3 position, math::Vector3 normal)
{
	physics::RigidBodyCreatePlaneCollider(this, position, normal);
}

void physics::RigidBody::SetCollisionFilters(uint32_t mask, uint32_t filter)
{
	physics::RigidBodySetCollisionMask(this, mask, filter);
}

math::Vector3 physics::RigidBody::GetVelocity()
{
	return physics::RigidBodyGetVelocity(this);
}

void physics::RigidBody::SetVelocity(math::Vector3 velocity)
{
	physics::RigidBodySetVelocity(this, velocity);
}

void physics::RigidBody::AddVelocity(math::Vector3 velocity, float maxV)
{
	if (maxV != 0.0f)
	{
		physics::RigidBodyAddVelocity(this, velocity, maxV);
	}
	else
	{
		physics::RigidBodyAddVelocity(this, velocity);
	}
}

void physics::RigidBody::AddForce(math::Vector3 force)
{
	physics::RigidBodyAddForce(this, force);
}

void physics::RigidBody::AddTorque(math::Vector3 torque)
{
	physics::RigidBodyAddTorque(this, torque);
}

void physics::RigidBody::SetPosition(math::Vector3 position)
{
	physics::RigidBodySetPosition(this, position);
}

void physics::RigidBody::SetRotation(math::Quaternion rotation)
{
	physics::RigidBodySetRotation(this, rotation);
}

void physics::RigidBody::MovePosition(math::Vector3 position)
{
	m_targetPositon = position;
	m_hasTarget = true;
}

void physics::RigidBody::MoveRotation(math::Quaternion rotation)
{
	m_targetRotation = rotation;
	m_hasTarget = true;
}

bool physics::RigidBody::CheckCollisionWith(uint32_t collisionLayer)
{
	return false;
}

bool physics::RigidBody::IsOverlaping(RigidBody* other)
{
	return physics::RigidBodyIsOverlaping(this, other);
}

void physics::RigidBody::IsKinematic(bool isKinematic)
{
	m_isKinematic = isKinematic;
	RigidBodySetKinematic(this, m_isKinematic);
}