#pragma once

#include "../mathHelpers.h"

namespace sle
{
	namespace physics
	{
		class RigidBody;
		class Collider;

		struct RaycastHit
		{
			math::Vector3 Position;
			math::Vector3 Normal;
			RigidBody* RigibBodyHit;
		};

		SLE_EXPORTS void Initialize();
		SLE_EXPORTS void Shutdown();
		SLE_EXPORTS void Update(float deltaT);

		SLE_EXPORTS void CreateRigidBody(RigidBody* rigidBody, math::Vector3 position, math::Quaternion rotation, float initalMass, bool isKinematic = false, bool isStatic = false);
		SLE_EXPORTS void RigidBodySetMass(RigidBody* rigidBody, float mass);
		SLE_EXPORTS math::Vector3 RigidBodyGetVelocity(RigidBody* rigidBody);
		SLE_EXPORTS void RigidBodySetVelocity(RigidBody* rigidBody, math::Vector3 velocity);
		SLE_EXPORTS void RigidBodyAddVelocity(RigidBody* rigidBody, math::Vector3 velocity);
		SLE_EXPORTS void RigidBodyAddVelocity(RigidBody* rigidBody, math::Vector3 velocity, float maxModule);
		SLE_EXPORTS void RigidBodyAddVelocity(RigidBody* rigidBody, math::Vector3 velocity, float maxModule);
		SLE_EXPORTS void RigidBodyAddForce(RigidBody* rigidBody, math::Vector3 force);
		SLE_EXPORTS void RigidBodyAddTorque(RigidBody* rigidBody, math::Vector3 torque);
		SLE_EXPORTS void RigidBodySetKinematic(RigidBody* rigidBody, bool isKinematic);
		SLE_EXPORTS void RigidBodySetCollisionMask(RigidBody* rigidBody, uint32_t mask, uint32_t filter);
		SLE_EXPORTS void RigidBodySetPosition(RigidBody* rigidBody, math::Vector3 position);
		SLE_EXPORTS void RigidBodySetRotation(RigidBody* rigidBody, math::Quaternion rotation);
		SLE_EXPORTS bool RigidBodyCheckCollisionWith(RigidBody* rigidBody, uint32_t collisionLayer);

		SLE_EXPORTS bool RigidBodyIsOverlaping(RigidBody* rigidBody1, RigidBody* rigidBody2);

		//Colliders geometry.
		SLE_EXPORTS void CreateCapsuleCollider(RigidBody* rigidBody, float radius, float halfHeight);
		SLE_EXPORTS void RigidBodyCreateBoxCollider(RigidBody* rigidBody, float width, float height, float depth, math::Vector3 relativePositon = math::Vector3(0, 0, 0));
		SLE_EXPORTS void RigidBodyCreatePlaneCollider(RigidBody* rigidBody, math::Vector3 position, math::Quaternion rotation);
		SLE_EXPORTS void RigidBodyCreatePlaneCollider(RigidBody* rigidBody, float x, float y, float z, float w);
		SLE_EXPORTS void RigidBodyCreatePlaneCollider(RigidBody* rigidBody, math::Vector3 position, float distance);
		SLE_EXPORTS void RigidBodyCreatePlaneCollider(RigidBody* rigidBody, math::Vector3 position, math::Vector3 normal);

		SLE_EXPORTS void ColliderSetLocalPosition(Collider* collider, math::Vector3 localPosition);
		SLE_EXPORTS void ColliderSetLocalRotation(Collider* collider, math::Quaternion localRotation);

		SLE_EXPORTS bool Raycast(math::Vector3 origin, math::Vector3 direction, RaycastHit& result, float maxDistance, uint32_t layerMask);
	}
}