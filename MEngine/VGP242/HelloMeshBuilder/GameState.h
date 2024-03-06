#pragma once

#include <MEngine/Inc/MEngine.h>

class GameState : public MEngine::AppState
{
public:
	void Initialize() override;
	void Terminate() override;
	void Update(float deltaTime) override;
	void Render() override;

protected:
	MEngine::Graphics::Camera mCamera;
	MEngine::Graphics::MeshPX mMesh;
	MEngine::Graphics::ConstantBuffer mConstantBuffer;
	MEngine::Graphics::MeshBuffer mMeshBuffer;
	MEngine::Graphics::VertexShader mVertexShader;
	MEngine::Graphics::PixelShader mPixelShader;

	MEngine::Graphics::Texture mTexture;
	MEngine::Graphics::Sampler mSampler;
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