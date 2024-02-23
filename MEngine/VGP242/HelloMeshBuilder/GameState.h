#pragma once

#include "../../Engine/MEngine/Inc/MEngine.h"

class GameState : public MEngine::AppState
{
public:
	void Initialize() override;
	void Terminate() override;
	void Update(float deltaTime) override;
	void Render() override;

protected:
	MEngine::Graphics::Camera mCamera;
	MEngine::Graphics::MeshPC mMesh;
	MEngine::Graphics::ConstantBuffer mConstantBuffer;
	MEngine::Graphics::MeshBuffer mMeshBuffer;
	MEngine::Graphics::VertexShader mVertexShader;
	MEngine::Graphics::PixelShader mPixelShader;

	MEngine::Graphics::Texture mTexture;
	MEngine::Graphics::Sampler mSampler;
};

