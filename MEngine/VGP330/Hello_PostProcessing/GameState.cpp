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
    {
        Model modelB;
        ModelIO::LoadModel("../../Assets/Models/DrakeEnemy/CH25_nonPBR.model", modelB);
        ModelIO::LoadMaterial("../../Assets/Models/DrakeEnemy/CH25_nonPBR.model", modelB);
        mRenderGroupB = CreateRenderGroup(modelB);
    }
    
    Mesh groundMesh = MeshBuilder::CreateHorizontalPlane(20, 20, 1.0f);
    mGround.meshBuffer.Initialize(groundMesh);
    mGround.diffuseMapID = TextureManager::Get()->LoadTexture("../../Assets/Images/water/water_texture.jpg");

    MeshPX screenQuad = MeshBuilder::CreateScreenQuad(); 
    mScreenQuad.meshBuffer.Initialize(screenQuad);

    std::filesystem::path shaderFilePath = L"../../Assets/Shaders/Standard.fx";
    //std::filesystem::path shaderFilePath = L"../../Assets/Shaders/CellShading.fx";
    mStandardEffect.Initialize(shaderFilePath);
    mStandardEffect.SetCamera(mCamera);
    mStandardEffect.SetDirectionalLight(mDirectionalLight);

    shaderFilePath = L"../../Assets/Shaders/PostProcessing.fx";
    mPostProcessingEffect.Initialize(shaderFilePath);
    mPostProcessingEffect.SetTexture(&mRenderTarget);
    mPostProcessingEffect.SetTexture(&mCombineTexture, 1);

    GraphicsSystem* gs = GraphicsSystem::Get();
    const uint32_t screenWidth = gs->GetBackBufferWidth();
    const uint32_t screenHeight = gs->GetBackBufferHeight();
    mRenderTarget.Initialize(screenWidth, screenHeight, RenderTarget::Format::RGBA_U8);

    mCombineTexture.Initialize("../../Assets/Images/water/water_texture.jpg");

    //GET THESE GONE,  position is already stored in the renderObjects transform parameter. No need to have it twice
    mPositionA = GetMatrix({ 1.0f, 0.0f, 0.0f });
    mPositionB = GetMatrix({ -1.0f, 0.0f, 0.0f });
}

void GameState::Terminate()
{
    mCombineTexture.Terminate();
    mRenderTarget.Terminate();
    mStandardEffect.Terminate();
    mGround.Terminate();
    mScreenQuad.Terminate();
    mPostProcessingEffect.Terminate();
}

void GameState::Update(float dt)
{
    CameraControllers(dt);
}

void GameState::Render()
{
    mRenderTarget.BeginRender();
        mStandardEffect.Begin();
            DrawRenderGroup(mStandardEffect, mRenderGroupA, mPositionA);
            DrawRenderGroup(mStandardEffect, mRenderGroupB, mPositionB);
            //REMOVE THE POSITION PARAMETER FROM THIS FUNCTION
            mStandardEffect.Render(mGround, Math::Matrix4::Identity);
        mStandardEffect.End();
    mRenderTarget.EndRender();

    SimpleDraw::AddGroundPlane(50, Colors::Gray);

    SimpleDraw::Render(mCamera);

    mPostProcessingEffect.Begin();
    mPostProcessingEffect.Render(mScreenQuad);
    mPostProcessingEffect.End();
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

    ImGui::Separator();
    ImGui::Text("Render Target:");
    ImGui::Image(
        mRenderTarget.GetRawData(),
        { 128, 128 },
        { 0, 0 },
        { 1, 1 },
        { 1, 1, 1, 1 },
        { 1, 1, 1, 1 }
    );

    mPostProcessingEffect.DebugUI();
    mStandardEffect.DebugUI();

    ImGui::End();
}