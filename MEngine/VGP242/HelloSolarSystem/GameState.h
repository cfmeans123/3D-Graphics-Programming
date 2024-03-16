#pragma once

#include "SpaceOrb.h"
#include "SkySphere.h"

using namespace MEngine;

class MilkyWay : public MEngine::AppState
{
public:
	void Initialize() override;
	void Terminate() override;
	void Update(float dt) override;
	void Render() override;
	void DebugUI() override;

	float soMercury;
	float soVenus;
	float soEarth;
	float soMars;
	float soJupiter;
	float soSaturn;
	float soUranus;
	float soNeptune;
	float soPluto;


	bool mActive = true;
	float mdt;
	float itercount = 0;
private:
	Graphics::Camera mCamera;

	std::vector<SpaceOrb> mOrbs;
	//std::vector<SimpleDraw::> mOrbs;
	SkySphere* skySphere;
	Graphics::ConstantBuffer mConstantBuffer;
	Graphics::VertexShader mVertexShader;
	Graphics::PixelShader mPixelShader;
	Graphics::Sampler mSampler;
};
