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

	MEngine::Graphics::Camera mCamera;
	MEngine::Graphics::DirectionalLight mDirectionalLight;
	MEngine::Graphics::StandardEffect mStandardEffect;
	
	MEngine::Graphics::RenderGroup mCharacter;

	float mPitch = 0.0f;
	float mYaw = 0.0f;
	float mRoll = 0.0f;
};