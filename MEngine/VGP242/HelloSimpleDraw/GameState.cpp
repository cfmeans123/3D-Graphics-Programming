#include "GameState.h"

using namespace MEngine;
using namespace MEngine::Graphics;
using namespace MEngine::Input;

void GameState::Initialize()
{
	mCamera.SetPosition({ 0.0f, 1.0f, -3.0f });
	mCamera.SetLookAt({ 0.0f, 0.0f, 0.0f });
}
void GameState::Terminate()
{
}
void GameState::Update(float deltaTime)
{
	auto input = Input::InputSystem::Get();
	const float moveSpeed = input->IsKeyDown(KeyCode::LSHIFT) ? 10.0f : 1.0f;
	const float turnSpeed = 0.1f;

	if (input->IsKeyDown(KeyCode::W))
	{
		mCamera.Walk(moveSpeed * deltaTime);
	}
	else if (input->IsKeyDown(KeyCode::S))
	{
		mCamera.Walk(-moveSpeed * deltaTime);
	}
	if (input->IsKeyDown(KeyCode::D))
	{
		mCamera.Strafe(moveSpeed * deltaTime);
	}
	else if (input->IsKeyDown(KeyCode::A))
	{
		mCamera.Strafe(-moveSpeed * deltaTime);
	}
	if (input->IsKeyDown(KeyCode::E))
	{
		mCamera.Rise(moveSpeed * deltaTime);
	}
	else if (input->IsKeyDown(KeyCode::Q))
	{
		mCamera.Rise(-moveSpeed * deltaTime);
	}
	if (input->IsMouseDown(MouseButton::RBUTTON))
	{
		mCamera.Yaw(input->GetMouseMoveX() * turnSpeed * deltaTime);
		mCamera.Pitch(input->GetMouseMoveY() * turnSpeed * deltaTime);
	}

	//Change States
	if (input->IsKeyPressed(KeyCode::ONE))
	{
		MainApp().ChangeState("TransformState");
	}
	else if (input->IsKeyPressed(KeyCode::TWO))
	{
		MainApp().ChangeState("SphereState");
	}
	else if (input->IsKeyPressed(KeyCode::THREE))
	{
		MainApp().ChangeState("AABBState");
	}
	else if (input->IsKeyPressed(KeyCode::FOUR))
	{
		MainApp().ChangeState("AABBFilledState");
	}
	else if (input->IsKeyPressed(KeyCode::FIVE))
	{
		MainApp().ChangeState("LinesState");
	}
}
void GameState::Render()
{
	SimpleDraw::AddGroundPlane(10, Colors::White);
	SimpleDraw::Render(mCamera);
}

void TransformState::Render()
{
	SimpleDraw::AddTransform(Matrix4::Identity);
	SimpleDraw::Render(mCamera);
}

void SphereState::Render()
{
	SimpleDraw::AddSphere(60, 60, 2.0f, Colors::Yellow);
	SimpleDraw::Render(mCamera);
}

void AABBState::Render()
{
	SimpleDraw::AddAABB({ -1.0f, -1.0f, -1.0f }, { 1.0f, 1.0f, 1.0f }, Colors::Pink);
	SimpleDraw::Render(mCamera);
}

void AABBFilledState::Render()
{
	SimpleDraw::AddFilledAABB({ -1.0f, -1.0f, -1.0f }, { 1.0f, 1.0f, 1.0f }, Colors::Purple);
	SimpleDraw::Render(mCamera);
}

void LinesState::Render()
{
	SimpleDraw::AddLine({ -1.0f, 0.0f, -1.0f }, { 0.0f, 1.0f, 0.0f }, Colors::Green);
	SimpleDraw::AddLine({ 1.0f, 0.0f,  1.0f }, { 0.0f, 1.0f, 0.0f }, Colors::Green);
	SimpleDraw::AddLine({ 1.0f, 0.0f, -1.0f }, { 0.0f, 1.0f, 0.0f }, Colors::Green);
	SimpleDraw::AddLine({ -1.0f, 0.0f,  1.0f }, { 0.0f, 1.0f, 0.0f }, Colors::Green);

	SimpleDraw::AddLine({ -1.0f, 0.0f, -1.0f }, { 1.0f, 0.0f, -1.0f }, Colors::Green);
	SimpleDraw::AddLine({ 1.0f, 0.0f,  1.0f }, { 1.0f, 0.0f, -1.0f }, Colors::Green);
	SimpleDraw::AddLine({ 1.0f, 0.0f,  1.0f }, { -1.0f, 0.0f,  1.0f }, Colors::Green);
	SimpleDraw::AddLine({ -1.0f, 0.0f,  1.0f }, { -1.0f, 0.0f, -1.0f }, Colors::Green);

	SimpleDraw::Render(mCamera);
}