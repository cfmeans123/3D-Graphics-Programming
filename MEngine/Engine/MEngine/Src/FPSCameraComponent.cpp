#include "Precompiled.h"
#include "FPSCameraComponent.h"

#include "GameObject.h"
#include "CameraComponent.h"

using namespace MEngine;
using namespace MEngine::Graphics;
using namespace MEngine::Input;

void MEngine::FPSCameraComponent::Initialize()
{
	mCameraComponent = GetOwner().GetComponent<CameraComponent>();
	ASSERT(mCameraComponent != nullptr, "FPSCameraComponent: Camera not found");
}

void MEngine::FPSCameraComponent::Terminate()
{
    mCameraComponent = nullptr;
}

void MEngine::FPSCameraComponent::Update(float deltaTime)
{
    Camera& camera = mCameraComponent->GetCamera();

    auto input = Input::InputSystem::Get();
    const float moveSpeed = input->IsKeyDown(KeyCode::LSHIFT) ? 10.0f : 1.0f;
    const float turnSpeed = 0.8f;

    if (input->IsKeyDown(KeyCode::W))
    {
        camera.Walk(moveSpeed * deltaTime);
    }
    if (input->IsKeyDown(KeyCode::A))
    {
        camera.Strafe(-moveSpeed * deltaTime);
    }
    if (input->IsKeyDown(KeyCode::S))
    {
        camera.Walk(-moveSpeed * deltaTime);
    }
    if (input->IsKeyDown(KeyCode::D))
    {
        camera.Strafe(moveSpeed * deltaTime);
    }
    if (input->IsKeyDown(KeyCode::E))
    {
        camera.Rise(moveSpeed * deltaTime);
    }
    if (input->IsKeyDown(KeyCode::Q))
    {
        camera.Rise(-moveSpeed * deltaTime);
    }

    if (input->IsMouseDown(MouseButton::LBUTTON))
    {
        camera.Yaw(input->GetMouseMoveX() * turnSpeed * deltaTime);
        camera.Pitch(input->GetMouseMoveY() * turnSpeed * deltaTime);
    }
}
