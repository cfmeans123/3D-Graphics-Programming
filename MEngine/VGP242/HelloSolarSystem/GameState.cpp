#include "GameState.h"
#include <iostream>
using namespace MEngine;
using namespace MEngine::Graphics;
using namespace MEngine::Input;


namespace
{
    void CameraControllers(float dt, Camera& camera)
    {
        auto input = Input::InputSystem::Get();
        const float moveSpeed = input->IsKeyDown(KeyCode::LSHIFT) ? 10.0f : 1.0f;
        const float turnSpeed = 0.8f;

        if (input->IsKeyDown(KeyCode::W))
        {
            camera.Walk(moveSpeed * dt);
        }
        if (input->IsKeyDown(KeyCode::A))
        {
            camera.Strafe(-moveSpeed * dt);
        }
        if (input->IsKeyDown(KeyCode::S))
        {
            camera.Walk(-moveSpeed * dt);
        }
        if (input->IsKeyDown(KeyCode::D))
        {
            camera.Strafe(moveSpeed * dt);
        }
        if (input->IsKeyDown(KeyCode::E))
        {
            camera.Rise(moveSpeed * dt);
        }
        if (input->IsKeyDown(KeyCode::Q))
        {
            camera.Rise(-moveSpeed * dt);
        }

        if (input->IsMouseDown(MouseButton::LBUTTON))
        {
            camera.Yaw(input->GetMouseMoveX() * turnSpeed * dt);
            camera.Pitch(input->GetMouseMoveY() * turnSpeed * dt);
        }
    }

    void CheackStates()
    {

    }

    const char* shapeTypes[] =
    {
        "None",
        "Sphere",
        "AABB",
        "FilledAABB"
    };
}

void MilkyWay::Initialize()
{
    mCamera.SetPosition({ 0.0f, 10.0f, -20.0f });
    mCamera.SetLookAt({ 0.0f, 5.0f, -9.0f });

    DebugUI::SetTheme(DebugUI::Theme::Dark);

    std::filesystem::path shaderFile = L"../../Assets/Shaders/DoTexturing.fx";

    mConstantBuffer.Initialize(sizeof(Math::Matrix4));
    mVertexShader.Initialize<VertexPX>(shaderFile);
    mPixelShader.Initialize(shaderFile);
    mSampler.Initialize(Sampler::Filter::Linear, Sampler::AddressMode::Wrap);

    float distance = 0;
    float sizeOffset = 10;
    

    //Hooray for hard coding!
    mOrbs.push_back(SpaceOrb("../../Assets/Images/planets/sun.jpg", sizeOffset * 0.1, 0, SpaceOrb::OrbName::Sun));
    mOrbs.push_back(SpaceOrb("../../Assets/Images/planets/mercury.jpg", 0.035f, sizeOffset + 0.39f, SpaceOrb::OrbName::Mercury));
    std::cout << "mercury offset" << sizeOffset << std::endl;
    
    sizeOffset +=  0.39;
    mOrbs.push_back(SpaceOrb("../../Assets/Images/planets/venus.jpg", 0.086f, sizeOffset + 0.72f, SpaceOrb::OrbName::Venus));
    std::cout << "Venus offset" << sizeOffset << std::endl;
    soVenus = sizeOffset;
    sizeOffset += 0.72f;
    mOrbs.push_back(SpaceOrb("../../Assets/Images/planets/earth/earth.jpg", 0.091f, sizeOffset + 1, SpaceOrb::OrbName::Earth));
    std::cout << "Earth offset" << sizeOffset << std::endl;
    soEarth = sizeOffset;
    sizeOffset += 1;
    mOrbs.push_back(SpaceOrb("../../Assets/Images/planets/mars.jpg", 0.0486f, sizeOffset + 1.52f, SpaceOrb::OrbName::Mars));
    std::cout << "Mars offset" << sizeOffset << std::endl;
    soMars = sizeOffset;
    sizeOffset += 1.52f;
    mOrbs.push_back(SpaceOrb("../../Assets/Images/planets/jupiter.jpg", 1.003f, sizeOffset + 5.2f, SpaceOrb::OrbName::Jupiter));
    std::cout << "Jupiter offset" << sizeOffset << std::endl;
    soJupiter = sizeOffset;
    sizeOffset += 5.2f;
    mOrbs.push_back(SpaceOrb("../../Assets/Images/planets/saturn.jpg", 0.836f, sizeOffset + 9.54f, SpaceOrb::OrbName::Saturn));
    std::cout << "Saturn offset" << sizeOffset << std::endl;
    soSaturn = sizeOffset;
    sizeOffset += 9.54f;
    mOrbs.push_back(SpaceOrb("../../Assets/Images/planets/uranus.jpg", 0.364f, sizeOffset + 19.22f, SpaceOrb::OrbName::Uranus));
    std::cout << "Uranus offset" << sizeOffset << std::endl;
    soUranus = sizeOffset;
    sizeOffset += 19.22f;
    mOrbs.push_back(SpaceOrb("../../Assets/Images/planets/neptune.jpg", 0.353f, sizeOffset + 30.06f, SpaceOrb::OrbName::Neptune));
    std::cout << "Neptune offset" << sizeOffset << std::endl;
    soNeptune = sizeOffset;
    sizeOffset += 30.06f;
    mOrbs.push_back(SpaceOrb("../../Assets/Images/planets/pluto.jpg", 0.017f, sizeOffset + 39, SpaceOrb::OrbName::Pluto));
    std::cout << "Pluto offset" << sizeOffset << std::endl;
    soPluto = sizeOffset;
    sizeOffset += 39.0f;
    soMercury = sizeOffset;
    //SkySphere
    skySphere = new SkySphere("../../Assets/Images/skysphere/space.jpg", 1000.0f, 0.0f);
}

void MilkyWay::Terminate()
{
    for (auto& planet : mOrbs)
    {
        planet.Terminate();
    }
    mSampler.Terminate();
    mPixelShader.Terminate();
    mVertexShader.Terminate();
    mConstantBuffer.Terminate();
}

//mdt for modified delta time
//mActive for simple update pause

void MilkyWay::Update(float dt)
{
    CameraControllers(dt, mCamera);

    if (mActive)
    {
        itercount += mdt;
        for (SpaceOrb& orb : mOrbs)
        {
            switch (orb.mName)
            {
            case SpaceOrb::OrbName::Sun:
                //orb.Update(mdt, 0, 0.18);
                //6.26 rotation div
                //5 revolution div
                orb.Update(mdt, 0, 1);
                break;
            case SpaceOrb::OrbName::Mercury:
                orb.Update(mdt, 0.0715f, 0.08532f);
                break;
            case SpaceOrb::OrbName::Venus:
                orb.Update(mdt, 0.02576f, 0.02225f);
                break;
            case SpaceOrb::OrbName::Earth:
                orb.Update(mdt, 0.01719f, 5);
                break;
            case SpaceOrb::OrbName::Mars:
                orb.Update(mdt, 0.1645f, 4.8543f);
                break;
            case SpaceOrb::OrbName::Jupiter:
                orb.Update(mdt, 0.00115f, 12.1951f);
                break;
            case SpaceOrb::OrbName::Saturn:
                orb.Update(mdt, 0.000466f, 11.1111f);
                break;
            case SpaceOrb::OrbName::Uranus:
                orb.Update(mdt, 0.000163f, 6.9444f);
                break;
            case SpaceOrb::OrbName::Neptune:
                orb.Update(mdt, 0.000083f, 7.4626f);
                break;
            case SpaceOrb::OrbName::Pluto:
                orb.Update(mdt, 0.000055, 0.7824f);
                break;
            default:
                break;
            }
        }
    }  
}

void MilkyWay::Render()
{
    mVertexShader.Bind();
    mPixelShader.Bind();
    mSampler.BindPS(0);


    Math::Matrix4 matView = mCamera.GetViewMatrix();
    Math::Matrix4 matProj = mCamera.GetProjectionMatrix();

    
    for (auto& planet : mOrbs)
    {
        planet.Render(mCamera, mConstantBuffer, matView, matProj);
    }
    skySphere->Render(mCamera, mConstantBuffer, matView, matProj);
    SimpleDraw::AddGroundPlane(50, Colors::Gray);
    SimpleDraw::AddGroundCircle(500, soMercury, Colors::BlanchedAlmond);
    SimpleDraw::AddGroundCircle(500, soVenus, Colors::BlanchedAlmond);
    SimpleDraw::AddGroundCircle(500, soEarth, Colors::BlanchedAlmond);
    SimpleDraw::AddGroundCircle(500, soMars, Colors::BlanchedAlmond);
    SimpleDraw::AddGroundCircle(500, soJupiter, Colors::BlanchedAlmond);
    SimpleDraw::AddGroundCircle(500, soSaturn, Colors::BlanchedAlmond);
    SimpleDraw::AddGroundCircle(500, soUranus, Colors::BlanchedAlmond);
    SimpleDraw::AddGroundCircle(500, soNeptune, Colors::BlanchedAlmond);
    SimpleDraw::AddGroundCircle(500, soPluto, Colors::BlanchedAlmond);
    //SimpleDraw::AddGroundCircle();
    
    SimpleDraw::Render(mCamera);
}

void MilkyWay::DebugUI()
{
    ImGui::Begin("DebugUI", nullptr, ImGuiWindowFlags_AlwaysAutoResize);

    if (ImGui::Button("Pause"))
    {
        mActive = !mActive;
    }
    
    if (ImGui::CollapsingHeader("Info", ImGuiTreeNodeFlags_DefaultOpen))
    {           
        ImGui::DragFloat("Time Scale", &mdt, TimeUtil::GetDeltaTime() * 0.01, -0.5f, 0.50f);
        
        ImGui::Text("Days Passed: %f", itercount);
    }

    ImGui::End();
}


