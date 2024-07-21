#include "GameState.h"

using namespace MEngine;
using namespace MEngine::Graphics;
using namespace MEngine::Math;
using namespace MEngine::Input;

void GameState::UpdateCameraControl(float dt)
{
    auto input = Input::InputSystem::Get();
    const float moveSpeed = input->IsKeyDown(KeyCode::LSHIFT) ? 10.0f : 1.0f;
    const float turnSpeed = 0.8f;

    if (input->IsKeyDown(KeyCode::W))
    {
        mCamera.Walk(moveSpeed * dt);
    }
    if (input->IsKeyDown(KeyCode::A))
    {
        mCamera.Strafe(-moveSpeed * dt);
    }
    if (input->IsKeyDown(KeyCode::S))
    {
        mCamera.Walk(-moveSpeed * dt);
    }
    if (input->IsKeyDown(KeyCode::D))
    {
        mCamera.Strafe(moveSpeed * dt);
    }
    if (input->IsKeyDown(KeyCode::E))
    {
        mCamera.Rise(moveSpeed * dt);
    }
    if (input->IsKeyDown(KeyCode::Q))
    {
        mCamera.Rise(-moveSpeed * dt);
    }

    if (input->IsMouseDown(MouseButton::LBUTTON))
    {
        mCamera.Yaw(input->GetMouseMoveX() * turnSpeed * dt);
        mCamera.Pitch(input->GetMouseMoveY() * turnSpeed * dt);
    }
}

void GameState::Initialize()
{
    mCamera.SetPosition({ 0.0f,1.0f,-4.0f });
    mCamera.SetLookAt({ 0.0f,0.0f,0.0f });

    mDirectionalLight.direction = Math::Normalize({ 1.0f,-1.0f,1.0f });
    mDirectionalLight.ambient = { 0.5f,0.5f,0.5f,1.0f };
    mDirectionalLight.diffuse = { 0.8f,0.8f,0.8f,1.0f };
    mDirectionalLight.specular = { 1.0f,1.0f,1.0f,1.0f };

    mStandardEffect.Initialize(L"../../Assets/Shaders/Standard.fx");
    mStandardEffect.SetCamera(mCamera);
    mStandardEffect.SetDirectionalLight(mDirectionalLight);

    Model modelA;
    ModelIO::LoadModel("../../Assets/Models/TechSoldier/Ch44_nonPBR.model", modelA);
    ModelIO::LoadMaterial("../../Assets/Models/TechSoldier/Ch44_nonPBR.model", modelA);
    mCharacter = CreateRenderGroup(modelA);

}
void GameState::Terminate()
{
    CleanRenderGroup(mCharacter);
    mStandardEffect.Terminate();
}
void GameState::Update(const float deltaTime)
{
    UpdateCameraControl(deltaTime);
}
void GameState::Render()
{
    mStandardEffect.Begin();
    DrawRenderGroup(mStandardEffect, mCharacter);
    mStandardEffect.End();
}
void GameState::DebugUI()
{
    ImGui::Begin("Debug control", nullptr, ImGuiWindowFlags_AlwaysAutoResize);
    if (ImGui::CollapsingHeader("Light", ImGuiTreeNodeFlags_DefaultOpen))
    {
        if (ImGui::DragFloat3("Direction", &mDirectionalLight.direction.x, 0.1f))
        {
            mDirectionalLight.direction = Math::Normalize(mDirectionalLight.direction);
        }

        ImGui::ColorEdit4("Ambient##Light", &mDirectionalLight.ambient.r);
        ImGui::ColorEdit4("Diffuse##Light", &mDirectionalLight.diffuse.r);
        ImGui::ColorEdit4("Specular##Light", &mDirectionalLight.specular.r);
    }
    if (ImGui::CollapsingHeader("Quaternion", ImGuiTreeNodeFlags_DefaultOpen))
    {
        if (ImGui::DragFloat("Yaw", &mYaw, 0.01f))
        {
            Quaternion rot = Quaternion::CreateFromYawPitchRoll(mYaw, mPitch, mRoll);
            for (auto& r : mCharacter)
            {
                r.transform.rotation = rot;
            }
        }
        if (ImGui::DragFloat("Pitch", &mPitch, 0.01f))
        {
            Quaternion rot = Quaternion::CreateFromYawPitchRoll(mYaw, mPitch, mRoll);
            for (auto& r : mCharacter)
            {
                r.transform.rotation = rot;
            }
        }
        if (ImGui::DragFloat("Roll", &mRoll, 0.01f))
        {
            Quaternion rot = Quaternion::CreateFromYawPitchRoll(mYaw, mPitch, mRoll);
            for (auto& r : mCharacter)
            {
                r.transform.rotation = rot;
            }
        }
    }
    ImGui::End();
}