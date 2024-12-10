#pragma once
#include "Bone.h"
#include "AnimationUtil.h"


namespace MEngine::Graphics
{
    using BonePtr = Bone*; 
    using BoneContainer = std::vector<BonePtr>; 
    struct OrganizedBones 
    { 
        std::vector<Bone*> preRotationBones; 
        std::vector<Bone*> positionBones; 
    };

    class IKChain final
    {
    public:
        void Initialize(const Model* model, int baseLinkIndex, int endEffectorIndex, int maxIterations, float threshold, Math::Vector3 target);
        void SolveIK();

    private:

        OrganizedBones organizedBones;
        int mModelID;
        int mBaseLinkIndex;
        int mEndEffectorIndex;       
        Math::Vector3 mTarget;
        int mMaxIterations; 
        float mThreshold;
        
        
        void UpdateBoneRotation(Bone& bone, const Math::Vector3& target);
    };



}