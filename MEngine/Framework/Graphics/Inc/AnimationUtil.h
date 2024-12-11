#pragma once

#include "ModelManager.h";
#include "Animator.h"

namespace MEngine::Graphics::AnimationUtil
{
	using BoneTransforms = std::vector<Math::Matrix4>;

	void ComputeBoneTransforms(ModelID id, BoneTransforms& boneTransforms, const Animator* animator = nullptr);
	void ApplyBoneOfset(ModelID id, BoneTransforms& boneTransforms);
	void DrawSkeleton(ModelID id, BoneTransforms& boneTransforms);
}