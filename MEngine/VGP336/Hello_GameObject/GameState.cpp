#include "GameState.h"

using namespace MEngine;
using namespace MEngine::Graphics;
using namespace MEngine::Math;
using namespace MEngine::Input;
using namespace MEngine::Audio;


void GameState::Initialize()
{
    // add components first    
    mGameWorld.AddService<CameraService>();
    mGameWorld.Initialize();

    GameObject* mGameObject = mGameWorld.CreateGameObject("Object0");
    mGameObject->AddComponent<TransformComponent>();
    // finally initialize
    mGameObject->Initialize();
    
    GameObject* cameraGameObject = mGameWorld.CreateGameObject("Camera");
    cameraGameObject->AddComponent<CameraComponent>();
    cameraGameObject->AddComponent<FPSCameraComponent>();
    cameraGameObject->Initialize();
}
void GameState::Terminate()
{
    mGameWorld.Terminate();
}
void GameState::Update(const float deltaTime)
{
    mGameWorld.Update(deltaTime);
 
}
void GameState::Render()
{
    mGameWorld.Render();
}
void GameState::DebugUI()
{
    ImGui::Begin("Debug control", nullptr, ImGuiWindowFlags_AlwaysAutoResize);
        mGameWorld.DebugUI();
    ImGui::End();
}