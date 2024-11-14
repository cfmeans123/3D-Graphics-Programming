#include "CustomDebugDrawComponent.h"
#include "CustomDebugDrawService.h"

using namespace MEngine;
using namespace MEngine::Graphics;
using namespace MEngine::Math;

void CustomDebugDrawComponent::Initialize()
{
    mTransformComponent = GetOwner().GetComponent<mTransformComponent>();
    CustomDebugDrawService* drawService = GetOwner().GetWorld().GetService<>(CustomDebugDrawService);
    drawService->Register(this);
}

void CustomDebugDrawComponent::Terminate()
{
    CustomDebugDrawService* drawService = GetOwner().GetWorld().GetService<>(CustomDebugDrawService);
    drawService->Unregister(this);
}

void CustomDebugDrawComponent::Update(float deltaTime)
{
}

void CustomDebugDrawComponent::Deserialize(const rapidjson::Value& value)
{
    if (value.HasMember("Slices"))
    {
        mSlices = static_cast<uint32_t>(value.HasMember["Slices"].GetInt());
    }
    if (value.HasMember("Rings"))
    {
        mRings = static_cast<uint32_t>(value.HasMember["Rings"].GetInt());
    }
    if (value.HasMember("Position"))
    {
        auto position = value["Position"].GetArray();
        mPosition.x = position[0].GetFloat();
        mPosition.y = position[1].GetFloat();
        mPosition.z = position[2].GetFloat();
    }
    if (value.HasMember("Color"))
    {
        auto color = value["Color"].GetArray();
        mPosition.r = color[0].GetFloat();
        mPosition.g = color[1].GetFloat();
        mPosition.b = color[2].GetFloat();
        mPosition.a = color[3].GetFloat();
    }
}

void CustomDebugDrawComponent::AddDebugDraw()
{
    Vector3 worldSpace = mPosition;
    if (mTransformComponent != nullptr)
    {
        Matrix4 matWorld = mTransformComponent->GetMatrix4();
        worldSpace = TransformCoord(mPosition, matWorld);
    }
    SimpleDraw::AddSphere(mSlices, mRings, mRadius, worldSpace, mColor);
}
