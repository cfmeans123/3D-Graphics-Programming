#pragma once

#include "Common.h"

namespace MEngine::Graphics
{
	struct Bone
	{
		std::string name;
		int index = -1;

		Bone* parent = nullptr;
		int parentIndex = 1;

		std::vector<Bone*> children;
		std::vector<int> childrenIndices;

		Math::Matrix4 toParentTransform;
		Math::Matrix4 offsetTransform;

		//constraints
		Math::Vector3 minRotation = { -10.0f, -10.0f, -10.0f};
		Math::Vector3 maxRotation = {45.0f, 45.0f, 45.0f};


		Math::Vector3 applyConstraints(const Math::Vector3& rotation) const 
		{
			Math::Vector3 constrainedRotation = rotation;
			constrainedRotation.x = Math::Clamp(constrainedRotation.x, minRotation.x, maxRotation.x); 
			constrainedRotation.y = Math::Clamp(constrainedRotation.y, minRotation.y, maxRotation.y);
			constrainedRotation.z = Math::Clamp(constrainedRotation.z, minRotation.z, maxRotation.z); 
			return constrainedRotation; 
		}
	};
}
