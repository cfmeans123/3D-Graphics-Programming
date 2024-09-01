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

	void OnSpaceEvent(const MEngine::Event* event);
	void OnAnimEvent(const MEngine::Event* event);

	MEngine::Graphics::Camera mCamera;
	MEngine::Graphics::DirectionalLight mDirectionalLight;
	MEngine::Graphics::StandardEffect mStandardEffect;
	
	MEngine::Graphics::RenderGroup mCharacter;

	std::vector<MEngine::Audio::SoundId> mEventSoundIds;
	MEngine::Graphics::Animation mEventAnimation;
	float mEventAnimationTime;

	MEngine::ListenerId mSpaceEventId = 0;
	MEngine::ListenerId mAnimEventId = 0;
	//MEngine::Graphics::EventAnimation mEventAnimation;

	float mPitch = 0.0f;
	float mYaw = 0.0f;
	float mRoll = 0.0f;

	void OnEvent2();
	void OnEvent3();
};