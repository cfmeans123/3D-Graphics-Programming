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

	bool DragTwoFloats(const char* label, float* v1, float* v2, float v_speed = 1.0f, float v_min = 0.0f, float v_max = 0.0f, const char* format = "%.3f", ImGuiSliderFlags flags = 0)
	{
		ImGui::Begin(label);
		bool valueChanged1 = ImGui::DragFloat("##v1", v1, v_speed, v_min, v_max, format, flags);
		bool valueChanged2 = ImGui::DragFloat("##v2", v2, v_speed, v_min, v_max, format, flags);
		ImGui::End();
		return valueChanged1 || valueChanged2;
	}



	MEngine::Graphics::DirectionalLight mDirectionalLight;
	MEngine::Graphics::Camera mCamera;
	MEngine::Graphics::RenderObject mGround;

	MEngine::Graphics::ModelID mModelID;
	MEngine::Graphics::RenderGroup mCharacter;
	MEngine::Graphics::IKAnimator mCharacterAnimator;
	MEngine::Graphics::StandardEffect mStandardEffect;
	MEngine::Graphics::IKChain mIKChain;
	MEngine::Graphics::IKChain mIKChain2;
	bool two = false;


	int selectedIndexStart = 0;
	int selectedIndexEnd = 0;

	int mAnimIndex = -1;
	bool mDrawSkeleton = false;

	MEngine::Graphics::AnimationUtil::BoneTransforms boneTransforms;
	Vector3 mTarget = { 1.0f, 0.0f, 0.0f };
	Vector3 mTarget2 = { -1.0f, 0.0f, 0.0f };
	float length = 0.0f;

	int startBoneIndex = 0;
	int endBoneIndex = 0;

	float mBlendDuration = 0.0f;
	float mBlendWeightIdle = 0.0f;
	float mBlendWeightForward = 0.0f;
	float mBlendWeightLeft = 0.0f;

};