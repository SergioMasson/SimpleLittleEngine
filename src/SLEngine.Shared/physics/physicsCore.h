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

		void Initialize();
		void Shutdown();
		void Update(float deltaT);

		void CreateRigidBody(RigidBody* rigidBody, math::Vector3 position, math::Quaternion rotation, float initalMass, bool isKinematic = false, bool isStatic = false);
		void RigidBodySetMass(RigidBody* rigidBody, float mass);
		math::Vector3 RigidBodyGetVelocity(RigidBody* rigidBody);
		void RigidBodySetVelocity(RigidBody* rigidBody, math::Vector3 velocity);
		void RigidBodyAddVelocity(RigidBody* rigidBody, math::Vector3 velocity);
		void RigidBodyAddVelocity(RigidBody* rigidBody, math::Vector3 velocity, float maxModule);
		void RigidBodyAddVelocity(RigidBody* rigidBody, math::Vector3 velocity, float maxModule);
		void RigidBodyAddForce(RigidBody* rigidBody, math::Vector3 force);
		void RigidBodyAddTorque(RigidBody* rigidBody, math::Vector3 torque);
		void RigidBodySetKinematic(RigidBody* rigidBody, bool isKinematic);
		void RigidBodySetCollisionMask(RigidBody* rigidBody, uint32_t mask, uint32_t filter);
		void RigidBodySetPosition(RigidBody* rigidBody, math::Vector3 position);
		void RigidBodySetRotation(RigidBody* rigidBody, math::Quaternion rotation);
		bool RigidBodyCheckCollisionWith(RigidBody* rigidBody, uint32_t collisionLayer);

		bool RigidBodyIsOverlaping(RigidBody* rigidBody1, RigidBody* rigidBody2);

		//Colliders geometry.
		void CreateCapsuleCollider(RigidBody* rigidBody, float radius, float halfHeight);
		void RigidBodyCreateBoxCollider(RigidBody* rigidBody, float width, float height, float depth, math::Vector3 relativePositon = math::Vector3(0, 0, 0));
		void RigidBodyCreatePlaneCollider(RigidBody* rigidBody, math::Vector3 position, math::Quaternion rotation);
		void RigidBodyCreatePlaneCollider(RigidBody* rigidBody, float x, float y, float z, float w);
		void RigidBodyCreatePlaneCollider(RigidBody* rigidBody, math::Vector3 position, float distance);
		void RigidBodyCreatePlaneCollider(RigidBody* rigidBody, math::Vector3 position, math::Vector3 normal);

		void ColliderSetLocalPosition(Collider* collider, math::Vector3 localPosition);
		void ColliderSetLocalRotation(Collider* collider, math::Quaternion localRotation);

		bool Raycast(math::Vector3 origin, math::Vector3 direction, RaycastHit& result, float maxDistance, uint32_t layerMask);
	}
}