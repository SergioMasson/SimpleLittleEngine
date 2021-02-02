#pragma once

#include "../mathHelpers.h"

namespace sle
{
	namespace physics
	{
		class  Collider
		{
		public:
			bool IsActive() const { return m_isActive; };
			void SetActive(bool isActive) { m_isActive = isActive; };
			void SetLocalPosition(math::Vector3 localPosition);
			void SetLocalRotation(math::Quaternion localRotation);

		private:
			bool m_isActive;
		};
	}
}
