#pragma once

#include "Common.h"

namespace MEngine::Graphics
{
	struct Transform
	{
		Math::Vector3 position = Math::Vector3::Zero;
		Math::Quaternion rotation = Math::Quaternion::Identity;
		Math::Vector3 scale = Math::Vector3::One;

		Math::Matrix4 GetMatrix4() const
		{
			return {
				Math::Matrix4::Scaling(scale) *
				Math::Matrix4::MatrixRotationQuaternion(rotation) *
				Math::Matrix4::Translation(position)
			};
		}
	};
}