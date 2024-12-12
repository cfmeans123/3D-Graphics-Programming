#pragma once
#include "Bone.h"
#include "AnimationUtil.h"

namespace MEngine::Graphics
{
    class IKJoint
    {
    public:
        IKJoint(Bone* bone, IKJoint* parent= nullptr, bool isStatic = false);
        ~IKJoint();


        void SetStatic(bool is_static = true) { this->mIsStatic; }
        bool GetStatic() { return this->mIsStatic; }
        
        std::string GetName();

        void SetHingeConstraint( Math::Vector3 axis, float min_ang = -(Math::pi), float max_ang = Math::pi);
        //Note: roll axis is not controlled for ball joints, i.e.it might rotate around the roll axis uncontrollably.

        void SetBallConstraint(float min_ang = -(Math::pi), float max_ang = Math::pi);
        bool GetHasRotationAxis() { return this->mHasRotationAxis; }
        Math::Vector3 GetAxis() { return this->mAxis; }

        float GetMinAng() { return this->mMinAng; }
        float GetMaxAng() { return this->mMaxAng; }

        Bone* GetBone() { return this->mJoint; }

        Math::Vector4 GetCol() { return this->mCol; }

        IKJoint* GetParent() { return this->mParent; }

        void AddChild(IKJoint* bone);
        void RemoveChild(IKJoint* bone);
        void RemoveParent();

    private:
        Bone* mJoint;
        IKJoint* mParent;
        Math::Vector3 mAxis;
        float mMinAng;
        float mMaxAng;
        bool mIsStatic;
        bool mHasRotationAxis;
        
        //some representation of position in local space
        //Vector3?
        //Extend Bone with GetWorldPosition function?

        std::list<IKJoint*> mChildren;

        Math::Vector4 mCol;
    };


}