#include "Precompiled.h"
#include "IKChain.h"

using namespace MEngine;
using namespace MEngine::Graphics;


void IKChain::Initialize(const Model* model, int baseLinkIndex, int endEffectorIndex, int maxIterations, float threshold, Math::Vector3 target)
{
    mBaseLinkIndex = baseLinkIndex;
    mEndEffectorIndex = endEffectorIndex;
    mMaxIterations = maxIterations;
    mThreshold = threshold;
    mTarget = target;

    if (!organizedBones.positionBones.empty() || organizedBones.preRotationBones.empty())
    {
        organizedBones.positionBones.clear();
        organizedBones.preRotationBones.clear();
    }
    for (int i = mEndEffectorIndex; i >= mBaseLinkIndex; --i)
    {
        if (model->skeleton->bones[i]->name.find("PreRotation") != std::string::npos)
        {
            organizedBones.preRotationBones.push_back(model->skeleton->bones[i].get());
        }
        else if (model->skeleton->bones.at(i)->name.find("Translation") == std::string::npos)
        {
            organizedBones.positionBones.push_back(model->skeleton->bones[i].get());
        }
    }

}
void applyPreRotation(Bone* bone, Bone* preRotationBone)
{
    // Assume preRotationBone->toParentTransform contains the pre-rotation matrix 
    bone->toParentTransform = preRotationBone->toParentTransform * bone->toParentTransform;
}

Math::Vector3 getBonePosition(const Bone& bone)
{
    return TransformCoord(Math::Vector3(0.0f), bone.toParentTransform);
}

void setBoneRotation(Bone& bone, const Math::Matrix4& rotationMatrix)
{
    Math::Vector3 position = getBonePosition(bone);
    Math::Matrix4 translationMatrix = Math::Matrix4::Translation(position.x, position.y, position.z);
    bone.toParentTransform = translationMatrix * rotationMatrix;
}

void IKChain::SolveIK()
{
    for (size_t i = 0; i < organizedBones.positionBones.size(); ++i)
    { 
        //applyPreRotation(organizedBones.positionBones[i], organizedBones.preRotationBones[i]);
    } 
    // Perform IK on position bones 
    for (int iter = 0; iter < mMaxIterations; ++iter)
    {
        bool allBonesAdjusted = true;
        for (int i = organizedBones.positionBones.size() - 1; i >= 0; --i)
        {
            Bone* bone = organizedBones.positionBones[i];
            Math::Vector3 endEffectorPosition = TransformCoord(Math::Vector3(0.0f), bone->toParentTransform * bone->offsetTransform);
            if (Math::Distance(endEffectorPosition, mTarget) < mThreshold)
            {
                return;
                // Target reached 
            }
            UpdateBoneRotation(*bone, mTarget);
            // Update positions based on new rotations 
            for (size_t j = i; j < organizedBones.positionBones.size(); ++j)
            {
                Bone* childBone = organizedBones.positionBones[j];
                if (childBone->parent)
                {
                    childBone->toParentTransform = childBone->parent->toParentTransform * childBone->offsetTransform;
                }
            }
            allBonesAdjusted = false;
        }
        if (allBonesAdjusted)
        {
            break;
        }
    }

}



void MEngine::Graphics::IKChain::UpdateBoneRotation(Bone& bone, const Math::Vector3& target)
{
    Math::Vector3 bonePosition = getBonePosition(bone);
    Math::Vector3 endEffectorPosition = TransformCoord(Math::Vector3(0.0f), bone.toParentTransform * bone.offsetTransform); 
    Math::Vector3 direction = endEffectorPosition - bonePosition; 
    if (Math::Vector3::Length(direction) > 0.0001f)
    {
        Math::Vector3 toEndEffector = Math::Normalize(direction); 
        Math::Vector3 toTarget = Math::Normalize(target - bonePosition); 
        // Compute the rotation axis and angle 
        Math::Vector3 rotationAxis = Math::Cross(toEndEffector, toTarget); 
        float angle = acos(Math::Clamp(Math::Dot(toEndEffector, toTarget), -1.0f, 1.0f)); 
        if (Math::Vector3::Length(rotationAxis) > 0.0001f) 
        { 
            rotationAxis = Math::Normalize(rotationAxis); 
            Math::Vector3 deltaRotation = (rotationAxis * angle).toDegrees(); 
            Math::Vector3 constrainedRotation = bone.applyConstraints(deltaRotation); 
            Math::Matrix4 rotationMatrix = Math::Matrix4::ComposeRotation(constrainedRotation, 1.0f); 
            setBoneRotation(bone, rotationMatrix * bone.toParentTransform); 
        } 
    } 
    else 
    { 
        //std::cerr << "Error: Zero-length direction vector encountered." << std::endl; 
    } 
}

