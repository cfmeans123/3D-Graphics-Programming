#pragma once

#include "Animator.h"

//Consolidate all the IKSolver code into this class here and get working first

struct BlendNode
{
    int animIndex = -1;
    bool isLooping = false;
    float animationTime = 0.0f;
    float blendWeight = 0.0f;
};

enum class BlendDirection
{
    Idle,
    Forward,
    Run,
    Left,
    Right,
    Back
};

class IKAnimator : public Animator
{
public:
    void PlayAnimation(int clipIndex, bool looping) override;
    void PlayAnimation(int clipIndex, bool looping, float transitionTime);
    void Update(float deltaTime) override;

    bool IsFinished() const override;
    Math::Matrix4 GetToParentTransform(const Bone* bone) const override;

    void SetNodeAnimation(BlendDirection dir, int clipIndex);
    void SetBlendWeight(BlendDirection dir, float weight);
private:
    float mBlendDuration = 0.0f;
    float mBlendTime = 0.0f;
    BlendNode mCurrentAnimation;
    BlendNode mNextAnimation;

    using BlendNodes = std::unordered_map<BlendDirection, BlendNode>;
    BlendNodes mBlendNodes;
};