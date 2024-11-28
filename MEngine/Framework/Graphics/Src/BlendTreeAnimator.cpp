#include "Precompiled.h"
#include "BlendTreeAnimator.h"

using namespace MEngine;
using namespace MEngine::Graphics;

void MEngine::Graphics::BlendTreeAnimator::PlayAnimation(int clipIndex, bool looping)
{
    PlayAnimation(clipIndex, looping, 0.0f);
}

void MEngine::Graphics::BlendTreeAnimator::PlayAnimation(int clipIndex, bool looping, float transitionTime)
{
    mBlendDuration = transitionTime;
    mBlendTime = 0.0f;
    if (transitionTime > 0.0f)
    {
        mNextAnimation.animIndex = clipIndex;
        mNextAnimation.isLooping = looping;
        mNextAnimation.animationTime = 0.0f;
    }
    else
    {
        mCurrentAnimation.animIndex = clipIndex;
        mCurrentAnimation.isLooping = looping;
        mCurrentAnimation.animationTime = 0.0f;
    }
}

void MEngine::Graphics::BlendTreeAnimator::Update(float deltaTime)
{
    if (mBlendDuration > 0.0f)
    {
        mBlendTime += deltaTime;
        if (mBlendTime >= mBlendDuration)
        {
            if (mNextAnimation.animIndex >= 0)
            {
                mCurrentAnimation = mNextAnimation;
                mNextAnimation.animIndex = -1;
            }
            mBlendDuration = 0.0f;
            mBlendTime = 0.0f;
        }
    }

    auto model = ModelManager::Get()->GetModel(mModelId);
    if (mCurrentAnimation.animIndex >= 0)
    {
        const auto& animClip = model->animationClips[mCurrentAnimation.animIndex];
        mCurrentAnimation.animationTime += animClip.ticksPerSecond * deltaTime;
        if (mCurrentAnimation.isLooping)
        {
            while (mCurrentAnimation.animationTime >= animClip.ticksDuration)
            {
                mCurrentAnimation.animationTime -= animClip.ticksDuration;
            }
        }
        else
        {
            mCurrentAnimation.animationTime = std::min(mCurrentAnimation.animationTime, animClip.ticksDuration);
        }
    }
    if (mNextAnimation.animIndex >= 0)
    {
        const auto& animClip = model->animationClips[mNextAnimation.animIndex];
        mNextAnimation.animationTime += animClip.ticksPerSecond * deltaTime;
        if (mNextAnimation.isLooping)
        {
            while (mNextAnimation.animationTime >= animClip.ticksDuration)
            {
                mNextAnimation.animationTime -= animClip.ticksDuration;
            }
        }
        else
        {
            mNextAnimation.animationTime = std::min(mNextAnimation.animationTime, animClip.ticksDuration);
        }
    }

    for (auto& node : mBlendNodes)
    {
        if (node.second.animIndex >= 0)
        {
            const auto& animClip = model->animationClips[node.second.animIndex];
            node.second.animationTime += animClip.ticksPerSecond * deltaTime;
            if (node.second.isLooping)
            {
                while (node.second.animationTime >= animClip.ticksDuration)
                {
                    node.second.animationTime -= animClip.ticksDuration;
                }
            }
            else
            {
                node.second.animationTime = std::min(node.second.animationTime, animClip.ticksDuration);
            }
        }
    }
}

bool MEngine::Graphics::BlendTreeAnimator::IsFinished() const
{
    if (mCurrentAnimation.isLooping || mCurrentAnimation.animIndex < 0)
    {
        return false;
    }

    auto model = ModelManager::Get()->GetModel(mModelId);
    const auto& animClip = model->animationClips[mCurrentAnimation.animIndex];
    return mCurrentAnimation.animationTime >= animClip.ticksDuration;
}

Math::Matrix4 MEngine::Graphics::BlendTreeAnimator::GetToParentTransform(const Bone* bone) const
{
    //if (mCurrentAnimation.animIndex < 0)
    //{
    //    return bone->toParentTransform;
    //}

    //const Model* model = ModelManager::Get()->GetModel(mModelId);
    //const AnimationClip& animClip = model->animationClips[mCurrentAnimation.animIndex];
    //const Animation* animation = animClip.boneAnimation[bone->index].get();
    //if (animation == nullptr)
    //{
    //    return Math::Matrix4::Identity;
    //}

    //Transform transform = animation->GetTransform(mCurrentAnimation.animationTime);
    //if (mNextAnimation.animIndex >= 0)
    //{
    //    const AnimationClip& nextAnimClip = model->animationClips[mNextAnimation.animIndex];
    //    const Animation* nextAnimation = nextAnimClip.boneAnimation[bone->index].get();
    //    if (nextAnimation != nullptr)
    //    {
    //        Transform nextTransform = nextAnimation->GetTransform(mNextAnimation.animationTime);
    //        float t = mBlendTime / mBlendDuration;
    //        transform.position = Lerp(transform.position, nextTransform.position, t);
    //        transform.rotation = Math::Quaternion::slerp(transform.rotation, nextTransform.rotation, t);
    //        transform.scale = Lerp(transform.scale, nextTransform.scale, t);
    //    }
    //}

    //return transform.GetMatrix4();

    // IK result
    // if IK allows animation, proceed and blend
    // if DoIKResult(bone, out weight)
    //  apply to bone
    //  if weight < 1.0, get blend node results, add together by weight

    float weightSum = 0.0f;
    for (auto& node : mBlendNodes)
    {
        if (node.second.animIndex >= 0)
        {
            weightSum += node.second.blendWeight;
        }
    }
    if (weightSum <= 0.0f)
    {
        return bone->toParentTransform;
    }
    Transform transform;
    bool firstOne = true;
    const Model* model = ModelManager::Get()->GetModel(mModelId);
    for (auto& node : mBlendNodes)
    {
        if (node.second.blendWeight <= 0.0f)
        {
            continue;
        }
        const AnimationClip& animClip = model->animationClips[node.second.animIndex];
        const Animation* animation = animClip.boneAnimation[bone->index].get();
        if (animation == nullptr)
        {
            return Math::Matrix4::Identity;
        }

        Transform animTransform = animation->GetTransform(node.second.animationTime);
        float t = (firstOne) ? 1.0f : node.second.blendWeight / weightSum;
        transform.position = Lerp(transform.position, animTransform.position, t);
        transform.rotation = Math::Quaternion::slerp(transform.rotation, animTransform.rotation, t);
        transform.scale = Lerp(transform.scale, animTransform.scale, t);
        firstOne = false;
    }

    return transform.GetMatrix4();
}

void MEngine::Graphics::BlendTreeAnimator::SetNodeAnimation(BlendDirection dir, int clipIndex)
{
    mBlendNodes[dir].animIndex = clipIndex;
    mBlendNodes[dir].animationTime = 0.0f;
    mBlendNodes[dir].isLooping = true;
}

void MEngine::Graphics::BlendTreeAnimator::SetBlendWeight(BlendDirection dir, float weight)
{
    mBlendNodes[dir].blendWeight = std::clamp(weight, 0.0f, 1.0f);
}
