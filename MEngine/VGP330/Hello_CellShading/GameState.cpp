#include "GameState.h"

using namespace MEngine;
using namespace MEngine::Graphics;
using namespace MEngine::Input;

void GameState::CameraControllers(float dt)
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
    mCamera.SetPosition({ 0.0f, 0.0f, 5.0f });
    mCamera.SetLookAt({ 0.0f, 5.0f, -9.0f });

    mDirectionalLight.direction = Math::Normalize({ 1.0f, -1.0f, 1.0f });
    mDirectionalLight.ambient = { 0.3f, 0.3f, 0.3f, 1.0f };
    mDirectionalLight.diffuse = { 0.8f, 0.8f, 0.8f, 1.0f };
    mDirectionalLight.specular = { 1.0f, 1.0f, 1.0f, 1.0f };

    TextureManager* tm = TextureManager::Get();
    mMesh = MeshBuilder::CreateSphere(100, 100, 1.0f);
    mRenderObject.meshBuffer.Initialize(mMesh);
    mRenderObject.diffuseTextureMapId = tm->LoadTexture("materials/Stone/TCom_Wall_Stone3_2x2_4K_albedo.tif");
    mRenderObject.specTextureMapId = tm->LoadTexture("materials/Stone/TCom_Wall_Stone3_2x2_4K_ao.tif");
    mRenderObject.normalTextureMapId = tm->LoadTexture("materials/Stone/TCom_Wall_Stone3_2x2_4K_normal.tif");
    mRenderObject.bumpTextureMapId = tm->LoadTexture("materials/Stone/TCom_Wall_Stone3_2x2_4K_height.tif");

    std::filesystem::path shaderFilePath = L"../../Assets/Shaders/CellShader.fx";
    mStandardEffect.Initialize(shaderFilePath);
    mStandardEffect.SetCamera(mCamera);
    mStandardEffect.SetDirectionalLight(mDirectionalLight);
}

void GameState::Terminate()
{
    mStandardEffect.Terminate();
    mRenderObject.Terminate();
}

void GameState::Update(float dt)
{
    CameraControllers(dt);
}

void GameState::Render()
{
    mStandardEffect.Begin();
    mStandardEffect.Render(mRenderObject);
    mStandardEffect.End();
}

void GameState::DebugUI()
{
    ImGui::Begin("Debug Controls", nullptr, ImGuiWindowFlags_AlwaysAutoResize);

    if (ImGui::CollapsingHeader("Light", ImGuiTreeNodeFlags_DefaultOpen))
    {
        if (ImGui::DragFloat3("Direction", &mDirectionalLight.direction.x, 0.01f))
        {
            mDirectionalLight.direction = Math::Normalize(mDirectionalLight.direction);
        }

        ImGui::ColorEdit4("Ambient##Light", &mDirectionalLight.ambient.r);
        ImGui::ColorEdit4("Diffuse##Light", &mDirectionalLight.diffuse.r);
        ImGui::ColorEdit4("Specular##Light", &mDirectionalLight.specular.r);
    }

    if (ImGui::CollapsingHeader("Material", ImGuiTreeNodeFlags_DefaultOpen))
    {
        ImGui::ColorEdit4("Ambient##Material", &mRenderObject.material.ambient.r);
        ImGui::ColorEdit4("Diffuse##Material", &mRenderObject.material.diffuse.r);
        ImGui::ColorEdit4("Specular##Material", &mRenderObject.material.specular.r);
        ImGui::ColorEdit4("Emissive##Material", &mRenderObject.material.emissive.r);

        ImGui::DragFloat("Power##Material", &mRenderObject.material.power, 1.0f, 1.0f, 100.0f);
    }

    mStandardEffect.DebugUI();

    ImGui::End();
}

