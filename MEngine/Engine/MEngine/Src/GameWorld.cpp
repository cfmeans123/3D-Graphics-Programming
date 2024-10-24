#include "Precompiled.h"
#include "GameWorld.h"
#include "GameObjectFactory.h"

using namespace MEngine;

void GameWorld::Initialize()
{
	ASSERT(!mInitialized, "GameWorld: is already initialized");
	for (auto& service : mServices)
	{
		service->Initialize();
	}


	mInitialized = true;
}

void GameWorld::Terminate()
{
	for (auto& gameObject : mGameObjects)
	{
		gameObject->Terminate();
		gameObject.reset();
	}
	mGameObjects.clear();

	for (auto& service : mServices)
	{
		service->Terminate();
		service.reset();
	}
	mServices.clear();
}

void GameWorld::Update(float deltaTime)
{
	for (auto& gameObject : mGameObjects)
	{
		gameObject->Update(deltaTime);
	}
	for (auto& service : mServices)
	{
		service->Update(deltaTime);
	}
}

void GameWorld::Render()
{
	for (auto& service : mServices)
	{
		service->Render();
	}
}

void GameWorld::DebugUI()
{
	for (auto& gameObject : mGameObjects)
	{
		gameObject->DebugUI();
	}
	for (auto& service : mServices)
	{
		service->DebugUI();
	}
}

GameObject* GameWorld::CreateGameObject(std::string name, const std::filesystem::path& templatePath)
{

	auto& newGameObject = mGameObjects.emplace_back(std::make_unique<GameObject>());
	newGameObject->SetName(name);
	newGameObject->mWorld = this;
	if (!templatePath.empty())
	{
		GameObjectFactory::Make(templatePath, *newGameObject);
		newGameObject->Initialize();
		newGameObject->mTemplateFilePath = templatePath;
	}
	return newGameObject.get();
}
