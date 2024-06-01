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
	
	//Effects

	MEngine::Graphics::StandardEffect mStandardEffect;
	MEngine::Graphics::ShadowEffect mShadowEffect;
	MEngine::Graphics::TerrainEffect mTerrainEffect;

	//Objects
	MEngine::Graphics::Terrain mTerrain;
	MEngine::Graphics::RenderGroup mRenderGroupA;
	MEngine::Graphics::RenderObject mGround;

	bool mOnTerrain = false;

};