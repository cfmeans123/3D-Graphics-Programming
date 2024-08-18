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

    if (input->IsKeyPressed(KeyCode::SPACE))
    {
        Physics::ParticleActivationData data;
        data.startColor = Colors::Red;
        data.endColor = Colors::Aquamarine;
        data.startScale = { 1.5f, 1.5f, 1.5f };
        data.endScale = {0.1f, 0.1f, 0.1f};
        data.lifeTime = 3.0f;
        data.position = Vector3::Zero;
        data.velocity = { static_cast<float>(std::rand() % 360 - 180) , static_cast<float>(std::rand() % 360 - 180), 0.0f};
        mParticle.Activate(data);
    }
    mParticle.Update(dt);
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

    Mesh mesh = MeshBuilder::CreateSphere(30, 30, 1.0f);
    mParticleRenderObj.meshBuffer.Initialize(mesh);
    mParticle.Initialize();
}
void GameState::Terminate()
{
    mParticle.Terminate();
    mParticleRenderObj.Terminate();
    mStandardEffect.Terminate();
}
void GameState::Update(const float deltaTime)
{
    UpdateCameraControl(deltaTime);

}
void GameState::Render()
{
    SimpleDraw::AddGroundPlane(20.0f, Colors::WhiteSmoke);
    SimpleDraw::Render(mCamera);
    mStandardEffect.Begin();
    if (mParticle.IsActive())
    {
        Physics::CurrentParticleInfo info;
        mParticle.ObtainCurrentInfo(info);
        mParticleRenderObj.transform = info.transform;
        mParticleRenderObj.material.ambient = info.color;
        mParticleRenderObj.material.diffuse = info.color;
        mParticleRenderObj.material.specular = info.color;
        mParticleRenderObj.material.emissive = info.color;
        mStandardEffect.Render(mParticleRenderObj);
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
    mStandardEffect.DebugUI();
    Physics::PhysicsWorld::Get()->DebugUI();


 
    ImGui::End();

    SimpleDraw::Render(mCamera);
}