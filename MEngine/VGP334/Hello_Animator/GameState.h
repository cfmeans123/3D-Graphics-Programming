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

	void InitIKChains();



	MEngine::Graphics::DirectionalLight mDirectionalLight;
	MEngine::Graphics::Camera mCamera;
	MEngine::Graphics::RenderObject mGround;

	MEngine::Graphics::ModelID mModelID;
	MEngine::Graphics::RenderGroup mCharacter;
	MEngine::Graphics::IKAnimator mCharacterAnimator;
	MEngine::Graphics::StandardEffect mStandardEffect;
	
	MEngine::Graphics::IKChain mIKChain;
	MEngine::Graphics::IKChain mIKChain2;
	MEngine::Graphics::IKChain mIKChain3;
	MEngine::Graphics::IKChain mIKChain4;
	bool two = true;


	int selectedIndexStart = 0;
	int selectedIndexEnd = 0;

	int mAnimIndex = -1;
	bool mDrawSkeleton = false;

	MEngine::Graphics::AnimationUtil::BoneTransforms boneTransforms;
	Vector3 mTarget = { 1.0f, 1.0f, 0.0f };
	Vector3 mTarget2 = { -1.0f, 1.0f, 0.0f };
	Vector3 mTarget3 = { .25f, 0.0f, 0.0f };
	Vector3 mTarget4 = { -.25f, 0.0f, 0.0f };
	float length = 0.0f;

	int startBoneIndex = 0;
	int endBoneIndex = 0;

	float mBlendDuration = 0.0f;
	float mBlendWeightIdle = 0.0f;
	float mBlendWeightForward = 0.0f;
	float mBlendWeightLeft = 0.0f;

	//Debug Variables
	Vector3 offsetHands = Vector3::Zero;
	Vector3 offsetFeet = Vector3::Zero;

};