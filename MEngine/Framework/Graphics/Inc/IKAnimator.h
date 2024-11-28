#pragma once

#include "Animator.h"

//Consolidate all the IKSolver code into this class here and get working first

class IKAnimator : public Animator
{
public:
	void Initialize(ModelID id);
	void PlayAnimation(int clipIndex, bool looping);

	void Update(float deltaTime);

	bool IsFinished() const;
	size_t GetAnimationCount() const;
	Math::Matrix4 GetToParentTransform(const Bone* bone) const;

protected:
	ModelID mModelId = 0;
	int mClipIndex = -1;
	float mAnimationTick = 0.0f;
	bool mIsLooping = false;

};