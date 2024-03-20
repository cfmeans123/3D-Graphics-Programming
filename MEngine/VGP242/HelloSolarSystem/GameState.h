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

	float mdt;
	float itercount = 0;
	
	//Debug Draw Variables
	bool mActive = true;
	bool mTogglePathDraw = true;
	bool toggleRealTime = false;

	//Debug Modifier Variables
	float mercuryRevolution = 0.0715f;
	float venusRevolution = 0.02576f;
	float earthRevolution = 0.01719f;
	float marsRevolution = 0.1645f;
	float jupiterRevolution = 0.00115f;
	float saturnRevolution = 0.000466f;
	float uranusRevolution = 0.000163f;
	float neptuneRevolution = 0.000083f;
	float plutoRevolution = 0.000055;

	float mercuryRotation = 0.08532f;
	float venusRotation = 0.02225f;
	float earthRotation = 5;
	float marsRotation = 4.8543f;
	float jupiterRotation = 12.1951f;
	float saturnRotation = 11.1111f;
	float uranusRotation = 6.9444f;
	float neptuneRotation = 7.4626f;
	float plutoRotation = 0.7824f;

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
