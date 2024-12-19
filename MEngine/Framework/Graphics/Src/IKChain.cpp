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



void IKChain::SolveCCD(float threshold, int minIterations, int maxIterations, ModelID modelID, AnimationUtil::BoneTransforms &boneTransforms, const Animator* animator)
{
    ASSERT(this->mIKJoints.size() > 0, "IKChain: There are no joints stored currently!");    
    Bone* endEffector = this->GetEndEffector();
    this->mTargetReached = false;
    for (int i = 0; i < maxIterations; ++i)
    {
        if (i > minIterations)
        {
            AnimationUtil::ComputeBoneTransforms(modelID, boneTransforms, animator);
            float err = 0.0f;
            err = Math::Vector3::Length(this->mTarget - Math::Matrix4::GetPosition(boneTransforms[this->mEndEffector->index]));
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

                // The following is computed in local space.
                // First, get the target's position in the local space of this joint
                
                
                //**If the position we need is local to the parent bone, then override ToParentTransform
                //multiply the transform coordinate (target) by the inverse matrix to transform something into that joints space
                //renderObject transform matrix 4 is the 
                
                //from Local space to Bone space
                //multiply by the inverse matrix of the renderObject and then by the inverse matrix of the Bone (BoneTransform)


            //this is either the BoneTransform or the toParentOffset

            Math::Vector3 target = ToBoneSpace(mTarget, mCharacterLocalTransform, ikJoint->offsetTransform);


                // Then get the position of this node in local space always (0,0,0) and the 
                // current position of the end effector in current space:
                //LPoint3 pos = LPoint3( 0,0,0 );
            
            Math::Vector3 ee = Math::Matrix4::GetPosition(mEndEffector->offsetTransform);

                // Get the direction to the target and the direction to the end effector
                // (all still in local space). These are the two vectors we want to align,
                // i.e. we want to rotate the joint so that the direction to the end effector
                // is the direction to the target.
            Math::Vector3 d1 = target;
            Math::Vector3 d2 = ee;

            //// 
            Math::Vector3 cross = Math::Cross(d1, d2); //d1.cross(d2).normalized();
            //cross = Math::Vector3(Math::Abs(cross.x), Math::Abs(cross.y), Math::Abs(cross.z));
            cross = Math::Normalize(cross);
            float l1 = Math::Vector3::Length(cross);
            if ((l1 * l1) < 1e-9)
            {
                continue;
            }
            
            //if (cross.length_squared() < 1e-9)
            //    continue;
            
            float ang = Math::SignedAngle(Math::Normalize(d1), Math::Normalize(d2), cross);
               

            Math::Quaternion q = Math::Quaternion::Zero;
            q = Math::Quaternion::CreateFromAxisAngle(cross, ang);

            //DirectX::XMQuaternionRotationMatrix()
            DirectX::XMMATRIX tempMat = DirectX::XMMATRIX(ikJoint->boneTransform._11, ikJoint->boneTransform._12, ikJoint->boneTransform._13, ikJoint->boneTransform._14, 
                                                          ikJoint->boneTransform._21, ikJoint->boneTransform._22, ikJoint->boneTransform._23, ikJoint->boneTransform._24, 
                                                          ikJoint->boneTransform._31, ikJoint->boneTransform._32, ikJoint->boneTransform._33, ikJoint->boneTransform._34, 
                                                          ikJoint->boneTransform._41, ikJoint->boneTransform._42, ikJoint->boneTransform._43, ikJoint->boneTransform._44);

            DirectX::XMVECTOR quatTemp = DirectX::XMQuaternionRotationMatrix(tempMat);
            Math::Quaternion qOld = Math::Quaternion(quatTemp.m128_f32[0], quatTemp.m128_f32[1], quatTemp.m128_f32[2], quatTemp.m128_f32[3]);
            //LQuaternionf q(0, 0, 0, 0);
            //q.set_from_axis_angle_rad(ang, cross);
            //    //Add this rotation to the current rotation:
            //LQuaternionf q_old = ik_joint_node.get_quat();
            Math::Quaternion q_new = Math::Quaternion::Normalize(q * qOld);
            //q_new.normalize();

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
            //    // Valid rotation?
            if ((rotAxisLength * rotAxisLength) > 1e-3 && !std::isnan(rot_ang) and abs(rot_ang) > 0)
            {
                // reduce the angle
                //rot_ang = rot_ang % float(M_PI*2);
                // Force into the correct range, so that -180 < angle < 180:
                while (rot_ang > Math::pi)
                {
                    rot_ang -= 2 * Math::pi;
                }

                if (abs(rot_ang) > 1e-6 and abs(rot_ang) < Math::pi * 2)   // Still necessary?
                {
                    // Clamp rotation angle:
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

                ikJoint->offsetTransform = ikJoint->offsetTransform * ikJoint->boneTransform.MatrixRotationQuaternion(q_new);
                ikJoint->toParentTransform = ikJoint->toParentTransform * ikJoint->boneTransform.MatrixRotationQuaternion(q_new);
                for (auto bone : ikJoint->children)
                {
                    //bone->offsetTransform = ikJoint->offsetTransform * ikJoint->boneTransform.MatrixRotationQuaternion(q_new);
                    bone->toParentTransform = ikJoint->toParentTransform * ikJoint->boneTransform.MatrixRotationQuaternion(q_new);
                }
                    //.set_quat(q_new);

            }
            
        }
    }
}
