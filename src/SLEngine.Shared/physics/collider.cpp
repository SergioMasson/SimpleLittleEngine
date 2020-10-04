#include "pch.h"
#include "collider.h"
#include "../mathHelpers.h"
#include "physicsCore.h"

void physics::Collider::SetLocalPosition(math::Vector3 localPosition)
{
	physics::ColliderSetLocalPosition(this, localPosition);
}

void physics::Collider::SetLocalRotation(math::Quaternion localRotation)
{
	physics::ColliderSetLocalRotation(this, localRotation);
}