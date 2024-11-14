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

Service* CustomServiceMake(const std::string& serviceName, GameWorld& gameWorld)
{
    if (serviceName == "CustomDebugDrawService")
    {
        return gameWorld.AddService<CustomDebugDrawService>();
    }
    return nullptr;
}

void GameState::Initialize()
{
    GameObjectFactory::SetCustomMake(CustomComponentMake);
    GameObjectFactory::SetCustomGet(CustomComponentGet);
    GameWorld::SetCustomService(CustomServiceMake);
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
    UIFont::Get()->DrawString(L"Hello World", {100.0f, 100.0f}, 20.0f, Colors::Aqua);
}
void GameState::DebugUI()
{
    ImGui::Begin("Debug control", nullptr, ImGuiWindowFlags_AlwaysAutoResize);
        mGameWorld.DebugUI();
    ImGui::End();
}