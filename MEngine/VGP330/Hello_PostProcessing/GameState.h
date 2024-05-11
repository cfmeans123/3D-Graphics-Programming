#pragma once

#include <MEngine/Inc/MEngine.h>

class GameState : public MEngine::AppState
{
public:
	void Initialize() override;
	void Terminate() override;
	void Update(float dt) override;
	void Render() override;
	void DebugUI() override;

private:
	void CameraControllers(float dt);

private:
	MEngine::Graphics::DirectionalLight mDirectionalLight;
	MEngine::Graphics::Camera mCamera;
	
	MEngine::Graphics::RenderGroup mRenderGroupA;
	MEngine::Math::Matrix4 mPositionA;
	
	MEngine::Graphics::RenderGroup mRenderGroupB;
	MEngine::Math::Matrix4 mPositionB;

	MEngine::Graphics::StandardEffect mStandardEffect;
	MEngine::Graphics::RenderObject mGround;
	MEngine::Graphics::RenderObject mScreenQuad;

	MEngine::Graphics::RenderTarget mRenderTarget;

};