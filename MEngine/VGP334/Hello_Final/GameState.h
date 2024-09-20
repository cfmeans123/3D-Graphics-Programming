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

	MEngine::Graphics::RenderGroup mBiker;
	MEngine::Graphics::ModelID mBikerID;
	MEngine::Graphics::Animator mBikerAnimator;
	MEngine::Graphics::Animation mBikerEvent;
	float mBikerEventTime;

	MEngine::Graphics::RenderGroup mGuy;
	MEngine::Graphics::ModelID mGuyID;
	MEngine::Graphics::Animator mGuyAnimator;
	MEngine::Graphics::Animation mGuyEvent;
	float mGuyEventTime;

	MEngine::Graphics::RenderObject mGround;

	float mAnimSpeed = 1.0f;
};