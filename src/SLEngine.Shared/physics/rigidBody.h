#pragma once

#include "../gameObject.h"

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

		void AddBoxCollider(float width, float height, float depth, math::Vector3 relativePositon = math::Vector3(0, 0, 0));
		void AddCapsuleCollider(float radius, float halfHeight);
		void AddPlaneCollider(math::Vector3 position, math::Quaternion rotation);
		void AddPlaneCollider(math::Vector3 position, float distance);
		void AddPlaneCollider(math::Vector3 position, math::Vector3 normal);
		void SetCollisionFilters(uint32_t mask, uint32_t filter);

		math::Vector3 GetVelocity() const;

		void SetVelocity(math::Vector3 velocity);
		void AddVelocity(math::Vector3 velocity, float maxV = 0.0f);
		void AddForce(math::Vector3 force);
		void AddTorque(math::Vector3 torque);
		void SetPosition(math::Vector3 position);
		void SetRotation(math::Quaternion rotation);
		void MovePosition(math::Vector3 position);
		void MoveRotation(math::Quaternion rotation);

		/**
		\brief Set Kinematic behaviur.
		Kinematic objects are not affected by forces and acceleration. Allow to controll object by MovePosition and MoveRotation.
		*/
		void IsKinematic(bool isKinematic);

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
		RigidBody(GameObject* gameObject, float mass, bool isStatic = false);
	private:
		Collider* m_collider;

		float m_mass;
		bool m_isKinematic;
		friend GameObject;
	};
}