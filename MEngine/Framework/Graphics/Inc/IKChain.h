#pragma once
#include "IKJoint.h"
#include "Bone.h"

namespace MEngine::Graphics
{
    class IKChain {
    public:
        IKChain();
        ~IKChain();

        IKJoint* AddJoint(Bone* bone, IKJoint* parentIKJoint = nullptr, bool isStatic = false);

        IKJoint* GetIKJoint(std::string jointName);
        IKJoint* GetIKJoint(int index);

        size_t GetNumIKJoints() { return this->mIKJoints.size(); }

        void SetAnnealingExponent(int exp) { this->mAnnealingExponent = std::max(exp, 0); }
        int GetAnnealingExponent() { return this->mAnnealingExponent; }

        void SetTarget(Math::Vector3 target) { this->mTarget = target; }

        void UpdateIK(float threshold = 0.02, int minIterations = 0, int maxIterations = 10);

        float CalculateLength();

        Bone* GetEndEffector();

        //reformat this from the original source formatting?

    private:
        int mAnnealingExponent;
        
        //What type should this be?
        Math::Vector3 mTarget;


        Bone* mRoot;
        Bone* mEndEffector;

        bool mTargetReached;



        std::vector<IKJoint*> mIKJoints;
        
        void SolveCCD(float threshold = 0.02, int minIterations = 1, int maxIterations = 10);

    };
    

}