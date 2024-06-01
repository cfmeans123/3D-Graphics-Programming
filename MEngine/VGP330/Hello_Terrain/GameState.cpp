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

    if (mOnTerrain)
    {
        Vector3 pos = mCamera.GetPosition();
        float height = mTerrain.GetHeight(pos);
        pos.y = height + 2.5f;
        mCamera.SetPosition(pos);
    }
}

Math::Matrix4 GetMatrix(const Math::Vector3& position)
{
    const Math::Vector3 l = { 0.0f, 0.0f, 1.0f };
    const Math::Vector3 r = Math::Normalize(Math::Cross(Math::Vector3::YAxis, l));
    const Math::Vector3 u = Math::Normalize(Math::Cross(l, r));
    const float x = -Math::Dot(r, position);
    const float y = -Math::Dot(u, position);
    const float z = -Math::Dot(l, position);

    return
    {
        r.x, u.x, l.x, 0.0f,
        r.y, u.y, l.y, 0.0f,
        r.z, u.z, l.z, 0.0f,
        x,   y,   z,   1.0f
    };
}

void GameState::Initialize()
{
    mCamera.SetPosition({ 0.0f, 0.0f, 5.0f });
    mCamera.SetLookAt({ 0.0f, 5.0f, -9.0f });

    mDirectionalLight.direction = Math::Normalize({ 1.0f, -1.0f, 1.0f });
    mDirectionalLight.ambient = { 0.3f, 0.3f, 0.3f, 1.0f };
    mDirectionalLight.diffuse = { 0.8f, 0.8f, 0.8f, 1.0f };
    mDirectionalLight.specular = { 1.0f, 1.0f, 1.0f, 1.0f };

    {
        Model modelA;
        ModelIO::LoadModel("../../Assets/Models/TechSoldier/Ch44_nonPBR.model", modelA);
        ModelIO::LoadMaterial("../../Assets/Models/TechSoldier/Ch44_nonPBR.model", modelA);
        mRenderGroupA = CreateRenderGroup(modelA);
    }

    mTerrain.Initialize("../../Assets/Images/terrain/heightmap_512x512.raw", 20.0f);
    const Mesh& m = mTerrain.GetMesh();
    mGround.meshBuffer.Initialize(
        nullptr,
        static_cast<uint32_t>(sizeof(Vertex)),
        static_cast<uint32_t>(m.vertices.size()),
        m.indices.data(),
        static_cast<uint32_t>(m.indices.size())
        );
    mGround.meshBuffer.Update(m.vertices.data(), m.vertices.size());
    mGround.diffuseMapID = TextureManager::Get()->LoadTexture("terrain/dirt_seamless.jpg");
    mGround.bumpMapID = TextureManager::Get()->LoadTexture("terrain/grass_2048.jpg");


    std::filesystem::path shaderFilePath = L"../../Assets/Shaders/Standard.fx";
    mStandardEffect.Initialize(shaderFilePath);
    mStandardEffect.SetCamera(mCamera);
    mStandardEffect.SetDirectionalLight(mDirectionalLight);
    mStandardEffect.SetLightCamera(mShadowEffect.GetLightCamera());
    mStandardEffect.SetShadowMap(mShadowEffect.GetDepthMap());

    mTerrainEffect.Initialize();
    mTerrainEffect.SetCamera(mCamera);
    mTerrainEffect.SetDirectionalLight(mDirectionalLight);
    mTerrainEffect.SetLightCamera(mShadowEffect.GetLightCamera());
    mTerrainEffect.SetShadowMap(mShadowEffect.GetDepthMap());

    mShadowEffect.Initialize();
    mShadowEffect.SetDirectionalLight(mDirectionalLight);
}

void GameState::Terminate()
{
    mShadowEffect.Terminate();
    mTerrainEffect.Terminate();
    mStandardEffect.Terminate();
    mGround.Terminate();
    CleanRenderGroup(mRenderGroupA);
}

void GameState::Update(float dt)
{
    CameraControllers(dt);
}

void GameState::Render()
{
    SimpleDraw::AddGroundPlane(10.0f, Colors::White);
    SimpleDraw::Render(mCamera);

    mShadowEffect.SetFocus(mCamera.GetPosition());

    mShadowEffect.Begin();
        DrawRenderGroup(mShadowEffect, mRenderGroupA);
        mShadowEffect.Render(mGround);
    mShadowEffect.End();

    mTerrainEffect.Begin();
    mTerrainEffect.Render(mGround);
    mTerrainEffect.End();

    mStandardEffect.Begin();
        DrawRenderGroup(mStandardEffect, mRenderGroupA);
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

    if (ImGui::CollapsingHeader("Terrain", ImGuiTreeNodeFlags_DefaultOpen))
    {
        ImGui::Checkbox("On Terrain", &mOnTerrain);
        float heightScale = mTerrain.GetHeightScale();
        if (ImGui::DragFloat("HeightScale", &heightScale, 0.1f, 1.0f, 100.0f))
        {
            mTerrain.SetHeightScale(heightScale);
            const Mesh& m = mTerrain.GetMesh();
            mGround.meshBuffer.Update(m.vertices.data(), m.vertices.size());
        }
    }

    mStandardEffect.DebugUI();
    mTerrainEffect.DebugUI();
    mShadowEffect.DebugUI();
    ImGui::End();
}