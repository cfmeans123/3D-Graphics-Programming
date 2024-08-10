#pragma once

#include "KeyFrame.h"
#include "Transform.h"

namespace MEngine::Graphics
{
	class Animation
	{
	public:
		Transform GetTransform(float time) const;
		float GetDuration() const;


		void PlayEvents(float prevTime, float currentTime);

	private:
		Math::Vector3 GetPosition(float time) const;
		Math::Quaternion GetRotation(float time) const;
		Math::Vector3 GetScale(float time) const;

		friend class AnimationBuilder;
		friend class AnimationIO;

		EventKeys mEventKeys;
		PositionKeys mPositionKeys;
		RotationKeys mRotationKeys;
		ScaleKeys mScaleKeys;
		float mDuration;
	};
}