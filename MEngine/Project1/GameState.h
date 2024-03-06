#pragma once

#include "..\Engine\MEngine\Inc\MEngine.h"

using namespace MEngine;

namespace
{
	enum class Shapes
	{
		Sphere,
		AABB,
		Circle
	};

	const char* shapeType[] =
	{
		"Sphere",
		"AABB",
		"Circle"
	};
}

class GameState : public MEngine::AppState
{
public:
	void Initialize() override;
	void Terminate() override;
	void Update(float deltaTime) override;
	void Render() override;
	void DebugUI() override;

protected:
	MEngine::Graphics::Camera mCamera;
	float mAlpha = 1.0f;
	bool mDebugOn = false;
	Color mShapeColor = Colors::Yellow;
	Shapes mCurrentShape = Shapes::Sphere;
	bool mIsCircle = true;
	float mRadius = 1.0f;
	Math::Vector3 mAABBSize = Math::Vector3::One;
	Math::Vector3 mPosition = Math::Vector3::Zero;
};

