#pragma once

#include "../Engine/MEngine/Inc/MEngine.h"

class GameState : public MEngine::AppState
{
public:
	void Initialize() override;
	void Terminate() override;
	void Update(float deltaTime) override;
	void Render() override;

protected:
	MEngine::Graphics::Camera mCamera;

};

class TransformState : public GameState
{
public:
	void Render() override;
};

class SphereState : public GameState
{
public:
	void Render() override;
};

class AABBState : public GameState
{
public:
	void Render() override;
};

class AABBFilledState : public GameState
{
public:
	void Render() override;
};

class LinesState : public GameState
{
public:
	void Render() override;
};

