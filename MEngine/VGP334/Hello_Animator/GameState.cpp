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

    mModelID = ModelManager::Get()->LoadModel("../../Assets/Models/DrakeEnemy/Ch25_nonPBR.model");
    ModelManager::Get()->AddAnimation(mModelID, "../../Assets/Models/DrakeEnemy/Animations/armada.animset");
    ModelManager::Get()->AddAnimation(mModelID, "../../Assets/Models/DrakeEnemy/Animations/Au_To_Role.animset");
    ModelManager::Get()->AddAnimation(mModelID, "../../Assets/Models/DrakeEnemy/Animations/Chapa_2.animset");
    ModelManager::Get()->AddAnimation(mModelID, "../../Assets/Models/DrakeEnemy/Animations/Capoeira.animset");
    mCharacter = CreateRenderGroup(mModelID, &mCharacterAnimator);
    mCharacterAnimator.Initialize(mModelID);
    mCharacterAnimator.SetNodeAnimation(BlendDirection::Idle, 0);
    mCharacterAnimator.SetNodeAnimation(BlendDirection::Forward, 1);
    mCharacterAnimator.SetNodeAnimation(BlendDirection::Left, 2);


    mStandardEffect.Initialize(L"../../Assets/Shaders/Standard.fx");
    mStandardEffect.SetCamera(mCamera);
    mStandardEffect.SetDirectionalLight(mDirectionalLight);

}

void GameState::Terminate()
{
    CleanRenderGroup(mCharacter);
    mStandardEffect.Terminate();
}

void GameState::Update(const float deltaTime)
{
    mCharacterAnimator.Update(deltaTime);

    UpdateCameraControl(deltaTime);
}

void GameState::Render()
{
    SimpleDraw::AddGroundPlane(10.0f, Colors::White);
    SimpleDraw::Render(mCamera);

    mStandardEffect.Begin();
    if (mDrawSkeleton)
    {
        Matrix4 transform = mCharacter[0].transform.GetMatrix4();
        AnimationUtil::BoneTransforms boneTransforms;
        AnimationUtil::ComputeBoneTransforms(mModelID, boneTransforms, &mCharacterAnimator);
        AnimationUtil::DrawSkeleton(mModelID, boneTransforms);
        for (auto& boneTransform : boneTransforms)
        {
            boneTransform = boneTransform * transform;
        }
    }
    else
    {
        DrawRenderGroup(mStandardEffect, mCharacter);
    }
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
    ImGui::Checkbox("DrawSkeleton", &mDrawSkeleton);
    if (ImGui::DragInt("AnimClip", &mAnimIndex, 1, -1, mCharacterAnimator.GetAnimationCount() - 1))
    {
        mCharacterAnimator.PlayAnimation(mAnimIndex, true, mBlendDuration);
    }

    if (ImGui::DragFloat("BlendDuration", &mBlendDuration, 0.1f, 0.0f, 3.0f))
    if (ImGui::DragFloat("BlendWeightIdle", &mBlendWeightIdle, 0.1f, 0.0f, 1.0f))
    {
        mCharacterAnimator.SetBlendWeight(BlendDirection::Idle, mBlendWeightIdle);
    }
    if (ImGui::DragFloat("BlendWeightForward", &mBlendWeightForward, 0.1f, 0.0f, 1.0f))
    {
        mCharacterAnimator.SetBlendWeight(BlendDirection::Forward, mBlendWeightForward);
    }
    if (ImGui::DragFloat("BlendWeightLeft", &mBlendWeightLeft, 0.1f, 0.0f, 1.0f))
    {
        mCharacterAnimator.SetBlendWeight(BlendDirection::Left, mBlendWeightLeft);
    }
    if (ImGui::BeginCombo("Bones",
        ModelManager::Get()->GetModel(mModelID)->skeleton.get()->bones.front().get()->name.c_str(), ImGuiTreeNodeFlags_DefaultOpen))
    {
        for (int i = 0; i < ModelManager::Get()->GetModel(mModelID)->skeleton.get()->bones.size(); ++i)
        {
            const bool isSelected = (this->selectedIndex == i);
            if (ImGui::Selectable((ModelManager::Get()->GetModel(mModelID)->skeleton.get()->bones.at(i).get()->name.c_str()), isSelected)) {
                selectedIndex = i;
            }

            if (isSelected)
            {
                ImGui::SetItemDefaultFocus();
            }
        }
    ImGui::EndCombo();
    }

    //if (ImGui::ListBox("Joints", 3, ))
        
    mStandardEffect.DebugUI();
    ImGui::End();
}