#include "Precompiled.h"
#include "IKJoint.h"
//
//using namespace MEngine;
//using namespace MEngine::Graphics;
//
//
//IKJoint::IKJoint(Bone* bone, bool isStatic)
//{
//    this->mJoint = bone;
//    if (bone->parent != nullptr)
//    {
//        this->mParent = bone->parent;
//    }
//
//    this->mIsStatic = isStatic;
//    this->mAxis = Math::Vector3(0.0, 0.0, 0.0);
//    this->mHasRotationAxis = false;
//    this->mMinAng = 0;
//    this->mMaxAng = Math::pi * 2;
//
//    this->mCol = Math::Vector4(0.2, 0.2, 0.7, 1);
//
//    if (bone->parent != nullptr)
//    {
//        parent->AddChild(this);
//    }
//}
//
//IKJoint::~IKJoint()
//{
//    if (this->mParent != nullptr)
//    {
//        this->mParent->RemoveChild(this);
//    }
//    for (auto it = this->mChildren.begin(); it != this->mChildren.end(); ++it)
//    {
//        (*it)->RemoveParent();
//    }
//}
//
//
//std::string IKJoint::GetName()
//{
//    return this->mJoint->name;
//}
//
//void IKJoint::SetHingeConstraint(Math::Vector3 axis, float min_ang, float max_ang)
//{
//    this->mAxis = Math::Normalize(this->mAxis);
//    this->mMinAng = min_ang;
//    this->mMaxAng = max_ang;
//    this->mHasRotationAxis = true;
//}
//
//void IKJoint::SetBallConstraint(float min_ang, float max_ang)
//{
//    this->mMinAng = min_ang;
//    this->mMaxAng = max_ang;
//}
//
//void IKJoint::AddChild(Bone* bone)
//{
//    bool found = (std::find(this->mChildren.begin(), this->mChildren.end(), bone) != this->mChildren.end());
//    if (!found)
//    {
//        this->mChildren.push_back(bone);
//    }
//}
//
//void IKJoint::RemoveChild(Bone* bone)
//{
//    this->mChildren.remove(bone);
//}
//
//void IKJoint::RemoveParent()
//{
//    this->mParent = nullptr;
//}
