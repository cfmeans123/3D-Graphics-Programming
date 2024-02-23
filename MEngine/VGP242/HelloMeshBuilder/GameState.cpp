#include "GameState.h"

using namespace MEngine;
using namespace MEngine::Graphics;
using namespace MEngine::Input;

void GameState::Initialize()
{
	mCamera.SetPosition({ 0.0f, 1.0f, -3.0f });
	mCamera.SetLookAt({ 0.0f, 0.0f, 0.0f });

	//mMesh = MeshBuilder::CreatePyramid(1.0f);
	mMesh = MeshBuilder::CreateHorizontalPlanePC(10, 10, 1.0f);

	//create a shape
	//mMesh = MeshBuilder::CreatePyramidPC(2.0f);
	//mMesh = MeshBuilder::CreateCubePC(2.0f);
	//mMesh = MeshBuilder::CreateRectPC(2.0f, 0.5f, 1.0f);
	//mMesh = MeshBuilder::CreateVerticalPlanePC(10, 10, 1.0f);
	//mMesh = MeshBuilder::CreateHorizontalPlanePC(10, 10, 1.0f);
	//mMesh = MeshBuilder::CreateCylinderPC(100, 4);
	//mMesh = MeshBuilder::CreateSpherePC(100, 100, 1);

	/*mMesh.vertices.push_back({ { -1.0f, -1.0f, 0.0f }, { 0.0f, 1.0f } });
	mMesh.vertices.push_back({ { -1.0f,  1.0f, 0.0f }, { 0.0f, 0.0f } });
	mMesh.vertices.push_back({ {  1.0f,  1.0f, 0.0f }, { 1.0f, 0.0f } });
	mMesh.vertices.push_back({ {  1.0f, -1.0f, 0.0f }, { 1.0f, 1.0f } });
	mMesh.indices = {
		0, 1, 2,
		0, 2, 3
	};*/

	//std::filesystem::path shaderFilePath = ""
	std::filesystem::path shaderFilePath = L"../Assets/Shaders/DoTransform.fx";
	std::filesystem::path textureFilePath = L"../Assets/Images/planets/neptune.jpg";
	mConstantBuffer.Initialize(sizeof(Math::Matrix4));
	mMeshBuffer.Initialize(mMesh);
	mVertexShader.Initialize<VertexPC>(shaderFilePath);
	mPixelShader.Initialize(shaderFilePath);
	mTexture.Initialize(L"../Assets/Images/planets/neptune.jpg");
	mSampler.Initialize(Sampler::Filter::Linear, Sampler::AddressMode::Wrap);

}
void GameState::Terminate()
{
	mVertexShader.Terminate();
	mMeshBuffer.Terminate();
	mPixelShader.Terminate();
	mConstantBuffer.Terminate();
	mTexture.Terminate();
	mSampler.Terminate();

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
	mVertexShader.Bind();
	mPixelShader.Bind();
	mTexture.BindPS(0);
	mTexture.BindPS(0);

	Math::Matrix4 matWorld = Math::Matrix4::Identity;
	Math::Matrix4 matView = mCamera.GetViewMatrix();
	Math::Matrix4 matProj = mCamera.GetProjectionMatrix();
	Math::Matrix4 matFinal = matWorld * matView * matProj;
	Math::Matrix4 wvp = Transpose(matFinal);

	mConstantBuffer.Update(&wvp);
	mConstantBuffer.BindVS(0);

	mMeshBuffer.Render();
}