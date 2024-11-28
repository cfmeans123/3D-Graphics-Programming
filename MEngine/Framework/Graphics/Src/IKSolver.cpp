#include "Precompiled.h"
#include "IKSolver.h"

using namespace MEngine;
using namespace MEngine::Graphics;


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


























//void UpdateBoneRotation(Bone& bone, const Math::Vector3& target) {
//    Math::Vector3 toTarget = Math::Normalize(target - Math::GetTranslation(bone.offsetTransform));
//    Math::Vector3 toEndEffector = Math::Normalize(Math::GetTranslation(bone.toParentTransform) - Math::GetTranslation(bone.offsetTransform));    
//
//    float dotProduct = Math::Dot(toTarget, toEndEffector);
//    dotProduct = Math::Clamp(dotProduct, -1.0f, 1.0f); // Avoid floating-point errors
//
//    float angle = acos(dotProduct);
//    Math::Vector3 rotationAxis = Math::Cross(toEndEffector, toTarget);    
//
//    if ((Math::Vector3::Length(rotationAxis)) > 0.0001f) { // Avoid zero-length axis
//        rotationAxis = Math::Normalize(rotationAxis);
//        Math::Quaternion rotation = Math::Quaternion::CreateFromAxisAngle(rotationAxis, angle);
//
//        //This is expensive as hell, add functionality to prevent calculating rotation matrix from quaternion on update
//        
//        bone.offsetTransform = bone.offsetTransform.MatrixRotationQuaternion(rotation) * bone.offsetTransform;
//    }
//}
//
//void solveIK(std::vector<Bone>& bones, const Math::Vector3& target, int maxIterations, float threshold) {
//    for (int iter = 0; iter < maxIterations; ++iter) {
//        bool allBonesAdjusted = true; for (int i = bones.size() - 1; i >= 0; --i) {
//            UpdateBoneRotation(bones[i], target); // Update positions based on new rotations
//            for (int j = i; j < bones.size(); ++j) 
//            { 
//                if (bones[j].parent)
//                { 
//                    bones[j].[3] = bones[j].parent->transform * glm::vec4(getBonePosition(bones[j].parent->transform * bones[j].invBindPose), 1.0f);
//                } 
//                bones[j].invBindPose = glm::inverse(bones[j].transform); 
//            } // Check if end effector is within the threshold 
//            if (glm::distance(getBonePosition(bones.back().transform * bones.back().invBindPose), target) < threshold) 
//            { 
//                return; 
//                // Target reached 
//            } 
//            allBonesAdjusted = false; 
//        } 
//        if (allBonesAdjusted) 
//        { 
//            break; 
//        } 
//    } 
//}