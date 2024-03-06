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
}
void GameState::Render()
{

}

void GameState::DebugUI()
{
	ImGui::Begin("DebugUI", nullptr, ImGuiWindowFlags_AlwaysAutoResize);
	DebugUI::SetTheme(DebugUI::Theme::Dark);
	ImGui::LabelText("Debug Manager", "Hello ImGui");
	if (ImGui::Button("Debug On"))
	{
		mDebugOn = !mDebugOn;
	}
	if (mDebugOn)
	{
		ImGui::LabelText("DebugOn", "Showing Debug Plane");
	}

	int value = (int)mCurrentShape;
	if (ImGui::Combo("ActiveShape", &value, shapeType, 3))
	{
		mCurrentShape = (Shapes)value;
		if (mCurrentShape == Shapes::AABB)
		{
			mIsCircle = false;
		}
		else
		{
			mIsCircle = true;
		}
	}
	if (ImGui::CollapsingHeader("Info", ImGuiTreeNodeFlags_DefaultOpen))
	{
		ImGui::DragFloat("SphereAlpha", &mAlpha, 0.01f, 0.0f, 1.0f);
		ImGui::ColorEdit4("ShapeColor", &mShapeColor.r);
		//ImGui::DragFloat3("TransformPos", &mPosition.x, 0.01f, -2.0f, 2.0f);
		if (mIsCircle)
		{
			ImGui::DragFloat("CircleRadius", &mRadius, 0.01f, 0.1f, 3.0f);
		}
		else
		{
			ImGui::DragFloat3("AABBSize", &mAABBSize.x, 0.01f, 0.1f, 3.0f);
		}
	}
	ImGui::End();

	mShapeColor.a = mAlpha;
	switch (mCurrentShape)
	{
	case Shapes::Sphere: SimpleDraw::AddSphere(60, 60, mRadius, mShapeColor); break;
	case Shapes::AABB: SimpleDraw::AddAABB(-mAABBSize, mAABBSize, mShapeColor); break;
	case Shapes::Circle: SimpleDraw::AddGroundCircle(60, mRadius, mShapeColor); break;
	}
	if (mDebugOn)
	{
		SimpleDraw::AddGroundPlane(10, Colors::White);
		SimpleDraw::AddTransform(Math::Matrix4::Translation({ 0.0f, 0.0f, 0.0f }));
	}

	SimpleDraw::Render(mCamera);
}
