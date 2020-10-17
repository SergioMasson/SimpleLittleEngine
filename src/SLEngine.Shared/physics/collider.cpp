#include "pch.h"
#include "collider.h"
#include "../mathHelpers.h"
#include "physicsCore.h"

using namespace sle;

void physics::Collider::SetLocalPosition(math::Vector3 localPosition)
{
	physics::ColliderSetLocalPosition(this, localPosition);
}

void physics::Collider::SetLocalRotation(math::Quaternion localRotation)
{
	physics::ColliderSetLocalRotation(this, localRotation);
}