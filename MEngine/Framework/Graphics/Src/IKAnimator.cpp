#include "Precompiled.h"
#include "IKAnimator.h"


using namespace MEngine;
using namespace MEngine::Graphics;


void IKAnimator::PlayAnimation(int clipIndex, bool looping)
{
    PlayAnimation(clipIndex, looping, 0.0f);
}

void IKAnimator::PlayAnimation(int clipIndex, bool looping, float transitionTime)
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

void IKAnimator::Update(float deltaTime)
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

bool IKAnimator::IsFinished() const
{
    if (mCurrentAnimation.isLooping || mCurrentAnimation.animIndex < 0)
    {
        return false;
    }

    auto model = ModelManager::Get()->GetModel(mModelId);
    const auto& animClip = model->animationClips[mCurrentAnimation.animIndex];
    return mCurrentAnimation.animationTime >= animClip.ticksDuration;
}

Math::Matrix4 IKAnimator::GetToParentTransform(const Bone* bone) const
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
        
        //if (animClip.boneAnimation[bone->index])

        //const Bones* bones = &(model->skeleton->bones);

        for (int iter = 0; iter < maxIterations; ++iter)
        {
            bool allBonesAdjusted = true;
            for (int i = model->skeleton->bones.size() - 1; i >= 0; --i)
            {
                updateBoneRotation(bones[i], target); //Update positions based on new rotations
                for (int j = i; j < bones.size(); ++j)
                {
                    if (bones[j].parent)
                    {
                        Math::Vector3 t1 = getBonePosition(bones[j].parent->toParentTransform * bones[j].offsetTransform);
                        Math::Vector4 translate = bones[j].parent->toParentTransform.multiplyMatrixByVector({ t1.x, t1.y, t1.z, 1.0 });
                        bones[j].toParentTransform.Translation(translate.x, translate.y, translate.z);

                        //bones[j].toParentTransform[3] = bones[j].parent->toParentTransform * glm::vec4(getBonePosition(bones[j].parent->toParentTransform * bones[j].offsetTransform),
                    }
                    bones[j].offsetTransform = bones[j].toParentTransform.Inverse();
                }
                // Check if end effector is within the threshold
                if (Math::Distance(getBonePosition(bones.back().toParentTransform * bones.back().offsetTransform), target) < threshold)
                {
                    return;
                    // Target reached 
                }
                allBonesAdjusted = false;
            }
            if (allBonesAdjusted)
            {
                break;
            }
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

void IKAnimator::SetNodeAnimation(BlendDirection dir, int clipIndex)
{
    mBlendNodes[dir].animIndex = clipIndex;
    mBlendNodes[dir].animationTime = 0.0f;
    mBlendNodes[dir].isLooping = true;
}

void IKAnimator::SetBlendWeight(BlendDirection dir, float weight)
{
    mBlendNodes[dir].blendWeight = std::clamp(weight, 0.0f, 1.0f);
}





Math::Vector3 getBonePosition(const Math::Matrix4& transform)
{
    return transform.GetPosition(transform);
}

void setBoneRotation(Bone& bone, const Math::Matrix4& rotationMatrix)
{
    Math::Vector3 position = getBonePosition(bone.toParentTransform);
    bone.toParentTransform = Math::Matrix4::Translation(position) * rotationMatrix;
}

//*should be properly translated from GLM
void updateBoneRotation(Bone& bone, const Math::Vector3& target)
{
    Math::Vector3 bonePosition = getBonePosition(bone.toParentTransform);
    Math::Vector3 toTarget = Math::Normalize(target - bonePosition);
    Math::Vector3 toEndEffector = Math::Normalize(getBonePosition(bone.toParentTransform * bone.offsetTransform) - bonePosition);
    float dotProduct = Math::Dot(toTarget, toEndEffector);
    dotProduct = Math::Clamp(dotProduct, -1.0f, 1.0f);
    // Avoid floating-point errors 
    float angle = acos(dotProduct);
    Math::Vector3 rotationAxis = Math::Cross(toEndEffector, toTarget);
    if (Math::Vector3::Length(rotationAxis) > 0.0001f)
    {
        // Avoid zero-length axis 
        rotationAxis = Math::Normalize(rotationAxis);

        //Do rotations need to be compiled into a single matrix?

        Math::Matrix4 rotationMatrix = Math::Matrix4::ComposeRotation(rotationAxis, angle);
        setBoneRotation(bone, rotationMatrix * bone.toParentTransform);
    }
}

void solveIK(const std::vector<Bone>& bones, const Math::Vector3& target, int maxIterations, float threshold)
{
    for (int iter = 0; iter < maxIterations; ++iter) {
        bool allBonesAdjusted = true; for (int i = bones.size() - 1; i >= 0; --i) {
            updateBoneRotation(bones[i], target); // Update positions based on new rotations 
            for (int j = i; j < bones.size(); ++j)
            {
                if (bones[j].parent)
                {
                    Math::Vector3 t1 = getBonePosition(bones[j].parent->toParentTransform * bones[j].offsetTransform);
                    Math::Vector4 translate = bones[j].parent->toParentTransform.multiplyMatrixByVector({ t1.x, t1.y, t1.z, 1.0 });
                    bones[j].toParentTransform.Translation(translate.x, translate.y, translate.z);

                    //bones[j].toParentTransform[3] = bones[j].parent->toParentTransform * glm::vec4(getBonePosition(bones[j].parent->toParentTransform * bones[j].offsetTransform),
                }
                bones[j].offsetTransform = bones[j].toParentTransform.Inverse();
            }
            // Check if end effector is within the threshold 
            if (Math::Distance(getBonePosition(bones.back().toParentTransform * bones.back().offsetTransform), target) < threshold)
            {
                return;
                // Target reached 
            }
            allBonesAdjusted = false;
        }
        if (allBonesAdjusted)
        {
            break;
        }
    }
}