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
		Math::Matrix4 boneTransform;

	

		void SetStatic(bool is_static = true) { this->mIsStatic = is_static; }
		bool GetStatic() { return this->mIsStatic; }

		std::string GetName();

		void SetHingeConstraint(Math::Vector3 axis, float min_ang = -(Math::pi), float max_ang = Math::pi);
		//Note: roll axis is not controlled for ball joints, i.e.it might rotate around the roll axis uncontrollably.

		void SetBallConstraint(float min_ang = -(Math::pi), float max_ang = Math::pi);
		bool GetHasRotationAxis() { return this->mHasRotationAxis; }
		Math::Vector3 GetAxis() { return this->mAxis; }

		float GetMinAng() { return this->mMinAng; }
		float GetMaxAng() { return this->mMaxAng; }

	private: 
				
		bool mIsStatic = false;
		Math::Vector3 mAxis = Math::Vector3(0.0, 0.0, 0.0);
		bool mHasRotationAxis = false;
		float mMinAng = 0;
		float mMaxAng = Math::pi * 2;

		Math::Vector4 mCol = Math::Vector4(0.2, 0.2, 0.7, 1);

		//some representation of position in local space
		//Vector3?
		//Extend Bone with GetWorldPosition function?

		
	};
}
