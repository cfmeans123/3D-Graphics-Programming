#include "Precompiled.h"
#include "IKChain.h"
#include "Skeleton.h"




using namespace MEngine;
using namespace MEngine::Graphics;

IKChain::IKChain()
{
    this->mAnnealingExponent = 0;
    this->mTargetReached = false;

}

IKChain::~IKChain()
{
   /* for (size_t i = 0; i < this->mIKJoints.size() - 1; ++i)
    {
        if (this->mIKJoints[i] != nullptr)
        {
            delete this->mIKJoints[i];
        }
    }*/
}



Bone* IKChain::AddJoint(Bone* bone, bool isStatic)
{
    std::string name = bone->name;

    Bone* ikJoint = bone;
    //Ask about control node purpose in source here: https://github.com/Germanunkol/CCD-IK-Panda3D/blob/master/cpp/ccdik/ikChain.cxx

    this->mIKJoints.push_back(ikJoint);
    this->mIKJoints.back()->SetBallConstraint(-Math::pi * 0.9, Math::pi * 0.9);    
    //ASSERT(!this->mRoot, "IKChain: Root bone was not found!");

    return ikJoint;
}

//Bone* IKChain::GetIKJoint(std::string jointName)
//{
//    for (size_t i = 0; i < this->mIKJoints.size(); ++i)
//    {
//        if (this->mIKJoints[i]->GetName() == jointName)
//        {
//            return this->mIKJoints[i];
//        }
//        return nullptr;
//    }
//}
//
//Bone* IKChain::GetIKJoint(int index)
//{
//    ASSERT(index > this->mIKJoints.size(), "IKChain: Index out of range of joint list.");
//    return this->mIKJoints[index];
//}

void IKChain::UpdateIK(float threshold, int minIterations, int maxIterations)
{
    //run CCD IK solver for current chain
    //this->SolveCCD(threshold, minIterations, maxIterations);

    //apply results
    for (auto it = this->mIKJoints.begin(); it != this->mIKJoints.end(); ++it)
    {
        //this will need to be reviewed later to see if necessary or what transformations will need to take place to apply properly


    }
}

float IKChain::CalculateLength()
{
    float length = 0;
    for (size_t i = 1; i < this->mIKJoints.size(); ++i)
    {
        Bone* b1 = this->mIKJoints[i];
        Bone* b0 = this->mIKJoints[i - 1];
        //As long as the bones are in the same space, we can calculate with them without worry
        Math::Vector3 diff = Math::Matrix4::GetPosition(b1->boneTransform) - Math::Matrix4::GetPosition(b0->boneTransform);
        //find out how to calculate length appropriately in the current transform space
        //calculate position difference here
    }
    return 0.0;
}

Bone* IKChain::GetEndEffector()
{
    
    return this->mIKJoints.back();
}

Math::Vector3 MEngine::Graphics::IKChain::ToBoneSpace(Math::Vector3 target, Math::Matrix4 localTransform, Math::Matrix4 boneTransform)
{    
    Math::Matrix4 b1 = Math::Matrix4::Identity;
    b1 = b1.Translation(target);
    Math::Matrix4 r1 = localTransform.Inverse();
    Math::Matrix4 r2 = boneTransform.Inverse();
    b1 = r1 * r2;
    

    return Math::Matrix4::GetPosition(b1);
}

void swing_twist_decomposition(
    Math::Quaternion rotation,
    Math::Vector3 twist_axis,
    Math::Quaternion& swing,
    Math::Quaternion& twist)
{
    Math::Vector3 ra(rotation.x, rotation.y, rotation.z);
    Math::Vector3 p = Math::projectOntoAxis(ra, twist_axis);
    twist = Math::Quaternion(p.x, p.y, p.z, rotation.w);
    twist = Math::Quaternion::Normalize(twist);
    swing = rotation * Math::Quaternion::Conjugate(twist);
}


namespace
{
    void ComputeNewBoneTransformRecursive(Bone* bone, Bone* parentBone)
    {
        if (bone == nullptr)
        {
            return;
        }

        bone->boneTransform = bone->toParentTransform * parentBone->boneTransform;
        for (Bone* child : bone->children)
        {
            ComputeNewBoneTransformRecursive(child, bone);
        }
    }
}


void IKChain::SolveCCD(float threshold, int minIterations, int maxIterations, ModelID modelID, AnimationUtil::BoneTransforms &boneTransforms, const Animator* animator)
{
    ASSERT(this->mIKJoints.size() > 0, "IKChain: There are no joints stored currently!");    
    Bone* endEffector = this->GetEndEffector();
    this->mTargetReached = false;

    //This line overwrites the bonetransforms from previous CCD cycles, but would be important to base FK animation offsets
    //AnimationUtil::ComputeBoneTransforms(modelID, boneTransforms, animator);

    const Model* model = ModelManager::Get()->GetModel(modelID);
    if (model->skeleton != nullptr)
    {
        for (auto bone : model->skeleton->bones)
        {
            bone->boneTransform = boneTransforms[bone->index];
        }
    }

    for (int i = 0; i < maxIterations; ++i)
    {
        if (i > minIterations)
        {
            
            float err = 0.0f;
            err = Math::Vector3::Length(this->mTarget - Math::Matrix4::GetPosition(this->mEndEffector->boneTransform));
            if (err < threshold)
            {
                this->mTargetReached = true;
                
                break;
            }
        }

        for (size_t j = 0; j < this->mIKJoints.size() - 1; ++j)
        {
            Bone* ikJoint = this->mIKJoints[this->mIKJoints.size() - j - 2];


            //skip static joints - why have this?
            if (ikJoint->GetStatic())
            {
                continue;
            }

            Bone* ikJointBone = ikJoint;
            Bone* parentBone = (ikJoint->parent != nullptr) ? ikJoint->parent : this->mRoot;    

            Math::Vector3 target = mTarget;      
            
            Math::Vector3 ee = Math::Matrix4::GetPosition(mEndEffector->boneTransform);
            Math::Vector3 jointPos = Math::Matrix4::GetPosition(ikJoint->boneTransform);

            Math::Vector3 d1 = target - jointPos;
            Math::Vector3 d2 = ee - jointPos;

            Math::Vector3 cross = Math::Cross(d1, d2); 
            cross = Math::Normalize(cross);
            float l1 = Math::Vector3::Length(cross);
            if ((l1 * l1) < 1e-9)
            {
                continue;
            }
            
            float ang = Math::SignedAngle(Math::Normalize(d1), Math::Normalize(d2), cross);
               

            Math::Quaternion q = Math::Quaternion::Zero;
            q = Math::Quaternion::CreateFromAxisAngle(cross, ang);

            DirectX::XMMATRIX tempMat = DirectX::XMMATRIX(ikJoint->boneTransform._11, ikJoint->boneTransform._12, ikJoint->boneTransform._13, ikJoint->boneTransform._14, 
                                                          ikJoint->boneTransform._21, ikJoint->boneTransform._22, ikJoint->boneTransform._23, ikJoint->boneTransform._24, 
                                                          ikJoint->boneTransform._31, ikJoint->boneTransform._32, ikJoint->boneTransform._33, ikJoint->boneTransform._34, 
                                                          ikJoint->boneTransform._41, ikJoint->boneTransform._42, ikJoint->boneTransform._43, ikJoint->boneTransform._44);

            DirectX::XMVECTOR quatTemp = DirectX::XMQuaternionRotationMatrix(tempMat);
            Math::Quaternion qOld = Math::Quaternion(quatTemp.m128_f32[0], quatTemp.m128_f32[1], quatTemp.m128_f32[2], quatTemp.m128_f32[3]);

            Math::Quaternion q_new = Math::Quaternion::Normalize(q * qOld);

                // Correct rotation for hinge:
            if (ikJoint->GetHasRotationAxis())
            {
                Math::Vector3 myAxisInParentSpace = Math::extractRotationAxis(ikJoint->boneTransform);
                Math::Quaternion swing, twist;
                swing_twist_decomposition(q_new, -myAxisInParentSpace, swing, twist);
                    // Only keep the part of the rotation over the hinge axis:
                q_new = twist;
            }

            Math::Vector3 rot_axis = Math::getNormalizedAxis(q_new);

            float rot_ang = Math::Quaternion::getAngle(q_new);


            float rotAxisLength = Math::Vector3::Length(rot_axis);
            // Valid rotation?
            if ((rotAxisLength * rotAxisLength) > 1e-3 && !std::isnan(rot_ang) and abs(rot_ang) > 0)
            {
                while (rot_ang > Math::pi)
                {
                    rot_ang -= 2 * Math::pi;
                }

                if (abs(rot_ang) > 1e-6 and abs(rot_ang) < Math::pi * 2)
                {

                    if (ikJoint->GetHasRotationAxis() && (rot_axis - ikJoint->GetAxis()).lengthSquared() > 0.5)
                    {
                        rot_ang = std::max(-ikJoint->GetMaxAng(), std::min(-ikJoint->GetMinAng(), rot_ang));
                    }
                    else
                    {
                        rot_ang = std::max(ikJoint->GetMinAng(), std::min(ikJoint->GetMaxAng(), rot_ang));
                    }
                }
                
                q_new.CreateFromAxisAngle(rot_axis, rot_ang);

                float ik_joint_factor = float(j + 1) / float(this->mIKJoints.size() - 1);
                float annealing = pow(ik_joint_factor, this->mAnnealingExponent);
                q_new = qOld + (q_new-qOld) * annealing;

                Math::Matrix4 matTrans = Math::Matrix4::Translation(Math::Matrix4::GetPosition(ikJoint->boneTransform));
                ikJoint->boneTransform = ikJoint->boneTransform.MatrixRotationQuaternion(q_new) * matTrans;
                for (auto bone : ikJoint->children)
                {
                    ComputeNewBoneTransformRecursive(bone, ikJoint);
                }
               
               /* for (auto bone : ikJoint->children)
                {
                    bone->toParentTransform = ikJoint->toParentTransform * ikJoint->boneTransform.MatrixRotationQuaternion(q_new);
                }*/

            }
            
        }
    }
    //for (size_t j = 0; j < this->mIKJoints.size() - 1; ++j)
    //{
    //    Bone* ikJoint = this->mIKJoints[this->mIKJoints.size() - j - 2];
    //    boneTransforms[ikJoint->index] = ikJoint->boneTransform;
    //    for (auto bone : ikJoint->children)
    //    {
    //        ComputeNewBoneTransformRecursive(bone, ikJoint);
    //    }
    //}

    //for (auto bone : mEndEffector->children)
    //{
    //    ComputeNewBoneTransformRecursive(bone, mEndEffector);
    //}

    for (auto bone : model->skeleton->bones)
    {
        boneTransforms[bone->index] = bone->boneTransform;
    }
}
