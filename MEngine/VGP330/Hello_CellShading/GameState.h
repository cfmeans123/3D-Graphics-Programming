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
	MEngine::Graphics::Mesh mMesh;
	
	MEngine::Graphics::RenderObject mRenderObject;
	MEngine::Graphics::StandardEffect mStandardEffect;
};

