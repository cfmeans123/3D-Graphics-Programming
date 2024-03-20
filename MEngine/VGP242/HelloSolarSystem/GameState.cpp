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
    mOrbs.push_back(SpaceOrb("../../Assets/Images/planets/sun.jpg", sizeOffset, 0, SpaceOrb::OrbName::Sun));
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

    //SkySphere init
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
            mdt = (toggleRealTime) ? dt : mdt;
            switch (orb.mName)
            {
            case SpaceOrb::OrbName::Sun:
                orb.Update(mdt, 0, 0.18);
                //6.26 rotation div
                //5 revolution div
                break;
            case SpaceOrb::OrbName::Mercury:
                orb.Update(mdt, mercuryRevolution, mercuryRotation);
                break;
            case SpaceOrb::OrbName::Venus:
                orb.Update(mdt, venusRevolution, venusRotation);
                break;
            case SpaceOrb::OrbName::Earth:
                orb.Update(mdt, earthRevolution, earthRotation);
                break;
            case SpaceOrb::OrbName::Mars:
                orb.Update(mdt, marsRevolution, marsRotation);
                break;
            case SpaceOrb::OrbName::Jupiter:
                orb.Update(mdt, jupiterRevolution, jupiterRotation);
                break;
            case SpaceOrb::OrbName::Saturn:
                orb.Update(mdt, saturnRevolution, saturnRotation);
                break;
            case SpaceOrb::OrbName::Uranus:
                orb.Update(mdt, uranusRevolution, uranusRotation);
                break;
            case SpaceOrb::OrbName::Neptune:
                orb.Update(mdt, neptuneRevolution, neptuneRotation);
                break;
            case SpaceOrb::OrbName::Pluto:
                orb.Update(mdt, plutoRevolution, plutoRotation);
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

    if (mTogglePathDraw)
    {
        SimpleDraw::AddGroundCircle(500, soMercury, Colors::BlanchedAlmond);
        SimpleDraw::AddGroundCircle(500, soVenus, Colors::BlanchedAlmond);
        SimpleDraw::AddGroundCircle(500, soEarth, Colors::BlanchedAlmond);
        SimpleDraw::AddGroundCircle(500, soMars, Colors::BlanchedAlmond);
        SimpleDraw::AddGroundCircle(500, soJupiter, Colors::BlanchedAlmond);
        SimpleDraw::AddGroundCircle(500, soSaturn, Colors::BlanchedAlmond);
        SimpleDraw::AddGroundCircle(500, soUranus, Colors::BlanchedAlmond);
        SimpleDraw::AddGroundCircle(500, soNeptune, Colors::BlanchedAlmond);
        SimpleDraw::AddGroundCircle(500, soPluto, Colors::BlanchedAlmond);
    }
    SimpleDraw::AddGroundPlane(50, Colors::Gray);
    skySphere->Render(mCamera, mConstantBuffer, matView, matProj);
    SimpleDraw::Render(mCamera);        
}

void MilkyWay::DebugUI()
{
    ImGui::Begin("DebugUI", nullptr, ImGuiWindowFlags_AlwaysAutoResize);

    if (ImGui::Button("Pause"))
    {
        mActive = !mActive;
    }

    if (ImGui::Button("Show Orbital Rings"))
    {
        mTogglePathDraw = !mTogglePathDraw;
    }
    
    if (ImGui::CollapsingHeader("Time Modifier", ImGuiTreeNodeFlags_DefaultOpen))
    {           
        ImGui::DragFloat("Time Scale", &mdt, TimeUtil::GetDeltaTime() * 0.01, -0.5f, 0.50f);
        
        ImGui::Text("Days Passed: %f", itercount);

        if (ImGui::Button("Set 'Nice' Timestep"))
        {
            toggleRealTime = !toggleRealTime;
            mdt = 0.0f;
        }
    }

    if (ImGui::CollapsingHeader("Planet Revolution Modifier", ImGuiTreeNodeFlags_DefaultOpen))
    {
        ImGui::DragFloat("Mercury Revolution", &mercuryRevolution, abs(mercuryRevolution) * 0.1, -20.0f, 20.0f);
        ImGui::DragFloat("Venus Revolution", &venusRevolution, abs(venusRevolution) *       0.1, -20.0f, 20.0f);
        ImGui::DragFloat("Earth Revolution", &earthRevolution, abs(earthRevolution) *       0.1, -20.0f, 20.0f);
        ImGui::DragFloat("Mars Revolution", &marsRevolution, abs(marsRevolution) *          0.1, -20.0f, 20.0f);
        ImGui::DragFloat("Jupiter Revolution", &jupiterRevolution, abs(jupiterRevolution) * 0.1, -20.0f, 20.0f);
        ImGui::DragFloat("Saturn Revolution", &saturnRevolution, abs(saturnRevolution) *    0.1, -20.0f, 20.0f);
        ImGui::DragFloat("Uranus Revolution", &uranusRevolution, abs(uranusRevolution) *    0.1, -20.0f, 20.0f);
        ImGui::DragFloat("Neptune Revolution", &neptuneRevolution, abs(neptuneRevolution) * 0.1, -20.0f, 20.0f);
        ImGui::DragFloat("Pluto Revolution", &plutoRevolution, abs(plutoRevolution) *       0.1, -20.0f, 20.0f);
        if (ImGui::Button("Reset Revolution Speeds"))
        {
            mercuryRevolution = 0.0715f;
            venusRevolution = 0.02576f;
            earthRevolution = 0.01719f;
            marsRevolution = 0.1645f;
            jupiterRevolution = 0.00115f;
            saturnRevolution = 0.000466f;
            uranusRevolution = 0.000163f;
            neptuneRevolution = 0.000083f;
            plutoRevolution = 0.000055;
        }
    }


    if (ImGui::CollapsingHeader("Planet Rotation Modifier", ImGuiTreeNodeFlags_DefaultOpen))
    {
        ImGui::DragFloat("Mercury Rotation", &mercuryRotation, abs(mercuryRotation) * 0.1, -20.0f, 20.0f);
        ImGui::DragFloat("Venus Rotation", &venusRotation, abs(venusRotation) *       0.1, -20.0f, 20.0f);
        ImGui::DragFloat("Earth Rotation", &earthRotation, abs(earthRotation) *       0.1, -20.0f, 20.0f);
        ImGui::DragFloat("Mars Rotation", &marsRotation, abs(marsRotation) *          0.1, -20.0f, 20.0f);
        ImGui::DragFloat("Jupiter Rotation", &jupiterRotation, abs(jupiterRotation) * 0.1, -20.0f, 20.0f);
        ImGui::DragFloat("Saturn Rotation", &saturnRotation, abs(saturnRotation) *    0.1, -20.0f, 20.0f);
        ImGui::DragFloat("Uranus Rotation", &uranusRotation, abs(uranusRotation) *    0.1, -20.0f, 20.0f);
        ImGui::DragFloat("Neptune Rotation", &neptuneRotation, abs(neptuneRotation) * 0.1, -20.0f, 20.0f);
        ImGui::DragFloat("Pluto Rotation", &plutoRotation, abs(plutoRotation) *       0.1, -20.0f, 20.0f);
        if (ImGui::Button("Reset Rotation Speeds"))
        {
            mercuryRotation = 0.08532f;
            venusRotation = 0.02225f;
            earthRotation = 5;
            marsRotation = 4.8543f;
            jupiterRotation = 12.1951f;
            saturnRotation = 11.1111f;
            uranusRotation = 6.9444f;
            neptuneRotation = 7.4626f;
            plutoRotation = 0.7824f;
        }
    }




 

    ImGui::End();
}


