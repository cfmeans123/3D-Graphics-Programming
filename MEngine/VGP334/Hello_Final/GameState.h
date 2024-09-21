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

	MEngine::Graphics::RenderGroup mCreatureOne;
	MEngine::Graphics::ModelID mCreatureOneID;
	MEngine::Graphics::Animator mCreatureOneAnimator;
	MEngine::Graphics::Animation mCreatureOneEvent;
	float mCreatureOneEventTime;

	MEngine::Graphics::RenderGroup mCreatureTwo;
	MEngine::Graphics::ModelID mCreatureTwoID;
	MEngine::Graphics::Animator mCreatureTwoAnimator;
	MEngine::Graphics::Animation mCreatureTwoEvent;
	float mCreatureTwoEventTime;

	MEngine::Graphics::RenderObject mGround;
	MEngine::Graphics::RenderObject mSpace;
	

	float mAnimSpeed = 1.0f;

	MEngine::Graphics::ParticleSystemEffect mParticleEffect;
	MEngine::Physics::ParticleSystem mParticleSystem;

};