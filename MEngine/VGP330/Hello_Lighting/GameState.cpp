#include "GameState.h"

using namespace MEngine;
using namespace MEngine::Graphics;
using namespace MEngine::Input;

void GameState::Initialize()
{
	mCamera.SetPosition({ 0.0f, 1.0f, -3.0f });
	mCamera.SetLookAt({ 0.0f, 0.0f, 0.0f });

	mDirectionalLight.direction = Math::Normalize({1.0f, -1.0f, 1.0f});
	mDirectionalLight.ambient = { 0.5f, 0.5f, 0.5f, 1.0f };
	mDirectionalLight.diffuse = { 0.8f, 0.8f, 0.8f, 1.0f };
	mDirectionalLight.specular = { 1.0f, 1.0f, 1.0f, 1.0f };

	TextureManager* tm = TextureManager::Get();
	mMesh = MeshBuilder::CreateSphere(100.0f, 100.0f, 3.0f);
	mRenderObject.meshBuffer.Initialize(mMesh);
	mRenderObject.diffuseTextureMapId = tm->LoadTexture("planets/earth/earth.jpg");
	mRenderObject.specTextureMapId = tm->LoadTexture("planets/earth/earth_spec.jpg");
	mRenderObject.normalTextureMapId = tm->LoadTexture("planets/earth/earth_normal.jpg");

	std::filesystem::path shaderFilePath = L"../../Assets/Shaders/Standard.fx";
	mStandardEffect.Initialize(shaderFilePath);
	mStandardEffect.SetCamera(mCamera);
	mStandardEffect.SetDirectionalLight(mDirectionalLight);
}
void GameState::Terminate()
{
	mStandardEffect.Terminate();
	mRenderObject.Terminate();
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
	/*if (input->IsKeyPressed(KeyCode::ONE))
	{
		MainApp().ChangeState("CubeState");
	}*/
}
void GameState::Render()
{
	SimpleDraw::AddGroundPlane(10.0f, Colors::White);
	SimpleDraw::Render(mCamera);

	mStandardEffect.Begin();
	mStandardEffect.Render(mRenderObject);
	mStandardEffect.End();
}

void GameState::DebugUI()
{
	ImGui::Begin("Debug Controls", nullptr, ImGuiWindowFlags_AlwaysAutoResize);
	if (ImGui::CollapsingHeader("Light", ImGuiTreeNodeFlags_DefaultOpen))
	{
		if (ImGui::DragFloat3("Direction", &mDirectionalLight.direction.x, 0.01f))
		{
			mDirectionalLight.direction = Math::Normalize(mDirectionalLight.direction);
		}

		ImGui::ColorEdit4("Ambient##Light", &mDirectionalLight.ambient.r);
		ImGui::ColorEdit4("Diffuse##Light", &mDirectionalLight.diffuse.r);
		ImGui::ColorEdit4("Specular##Light", &mDirectionalLight.specular.r);
	}
	if (ImGui::CollapsingHeader("Light", ImGuiTreeNodeFlags_DefaultOpen))
	{
		ImGui::ColorEdit4("Ambient##Material", &mRenderObject.material.ambient.r);
		ImGui::ColorEdit4("Diffuse##Material", &mRenderObject.material.diffuse.r);
		ImGui::ColorEdit4("Specular##Material", &mRenderObject.material.specular.r);
		ImGui::ColorEdit4("Emissive##Material", &mRenderObject.material.emissive.r);

		ImGui::DragFloat3("SpecPower##Material", &mRenderObject.material.power, 1.0f, 1.0f, 500.0f);
	}


	mStandardEffect.DebugUI();
	ImGui::End();
}

