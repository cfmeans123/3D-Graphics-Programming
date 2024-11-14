#include "GameState.h"

#include "CustomDebugDrawService.h"
#include "CustomDebugDrawComponent.h"

using namespace MEngine;
using namespace MEngine::Graphics;
using namespace MEngine::Input;
using namespace MEngine::Audio;

Component* CustomComponentMake(const std::string& componentName, GameObject& gameObject)
{
    if (componentName == "CustomDebugDrawComponent")
    {
        return gameObject.AddComponent<CustomDebugDrawComponent>();
    }
    return nullptr;
}

Component* CustomComponentGet(const std::string& componentName, GameObject& gameObject)
{
    if (componentName == "CustomDebugDrawComponent")
    {
        return gameObject.GetComponent<CustomDebugDrawComponent>();
    }
    return nullptr;
}

void GameState::Initialize()
{
    mGameWorld.LoadLevel(L"../../Assets/Templates/Levels/test_level.json");
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