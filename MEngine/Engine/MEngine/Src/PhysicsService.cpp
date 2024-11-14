#include "Precompiled.h"
#include "PhysicsService.h"

#include "SaveUtil.h"

#include "RigidBodyComponent.h"

using namespace MEngine;
using namespace MEngine::Physics;

void PhysicsService::Update(float deltaTime)
{
    if (mEnabled)
    {
        PhysicsWorld::Get()->Update(deltaTime);
    }
}

void PhysicsService::DebugUI()
{
    if (mEnabled)
    {
        PhysicsWorld::Get()->DebugUI();
    }
}

void PhysicsService::Serialize(rapidjson::Document& doc, rapidjson::Value& value)
{
    PhysicsWorld::Settings settings = PhysicsWorld::Get()->GetSettings();
    rapidjson::Value serviceValue(rapidjson::kObjectType);
    SaveUtil::SaveVector3("Gravity", settings.gravity, doc, serviceValue);
    SaveUtil::SaveFloat("SimSteps", settings.simulationStep, doc, serviceValue);
    SaveUtil::SaveFloat("FixedTimeStep", settings.fixedTimeStep, doc, serviceValue);
    value.AddMember("PhysicsService", serviceValue, doc.GetAllocator());
}

void PhysicsService::Deserialize(const rapidjson::Value& value)
{
    PhysicsWorld::Settings settings = PhysicsWorld::Get()->GetSettings();
    if (value.HasMember("Gravity"))
    {
        const auto& gravity = value["Gravity"].GetArray();
        settings.gravity.x = gravity[0].GetFloat();
        settings.gravity.y = gravity[1].GetFloat();
        settings.gravity.z = gravity[2].GetFloat();
    }
    if (value.HasMember("SimSteps"))
    {
        settings.simulationStep = value["SimSteps"].GetFloat();
    }
    if (value.HasMember("FixedTimeStep"))
    {
        settings.fixedTimeStep = value["FixedTimeStep"].GetFloat();
    }
    PhysicsWorld::Get()->UpdateSettings(settings);
}

void PhysicsService::Register(RigidBodyComponent* rigidBodyComponent)
{
    PhysicsWorld::Get()->Register(&rigidBodyComponent->mRigidBody);
}

void PhysicsService::Unregister(RigidBodyComponent* rigidBodyComponent)
{
    PhysicsWorld::Get()->Unregister(&rigidBodyComponent->mRigidBody);
}

void PhysicsService::SetEnabled(bool enabled)
{
    mEnabled = enabled;
}
