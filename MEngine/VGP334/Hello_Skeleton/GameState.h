#pragma once

#include <MEngine/Inc/MEngine.h>

class GameState : public MEngine::AppState
{
public:
	void Initialize()  override;
	void Terminate()  override;
	void Update(const float deltaTime) override;
	void Render() override;
	void DebugUI() override;

private:
	void UpdateCameraControl(float deltaTime);

	MEngine::Graphics::DirectionalLight mDirectionalLight;
	MEngine::Graphics::Camera mCamera;
	
	MEngine::Graphics::ModelID mModelID;
	MEngine::Graphics::RenderGroup mCharacter;
	MEngine::Graphics::StandardEffect mStandardEffect;

	bool mDrawSkeleton = false;
};