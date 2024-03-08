#pragma once

#include "SpaceOrb.h"

using namespace MEngine;

class MilkyWay : public MEngine::AppState
{
public:
	void Initialize() override;
	void Terminate() override;
	void Update(float dt) override;
	void Render() override;
	void DebugUI() override;


	bool mActive = true;
	float mdt;
	float itercount = 0;
private:
	Graphics::Camera mCamera;

	std::vector<SpaceOrb> mOrbs;
	Graphics::ConstantBuffer mConstantBuffer;
	Graphics::VertexShader mVertexShader;
	Graphics::PixelShader mPixelShader;
	Graphics::Sampler mSampler;
};
