#include "Precompiled.h"
#include "IKChain.h"
#include <iostream>

using namespace MEngine;
using namespace MEngine::Graphics;


void IKChain::Initialize(const Model* model, int baseLinkIndex, int endEffectorIndex, int maxIterations, float threshold, Math::Vector3 target)
{
    mBaseLinkIndex = baseLinkIndex;
    mEndEffectorIndex = endEffectorIndex;
    mMaxIterations = maxIterations;
    mThreshold = threshold;
    mTarget = target;
    root = model->skeleton->root;

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
    endEffector = organizedBones.positionBones[0];
} 
void applyPreRotation(Bone* bone, Bone* preRotationBone)
{
    // Assume preRotationBone->toParentTransform contains the pre-rotation matrix 
    bone->toParentTransform = preRotationBone->toParentTransform * preRotationBone->parent->toParentTransform;
}

Math::Vector3 getBonePosition(const Bone& bone)
{
    Math::Matrix4 combinedTransform; 
    if (bone.parent) 
    { 
        combinedTransform = bone.parent->toParentTransform * bone.offsetTransform; 
    }
    else 
    {
        combinedTransform = bone.offsetTransform;
        // Root bone 
    }
    return Math::TransformCoord(Math::Vector3(0.0f, 0.0f, 0.0f), combinedTransform);
}

void setBoneRotation(Bone& bone, const Math::Matrix4& rotationMatrix)
{
    Math::Vector3 position = getBonePosition(bone);
    Math::Matrix4 translationMatrix = Math::Matrix4::Translation(position.x, position.y, position.z);
    bone.toParentTransform = translationMatrix * rotationMatrix;
}

void IKChain::SolveIK()
{
    if (!organizedBones.positionBones.empty()) 
    { 
        Math::Matrix4 inverseRootTransform = (root->toParentTransform).Inverse();
        mLocalTarget = TransformCoord(mTarget, inverseRootTransform); 
    }
    for (size_t i = 0; i < organizedBones.positionBones.size(); ++i)
    { 
        applyPreRotation(organizedBones.positionBones[i], organizedBones.preRotationBones[i]);
    } 
    // Perform IK on position bones 
    for (int iter = 0; iter < mMaxIterations; ++iter)
    {
        bool allBonesAdjusted = true;
        for (int i = organizedBones.positionBones.size() - 1; i >= 0; --i)
        {
            Bone* bone = organizedBones.positionBones.back();
            //endEffectorPosition = TransformCoord(Math::Vector3(0.0f), bone->offsetTransform * (bone->toParentTransform * bone->parent->toParentTransform));
            Math::Vector3 endEffectorPosition = TransformCoord(Math::Vector3(0.0f, 0.0f, 0.0f), bone->toParentTransform * bone->offsetTransform);
            if (Math::Distance(endEffectorPosition, mLocalTarget) < mThreshold)
            {
                return;
                // Target reached 
            }
            
            UpdateBoneRotation(*bone, mLocalTarget);
            // Update positions based on new rotations 
            for (size_t j = i; j < organizedBones.positionBones.size(); ++j)
            {
                Bone* childBone = organizedBones.positionBones[j];
                if (childBone->parent)
                {
                    //childBone->toParentTransform = childBone->offsetTransform * (childBone->toParentTransform * childBone->parent->toParentTransform);
                    childBone->offsetTransform = childBone->offsetTransform * childBone->parent->offsetTransform;
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
    endEffectorPosition = TransformCoord(Math::Vector3(0.0f), endEffector->toParentTransform * endEffector->offsetTransform); 
    Math::Vector3 direction = endEffectorPosition - bonePosition; 

    std::cout << "Bone Position: (" << bonePosition.x << ", " << bonePosition.y << ", " << bonePosition.z << ")" << std::endl; 
    std::cout << "End Effector Position: (" << endEffectorPosition.x << ", " << endEffectorPosition.y << ", " << endEffectorPosition.z << ")" << std::endl; 
    std::cout << "Direction: (" << direction.x << ", " << direction.y << ", " << direction.z << ")" << std::endl;

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
            Math::Vector3 normalizedAxis = Math::Normalize(constrainedRotation);
            Math::Matrix4 rotationMatrix = Math::Matrix4::composeRotation(constrainedRotation, normalizedAxis);
            setBoneRotation(bone, bone.toParentTransform * rotationMatrix);
        } 
    } 
    else 
    { 
        //std::cerr << "Error: Zero-length direction vector encountered." << std::endl; 
    } 
}

