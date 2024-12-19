#pragma once
#include "Bone.h"
#include "Skeleton.h"
#include "AnimationUtil.h"

namespace MEngine::Graphics
{
    class IKChain {
    public:
        IKChain();
        IKChain(AnimationUtil::BoneTransforms &boneTransforms);
        ~IKChain();

        Bone* AddJoint(Bone* bone, bool isStatic = false);

        void SetRoot(Skeleton* skeleton) { mRoot = skeleton->root; }
        void SetEndEffector(Bone* bone) { mEndEffector = bone; }
        void SetLocalTransform(Math::Matrix4 transform) { mCharacterLocalTransform = transform; }

        size_t GetNumIKJoints() { return this->mIKJoints.size(); }

        void SetAnnealingExponent(int exp) { this->mAnnealingExponent = std::max(exp, 0); }
        int GetAnnealingExponent() { return this->mAnnealingExponent; }

        void SetTarget(Math::Vector3 target) { this->mTarget = target; }

        void UpdateIK(float threshold = 0.02, int minIterations = 0, int maxIterations = 10);
        void SetModelID(int modelID) { this->mModelID = modelID; }

        float CalculateLength();

        Bone* GetEndEffector();

        Math::Vector3 ToBoneSpace(Math::Vector3 target, Math::Matrix4 localTransform, Math::Matrix4 boneTransform);

        //reformat this from the original source formatting?

        std::vector<Bone*> mIKJoints;
        void SolveCCD(float threshold, int minIterations, int maxIterations, ModelID modelID, AnimationUtil::BoneTransforms &boneTransforms, const Animator* animator);

    private:
        int mAnnealingExponent;
        
        //What type should this be?
        Math::Vector3 mTarget;

        int mModelID;
        Bone* mRoot;
        Bone* mEndEffector;
        Math::Matrix4 mCharacterLocalTransform = Math::Matrix4::Identity;


        bool mTargetReached;

    };
    

}