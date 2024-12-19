#include "Precompiled.h"
#include "Bone.h"

using namespace MEngine;
using namespace MEngine::Graphics;

void Bone::SetHingeConstraint(Math::Vector3 axis, float min_ang, float max_ang)
{
    this->mAxis = Math::Normalize(axis);
    this->mMinAng = min_ang;
    this->mMaxAng = max_ang;
    this->mHasRotationAxis = true;
}

void Bone::SetBallConstraint(float min_ang, float max_ang)
{
    this->mMinAng = min_ang;
    this->mMaxAng = max_ang;
}
