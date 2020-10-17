//
// Copyright (c) Microsoft. All rights reserved.
// This code is licensed under the MIT License (MIT).
// THIS CODE IS PROVIDED *AS IS* WITHOUT WARRANTY OF
// ANY KIND, EITHER EXPRESS OR IMPLIED, INCLUDING ANY
// IMPLIED WARRANTIES OF FITNESS FOR A PARTICULAR
// PURPOSE, MERCHANTABILITY, OR NON-INFRINGEMENT.
//
// Developed by Minigraph
//
// Author:  James Stanard
//
#pragma once
#include "vector3.h"

namespace sle
{
	namespace math
	{
		struct Quaternion
		{
		public:
			Quaternion() { m_vec = DirectX::XMQuaternionIdentity(); }
			Quaternion(const Vector3& axis, const Scalar& angle) { m_vec = DirectX::XMQuaternionRotationAxis(axis, angle); }
			Quaternion(float pitch, float yaw, float roll) { m_vec = DirectX::XMQuaternionRotationRollPitchYaw(pitch, yaw, roll); }
			Quaternion(float x, float y, float z, float w) { m_vec = DirectX::XMVectorSet(x, y, z, w); };
			Quaternion Inverse() const { return Quaternion(DirectX::XMQuaternionInverse(m_vec)); }

			explicit Quaternion(const DirectX::XMMATRIX& matrix) { m_vec = DirectX::XMQuaternionRotationMatrix(matrix); }
			explicit Quaternion(DirectX::FXMVECTOR vec) { m_vec = vec; }

			operator DirectX::XMVECTOR() const { return m_vec; }
			operator DirectX::XMFLOAT4() const
			{
				DirectX::XMFLOAT4 result;
				DirectX::XMStoreFloat4(&result, m_vec);
				return result;
			}

			Quaternion operator~ (void) const { return Quaternion(DirectX::XMQuaternionConjugate(m_vec)); }
			Quaternion operator- (void) const { return Quaternion(DirectX::XMVectorNegate(m_vec)); }

			Quaternion operator* (Quaternion rhs) const { return Quaternion(DirectX::XMQuaternionMultiply(rhs, m_vec)); }
			Vector3 operator* (Vector3 rhs) const { return Vector3(DirectX::XMVector3Rotate(rhs, m_vec)); }

			Quaternion& operator= (Quaternion rhs) { m_vec = rhs; return *this; }
			Quaternion& operator*= (Quaternion rhs) { *this = *this * rhs; return *this; }

			Vector3 GetRotationVector()
			{
				XMVECTOR axis;
				float angle;

				DirectX::XMQuaternionToAxisAngle(&axis, &angle, m_vec);
				return math::Vector3(axis) * angle;
			}

		public:
			DirectX::XMVECTOR m_vec;
		};
	}
}