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
	MEngine::Graphics::IKAnimator mCharacterAnimator;
	MEngine::Graphics::StandardEffect mStandardEffect;

	int selectedIndex = 0;

	int mAnimIndex = -1;
	bool mDrawSkeleton = false;

	float mBlendDuration = 0.0f;
	float mBlendWeightIdle = 0.0f;
	float mBlendWeightForward = 0.0f;
	float mBlendWeightLeft = 0.0f;

};