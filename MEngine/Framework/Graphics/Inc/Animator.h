#pragma once
#include "ModelManager.h"
#include "Bone.h"

namespace MEngine::Graphics
{
	class Animator
	{
	public:
		Animator() = default;
		virtual ~Animator() = default;

		virtual void Initialize(ModelID id);
		virtual void PlayAnimation(int clipIndex, bool looping);
		virtual void Update(float deltaTime);

		virtual bool IsFinished() const;
		virtual size_t GetAnimationCount() const;
		virtual Math::Matrix4 GetToParentTransform(const Bone* bone) const;

	protected:
		ModelID mModelId = 0;
		int mClipIndex = -1;
		float mAnimationTick = 0.0f;
		bool mIsLooping = false;
	};
}