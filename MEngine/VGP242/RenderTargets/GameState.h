#pragma once

#include <MEngine/Inc/MEngine.h>
#include "../Framework/Graphics/Inc/DebugUI.h"

class GameState : public MEngine::AppState
{
public:
	void Initialize() override;
	void Terminate() override;
	void Update(float deltaTime) override;
	void Render() override;
	void DebugUI();

protected:
	void RenderObject(const MEngine::Graphics::MeshBuffer& meshBuffer);
	MEngine::Graphics::Camera mCamera;
	MEngine::Graphics::MeshPX mMesh;
	MEngine::Graphics::ConstantBuffer mConstantBuffer;
	MEngine::Graphics::MeshBuffer mMeshBuffer;
	MEngine::Graphics::VertexShader mVertexShader;
	MEngine::Graphics::PixelShader mPixelShader;

	MEngine::Graphics::Texture mTexture;
	MEngine::Graphics::Sampler mSampler;

	MEngine::Graphics::RenderTarget mRenderTarget;
};

class CubeState : public GameState
{
public:
	void Initialize() override;
protected:
	MEngine::Graphics::MeshPC mMesh;
};

class RectState : public GameState
{
public:
	void Initialize() override;
protected:
	MEngine::Graphics::MeshPC mMesh;
};

class PlaneState : public GameState
{
public:
	void Initialize() override;
protected:
	MEngine::Graphics::MeshPC mMesh;
};

class SphereState : public GameState
{
public:
	void Initialize() override;
protected:
	MEngine::Graphics::MeshPC mMesh;
};

class CylinderState : public GameState
{
public:
	void Initialize() override;
protected:
	MEngine::Graphics::MeshPC mMesh;
};

class SkyboxState : public GameState
{
public:
	void Initialize() override;
protected:
	MEngine::Graphics::MeshPX mMesh;
};

class SkysphereState : public GameState
{
public:
	void Initialize() override;
protected:
	MEngine::Graphics::MeshPX mMesh;
};