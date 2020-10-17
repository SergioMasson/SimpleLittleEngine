#pragma once

#include "../gameObject.h"
#include "collider.h"

namespace sle
{
	namespace physics
	{
		class Collider;

		/**
		\brief Rigid body component for physics interaction.
		This components allows for components to participate on physics.
		*/
		class RigidBody : public Component
		{
		public:

			SLE_EXPORTS void AddBoxCollider(float width, float height, float depth, math::Vector3 relativePositon = math::Vector3(0, 0, 0));
			SLE_EXPORTS void AddCapsuleCollider(float radius, float halfHeight);
			SLE_EXPORTS void AddPlaneCollider(math::Vector3 position, math::Quaternion rotation);
			SLE_EXPORTS void AddPlaneCollider(math::Vector3 position, float distance);
			SLE_EXPORTS void AddPlaneCollider(math::Vector3 position, math::Vector3 normal);
			SLE_EXPORTS void SetCollisionFilters(uint32_t mask, uint32_t filter);

			SLE_EXPORTS math::Vector3 GetVelocity();

			SLE_EXPORTS void SetVelocity(math::Vector3 velocity);
			SLE_EXPORTS void AddVelocity(math::Vector3 velocity, float maxV = 0.0f);
			SLE_EXPORTS void AddForce(math::Vector3 force);
			SLE_EXPORTS void AddTorque(math::Vector3 torque);
			SLE_EXPORTS void SetPosition(math::Vector3 position);
			SLE_EXPORTS void SetRotation(math::Quaternion rotation);
			SLE_EXPORTS void MovePosition(math::Vector3 position);
			SLE_EXPORTS void MoveRotation(math::Quaternion rotation);

			SLE_EXPORTS bool CheckCollisionWith(uint32_t collisionLayer);
			SLE_EXPORTS bool IsOverlaping(RigidBody* other);

			Collider* GetCollider() { return &m_collider; };

			/**
			\brief Set Kinematic behaviur.
			Kinematic objects are not affected by forces and acceleration. Allow to controll object by MovePosition and MoveRotation.
			*/
			SLE_EXPORTS void IsKinematic(bool isKinematic);

			/**
			\brief Get Kinematic state of the object.
			Kinematic objects are not affected by forces and acceleration. Allow to controll object by MovePosition and MoveRotation.
			*/
			bool IsKinematic() const { return m_isKinematic; };

			bool IsStatic() const { return m_isStatic; };

			//TODO(Sergio): Move this to private.
		public:
			bool m_hasTarget{ false };
			math::Vector3 m_targetPositon;
			math::Quaternion m_targetRotation;

		private:
			bool m_isStatic;

		private:
			SLE_EXPORTS RigidBody(GameObject& gameObject, float mass, bool isStatic = false);
		private:
			Collider m_collider;

			float m_mass;
			bool m_isKinematic;
			friend GameObject;
		};
	}
}