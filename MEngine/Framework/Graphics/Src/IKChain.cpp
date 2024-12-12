#include "Precompiled.h"
#include "IKChain.h"

using namespace MEngine;
using namespace MEngine::Graphics;

IKChain::IKChain()
{
    this->mAnnealingExponent = 0;
    this->mTargetReached = false;
}

IKChain::~IKChain()
{
    for (size_t i = 0; i < this->mIKJoints.size(); ++i)
    {
        delete this->mIKJoints[i];
    }
}

IKJoint* IKChain::AddJoint(Bone* bone, IKJoint* parentIKJoint, bool isStatic)
{
    std::string name = bone->name;

    IKJoint* ikJoint = new IKJoint( bone, parentIKJoint, isStatic);
    //Ask about control node purpose in source here: https://github.com/Germanunkol/CCD-IK-Panda3D/blob/master/cpp/ccdik/ikChain.cxx

    this->mIKJoints.push_back(ikJoint);

    ASSERT(!this->mRoot, "IKChain: Root bone was not found!");

    return ikJoint;
}

IKJoint* IKChain::GetIKJoint(std::string jointName)
{
    for (size_t i = 0; i < this->mIKJoints.size(); ++i)
    {
        if (this->mIKJoints[i]->GetName() == jointName)
        {
            return this->mIKJoints[i];
        }
        return nullptr;
    }
}

IKJoint* IKChain::GetIKJoint(int index)
{
    ASSERT(index > this->mIKJoints.size(), "IKChain: Index out of range of joint list.");
    return this->mIKJoints[index];
}

void IKChain::UpdateIK(float threshold, int minIterations, int maxIterations)
{
    //run CCD IK solver for current chain
    this->SolveCCD(threshold, minIterations, maxIterations);

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
        IKJoint* b1 = this->mIKJoints[i];
        IKJoint* b0 = this->mIKJoints[i - 1];
        //find out how to calculate length appropriately in the current transform space
        //calculate position difference here
    }
    return 0.0;
}

Bone* IKChain::GetEndEffector()
{
    
    return this->mIKJoints.back()->GetBone();
}

void IKChain::SolveCCD(float threshold, int minIterations, int maxIterations)
{
    ASSERT(this->mIKJoints.size() <= 0, "IKChain: There are no joints stored currently!");
    
    Bone* endEffector = this->GetEndEffector();
    this->mTargetReached = false;
    for (int i = 0; i < maxIterations; ++i)
    {
        if (i > minIterations)
        {
            float err = 0.1;
            //err = (this->target.get_pos( this->root ) - end_effector.get_pos( this->root )).length();
            if (err < threshold)
            {
                this->mTargetReached = true;
                break;
            }
        }

        for (size_t j = 0; j < this->mIKJoints.size(); ++j)
        {
            IKJoint* ikJoint = this->mIKJoints[this->mIKJoints.size() - j - 2];

            //skip static joints - why have this?
            if (ikJoint->GetStatic())
            {
                continue;
            }
            Bone* ikJointBone = ikJoint->GetBone();
            Bone* parentBone = (ikJoint->GetParent() != nullptr) ? ikJoint->GetParent()->GetBone() : this->mRoot;

                // The following is computed in local space.
                // First, get the target's position in the local space of this joint

            //Math::Vector3 target = this->mTarget.GetPosition(inRelationToPosition)


                // Then get the position of this node in local space always (0,0,0) and the 
                // current position of the end effector in current space:
                //LPoint3 pos = LPoint3( 0,0,0 );
            
            //Math::Vector3 ee = end_effector.get_pos(ik_joint_node);

                // Get the direction to the target and the direction to the end effector
                // (all still in local space). These are the two vectors we want to align,
                // i.e. we want to rotate the joint so that the direction to the end effector
                // is the direction to the target.
            //LVector3f d1 = target;
            //LVector3f d2 = ee;

            //// 
            //LVector3f cross = d1.cross(d2).normalized();
            //if (cross.length_squared() < 1e-9)
            //    continue;

            //float ang = d2.normalized().signed_angle_rad(d1.normalized(), cross);
            //LQuaternionf q(0, 0, 0, 0);
            //q.set_from_axis_angle_rad(ang, cross);
            //    // Add this rotation to the current rotation:
            //LQuaternionf q_old = ik_joint_node.get_quat();
            //LQuaternionf q_new = q * q_old;
            //q_new.normalize();

            //    // Correct rotation for hinge:
            //if (ik_joint->get_has_rotation_axis())
            //{
            //    LVector3f my_axis_in_parent_space = ik_joint->get_axis();
            //    LQuaternionf swing, twist;
            //    swing_twist_decomposition(q_new, -my_axis_in_parent_space, swing, twist);
            //        // Only keep the part of the rotation over the hinge axis:
            //    q_new = twist;
            //}

            //LVector3f rot_axis = q_new.get_axis_normalized();

            //float rot_ang = q_new.get_angle_rad();


            //    // Valid rotation?
            //if (rot_axis.length_squared() > 1e-3 && !std::isnan(rot_ang) and abs(rot_ang) > 0)
            //{
            //        // reduce the angle
            //        //rot_ang = rot_ang % float(M_PI*2);
            //        // Force into the correct range, so that -180 < angle < 180:
            //    while (rot_ang > M_PI)
            //    {
            //        rot_ang -= 2 * M_PI;
            //    }

            //    if (abs(rot_ang) > 1e-6 and abs(rot_ang) < M_PI * 2)   // Still necessary?
            //    {
            //        // Clamp rotation angle:
            //        if (ik_joint->get_has_rotation_axis() && (rot_axis - ik_joint->get_axis()).length_squared() > 0.5)
            //            rot_ang = std::max(-ik_joint->get_max_ang(), std::min(-ik_joint->get_min_ang(), rot_ang));
            //        else
            //            rot_ang = std::max(ik_joint->get_min_ang(), std::min(ik_joint->get_max_ang(), rot_ang));
            //    }

            //    q_new.set_from_axis_angle_rad(rot_ang, rot_axis);

            //    float ik_joint_factor = float(j + 1) / float(this->ik_joints.size() - 1);
            //    float annealing = pow(ik_joint_factor, this->annealing_exponent);
            //    q_new = q_old + (q_new - q_old) * annealing;

            //    ik_joint_node.set_quat(q_new);

            
        }
    }
}
