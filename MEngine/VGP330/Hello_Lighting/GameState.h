#pragma once

#include <MEngine/Inc/MEngine.h>

class GameState : public MEngine::AppState
{
public:
	void Initialize() override;
	void Terminate() override;
	void Update(float deltaTime) override;
	void Render() override;
	void DebugUI() override;

protected:
	MEngine::Graphics::DirectionalLight mDirectionalLight;
	MEngine::Graphics::Camera mCamera;
	MEngine::Graphics::Mesh mMesh;
	MEngine::Graphics::ConstantBuffer mConstantBuffer;

	MEngine::Graphics::RenderObject mRenderObject;
	MEngine::Graphics::StandardEffect mStandardEffect;
};

