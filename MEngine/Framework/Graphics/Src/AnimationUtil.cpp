#include "Precompiled.h"
#include "AnimationUtil.h"
#include "SimpleDraw.h"

using namespace MEngine;
using namespace MEngine::Graphics;

namespace
{
	void ComputeBoneTransformRecursive(const Bone* bone, AnimationUtil::BoneTransforms& boneTransforms, const Animator* animator)
	{
		if (bone == nullptr)
		{
			return;
		}
		if (animator != nullptr)
		{
			boneTransforms[bone->index] = animator->GetToParentTransform(bone);
		}
		else
		{
			boneTransforms[bone->index] = bone->toParentTransform;
		}
		if (bone->parent != nullptr)
		{
			boneTransforms[bone->index] = boneTransforms[bone->index] * boneTransforms[bone->parentIndex];
		}
		for (const Bone* child : bone->children)
		{
			ComputeBoneTransformRecursive(child, boneTransforms, animator);
		}
	}
}

void AnimationUtil::ComputeBoneTransforms(ModelID id, BoneTransforms& boneTransforms, const Animator* animator)
{
	const Model* model = ModelManager::Get()->GetModel(id);
	if (model->skeleton != nullptr)
	{
		boneTransforms.resize(model->skeleton->bones.size(), Math::Matrix4::Identity);
		ComputeBoneTransformRecursive(model->skeleton->root, boneTransforms, animator);

	}

}

void AnimationUtil::ApplyBoneOfset(ModelID id, BoneTransforms& boneTransforms)
{
	const Model* model = ModelManager::Get()->GetModel(id);
	if (model != nullptr)
	{
		for (const auto& bone : model->skeleton->bones)
		{
			boneTransforms[bone->index] = bone->offsetTransform * boneTransforms[bone->index];
		}
	}
}

void AnimationUtil::DrawSkeleton(ModelID id, BoneTransforms& boneTransforms)
{
<<<<<<< HEAD
    const Model* model = ModelManager::Get()->GetModel(id);
    if (model->skeleton != nullptr)
    {
        for (auto& bone : model->skeleton->bones)
        {
            if (bone->parent != nullptr)
            {
                const Math::Vector3 posA = Math::GetTranslation(boneTransforms[bone->index]);
                const Math::Vector3 posB = Math::GetTranslation(boneTransforms[bone->parentIndex]);
                SimpleDraw::AddLine(posA, posB, Colors::Blue);
                SimpleDraw::AddSphere(4, 4, 0.03f, posA, Colors::Pink);
            }
        }
    }
}


Math::Vector3 getBonePosition(const Math::Matrix4& transform)
{
    return transform.GetPosition(transform);
}

void setBoneRotation(std::unique_ptr<Bone>& bone, const Math::Matrix4& rotationMatrix)
{
    Math::Vector3 position = getBonePosition(bone->toParentTransform);
    bone->toParentTransform = Math::Matrix4::Translation(position) * rotationMatrix;
}

//*should be properly translated from GLM
//void updateBoneRotation(std::unique_ptr<Bone>& bone, const Math::Vector3& target)
//{
//	Math::Vector3 bonePosition = getBonePosition(bone->toParentTransform);
//	Math::Vector3 toTarget = Math::Normalize(target - bonePosition);
//	Math::Vector3 toEndEffector = Math::Normalize(getBonePosition(bone->toParentTransform * bone->offsetTransform) - bonePosition);
//	float dotProduct = Math::Dot(toTarget, toEndEffector);
//	dotProduct = Math::Clamp(dotProduct, -1.0f, 1.0f);
//	// Avoid floating-point errors 
//	float angle = acos(dotProduct);
//	Math::Vector3 rotationAxis = Math::Cross(toEndEffector, toTarget);
//	if (Math::Vector3::Length(rotationAxis) > 0.0001f)
//	{
//		// Avoid zero-length axis 
//		rotationAxis = Math::Normalize(rotationAxis);
//		Math::Vector3 currentRotation = bone->toParentTransform.getRotation();
//		Math::Vector3 desiredRotation = currentRotation + (rotationAxis * angle).toDegrees();
//		Math::Vector3 constrainedRotation = bone->applyConstraints(desiredRotation);
//		Math::Vector3 deltaRotation = constrainedRotation - currentRotation;
//		//Do rotations need to be compiled into a single matrix?
//
//		Math::Matrix4 rotationMatrix = Math::Matrix4::ComposeRotation(rotationAxis, angle);
//		setBoneRotation(bone, rotationMatrix * bone->toParentTransform);
//	}
//}


//void updateBoneRotation(std::unique_ptr<Bone>& bone, const Vector3& target)
//{
//    Math::Vector3 bonePosition = getBonePosition(bone->toParentTransform);
//    Math::Vector3 toTarget = Math::Normalize(target - bonePosition);
//    Math::Vector3 toEndEffector = Math::Normalize(getBonePosition(bone->toParentTransform * bone->offsetTransform) - bonePosition);
//    float dotProduct = Math::Dot(toTarget, toEndEffector);
//    dotProduct = Math::Clamp(dotProduct, -1.0f, 1.0f);
//    // Avoid floating-point errors 
//    float angle = acos(dotProduct);
//    Math::Vector3 rotationAxis = Math::Cross(toEndEffector, toTarget);
//    if (Math::Vector3::Length(rotationAxis) > 0.0001f)
//    {
//    	// Avoid zero-length axis 
//    	rotationAxis = Math::Normalize(rotationAxis);
//    	Math::Vector3 currentRotation = bone->toParentTransform.getRotation();
//    	Math::Vector3 desiredRotation = currentRotation + (rotationAxis * angle).toDegrees();
//    	Math::Vector3 constrainedRotation = bone->applyConstraints(desiredRotation);
//    	Math::Vector3 deltaRotation = constrainedRotation - currentRotation;
//    	//Do rotations need to be compiled into a single matrix?
//    
//    	Math::Matrix4 rotationMatrix = Math::Matrix4::ComposeRotation(rotationAxis, angle);
//    	setBoneRotation(bone, rotationMatrix * bone->toParentTransform);
//    }
//}

//void AnimationUtil::solveIK(ModelID id, const Math::Vector3& target, int maxIterations, float threshold, int baseIndex, int endIndex)
//{
//    const Model* model = ModelManager::Get()->GetModel(id);
//    std::vector<std::unique_ptr<Bone>>& bones = model->skeleton->bones;
//
//    for (int iter = 0; iter < maxIterations; ++iter)
//    {
//        bool allBonesAdjusted = true;
//        for (int i = endIndex; i >= baseIndex; --i)
//        {
//            Math::Vector3 bonePosition = getBonePosition(bones[i]->toParentTransform);
//            Math::Vector3 endEffectorPosition = getBonePosition(bones[i]->toParentTransform * bones[i]->offsetTransform);
//            Math::Vector3 direction = endEffectorPosition - bonePosition;
//            if (Vector3::Length(direction) > 0.0001f) 
//            {
//                updateBoneRotation(bones[i], target);
//                // Update positions based on new rotations 
//
//                for (int j = i; j < endIndex; ++j)
//                {
//                    if (bones[j]->parent)
//                    {
//                        Math::Vector3 t1 = getBonePosition(bones[j]->parent->toParentTransform * bones[j]->offsetTransform);
//                        Math::Vector4 translate = bones[j]->parent->toParentTransform.multiplyMatrixByVector({ t1.x, t1.y, t1.z, 1.0 });
//                        bones[j]->toParentTransform = bones[j]->toParentTransform.Translation(translate.x, translate.y, translate.z);
//
//                        //bones[j].toParentTransform[3] = bones[j].parent->toParentTransform * glm::vec4(getBonePosition(bones[j].parent->toParentTransform * bones[j].offsetTransform),
//                    }
//                    bones[j]->offsetTransform = bones[j]->toParentTransform.Inverse();
//                }
//                // Check if end effector is within the threshold 
//                if (Math::Distance(getBonePosition(bones.back()->toParentTransform * bones.back()->offsetTransform), target) < threshold)
//                {
//                    return;
//                    // Target reached 
//                }
//                allBonesAdjusted = false;
//            }
//            else
//            {
//
//            }
//        }
//        if (allBonesAdjusted)
//        {
//            break;
//        }
//    }
//}
=======
	const Model* model = ModelManager::Get()->GetModel(id);
	if (model->skeleton != nullptr)
	{
		for (auto& bone : model->skeleton->bones)
		{
			if (bone->parent != nullptr)
			{
				const Math::Vector3 posA = Math::GetTranslation(boneTransforms[bone->index]);
				const Math::Vector3 posB = Math::GetTranslation(boneTransforms[bone->parentIndex]);
				SimpleDraw::AddLine(posA, posB, Colors::Blue);
				SimpleDraw::AddSphere(4, 4, 0.03f, posA, Colors::Pink);
			}
		}
	}
}
>>>>>>> parent of 6b3550b (AnimUtil_Progress_2)
