#pragma once

#include "Animation.h"

namespace MEngine::Graphics
{
	class AnimationBuilder
	{
	public:
		AnimationBuilder& AddPositionKey(const Math::Vector3& pos, float time, EaseType easeType = EaseType::Linear);
		AnimationBuilder& AddRotationKey(const Math::Quaternion& rot, float time, EaseType easeType = EaseType::Linear);
		AnimationBuilder& AddScaleKey(const Math::Vector3& scale, float time, EaseType easeType = EaseType::Linear);
		AnimationBuilder& AddEventKey(AnimationCallback cb, float time);


		[[nodiscard]] Animation  Build();
	private:
		Animation mWorkingCopy;
	};
}