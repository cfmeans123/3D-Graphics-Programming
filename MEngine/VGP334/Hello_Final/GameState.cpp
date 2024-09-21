#include "GameState.h"

using namespace MEngine;
using namespace MEngine::Graphics;
using namespace MEngine::Math;
using namespace MEngine::Input;
using namespace MEngine::Audio;

void GameState::UpdateCameraControl(float dt)
{
	auto input = Input::InputSystem::Get();
	const float moveSpeed = input->IsKeyDown(KeyCode::LSHIFT) ? 10.0f : 1.0f;
	const float turnSpeed = 0.8f;

	if (input->IsKeyDown(KeyCode::W))
	{
		mCamera.Walk(moveSpeed * dt);
	}
	if (input->IsKeyDown(KeyCode::A))
	{
		mCamera.Strafe(-moveSpeed * dt);
	}
	if (input->IsKeyDown(KeyCode::S))
	{
		mCamera.Walk(-moveSpeed * dt);
	}
	if (input->IsKeyDown(KeyCode::D))
	{
		mCamera.Strafe(moveSpeed * dt);
	}
	if (input->IsKeyDown(KeyCode::E))
	{
		mCamera.Rise(moveSpeed * dt);
	}
	if (input->IsKeyDown(KeyCode::Q))
	{
		mCamera.Rise(-moveSpeed * dt);
	}

	if (input->IsMouseDown(MouseButton::LBUTTON))
	{
		mCamera.Yaw(input->GetMouseMoveX() * turnSpeed * dt);
		mCamera.Pitch(input->GetMouseMoveY() * turnSpeed * dt);
	}

	if (input->IsKeyPressed(KeyCode::SPACE))
	{
		SpacePressedEvent spacePressed;
		EventManager::Broadcast(&spacePressed);
	}
}

void GameState::Initialize()
{
	mCamera.SetPosition({ 0.0f,1.0f,-4.0f });
	mCamera.SetLookAt({ 0.0f,0.0f,0.0f });

	mDirectionalLight.direction = Math::Normalize({ 0.5f,-1.0f,0.5f });
	mDirectionalLight.ambient = { 0.8f,0.8f,0.8f,1.0f };
	mDirectionalLight.diffuse = { 0.8f,0.8f,0.8f,1.0f };
	mDirectionalLight.specular = { 1.0f,1.0f,1.0f,1.0f };

	mStandardEffect.Initialize(L"../../Assets/Shaders/Standard.fx");
	mStandardEffect.SetCamera(mCamera);
	mStandardEffect.SetDirectionalLight(mDirectionalLight);
	{

		mCreatureOne = ModelManager::Get()->LoadModel("../../Assets/Models/DrakeEnemy/Drakey.model");
		ModelManager::Get()->AddAnimation(mCreatureOneID, "../../Assets/Models/DrakeEnemy/Animations/StrutWalking.animset");
		ModelManager::Get()->AddAnimation(mCreatureOneID, "../../Assets/Models/DrakeEnemy/Animations/Insult.animset");
		ModelManager::Get()->AddAnimation(mCreatureOneID, "../../Assets/Models/DrakeEnemy/Animations/HeadHit.animset");
		ModelManager::Get()->AddAnimation(mCreatureOneID, "../../Assets/Models/DrakeEnemy/Animations/Idle.animset");
		ModelManager::Get()->AddAnimation(mCreatureOneID, "../../Assets/Models/DrakeEnemy/Animations/Kicking.animset");
		ModelManager::Get()->AddAnimation(mCreatureOneID, "../../Assets/Models/DrakeEnemy/Animations/WarriorIdle.animset");
		ModelManager::Get()->AddAnimation(mCreatureOneID, "../../Assets/Models/DrakeEnemy/Animations/FallingDown.animset");
		ModelManager::Get()->AddAnimation(mCreatureOneID, "../../Assets/Models/DrakeEnemy/Animations/StandingUp.animset");
		ModelManager::Get()->AddAnimation(mCreatureOneID, "../../Assets/Models/DrakeEnemy/Animations/BrutalAssassination.animset");
		mCreatureOne = CreateRenderGroup(mCreatureOneID, &mCreatureOneAnimator);
		mCreatureOneAnimator.Initialize(mCreatureOneID);
		{
			mCreatureOneEventTime = 0.0f;
			mCreatureOneEvent = AnimationBuilder()
				.AddPositionKey({ -15.0f, 0.0f, 0.0f }, 0.0f)
				.AddRotationKey(Quaternion::CreateFromAxisAngle({ 0.0f, 1.0f, 0.0f }, 80.0f), 0.0f)
				.AddPositionKey({ -15.0f, 0.0f, 0.0f }, 0.1f)
				.AddEventKey([&]() {mCreatureOneAnimator.PlayAnimation(0, true); }, 0.1f)
				.AddPositionKey({ -1.0f, 0.0f, 0.0f }, 7.5f)
				.AddEventKey([&]() {mCreatureOneAnimator.PlayAnimation(1, true); }, 8.0f)
				.AddEventKey([&]() {mCreatureOneAnimator.PlayAnimation(2, false); }, 10.0f)
				.AddEventKey([&]() {mCreatureOneAnimator.PlayAnimation(3, true); }, 12.0f)
				.AddEventKey([&]() {mCreatureOneAnimator.PlayAnimation(4, false); }, 13.0f)
				.AddEventKey([&]() {mCreatureOneAnimator.PlayAnimation(5, true); }, 15.0f)
				.AddEventKey([&]() {mCreatureOneAnimator.PlayAnimation(6, false); }, 18.0f)
				.AddEventKey([&]() {mCreatureOneAnimator.PlayAnimation(7, false); }, 20.0f)
				.AddEventKey([&]() {mCreatureOneAnimator.PlayAnimation(8, false); }, 23.0f)
				.AddEventKey([&]() {mCreatureOneAnimator.PlayAnimation(3, true); }, 45.0f)
				//.AddPositionKey({ 0.0f, 0.0f, 0.0f }, 100.0f)
				.Build();
		}
	}
	{
		mGuyID = ModelManager::Get()->LoadModel("../../Assets/Models/DrakeEnemy/Drakey2.model");
		ModelManager::Get()->AddAnimation(mGuyID, "../../Assets/Models/DrakeEnemy/Animations/Walking.animset");
		ModelManager::Get()->AddAnimation(mGuyID, "../../Assets/Models/DrakeEnemy/Animations/ZombieAttack.animset");
		ModelManager::Get()->AddAnimation(mGuyID, "../../Assets/Models/DrakeEnemy/Animations/Idle.animset");
		ModelManager::Get()->AddAnimation(mGuyID, "../../Assets/Models/DrakeEnemy/Animations/LivershotKnockdown.animset");
		ModelManager::Get()->AddAnimation(mGuyID, "../../Assets/Models/DrakeEnemy/Animations/WarriorIdle.animset");
		ModelManager::Get()->AddAnimation(mGuyID, "../../Assets/Models/DrakeEnemy/Animations/RoundhouseKick.animset");
		ModelManager::Get()->AddAnimation(mGuyID, "../../Assets/Models/DrakeEnemy/Animations/BrutalAssassination_2.animset");
		mGuy = CreateRenderGroup(mGuyID, &mGuyAnimator);
		mGuyAnimator.Initialize(mGuyID);
		{
			mGuyEventTime = 0.0f;
			mGuyEvent = AnimationBuilder()
				.AddPositionKey({ 15.0f, 0.0f, 0.0f }, 0.0f)
				.AddRotationKey(Quaternion::CreateFromAxisAngle({ 0.0f, 1.0f, 0.0f }, -80.0f), 0.0f)
				.AddPositionKey({ 15.0f, 0.0f, 0.0f }, 0.1f)
				.AddEventKey([&]() {mGuyAnimator.PlayAnimation(0, true); }, 0.1f)
				.AddPositionKey({ 0.5f, 0.0f, 0.0f }, 7.5f)
				.AddEventKey([&]() {mGuyAnimator.PlayAnimation(2, false); }, 8.0f)
				.AddEventKey([&]() {mGuyAnimator.PlayAnimation(1, false); }, 9.3f)
				.AddEventKey([&]() {mGuyAnimator.PlayAnimation(2, true); }, 11.0f)
				.AddEventKey([&]() {mGuyAnimator.PlayAnimation(3, false); }, 13.8f)
				.AddEventKey([&]() {mGuyAnimator.PlayAnimation(4, true); }, 16.0f)
				.AddEventKey([&]() {mGuyAnimator.PlayAnimation(5, false); }, 17.0f)
				.AddEventKey([&]() {mGuyAnimator.PlayAnimation(2, true); }, 18.5f)
				.AddPositionKey({ 0.5f, 0.0f, 0.0f }, 18.6f)
				.AddPositionKey({ 1.0f, 0.0f, 0.2f }, 20.0f)
				.AddEventKey([&]() {mGuyAnimator.PlayAnimation(6, false); }, 23.3f)
				.AddEventKey([&]() {mGuyAnimator.PlayAnimation(2, true); }, 45.0f)
				
				
				//.AddRotationKey(Quaternion::CreateFromAxisAngle({ 0.0f, 1.0f, 0.0f }, 80.0f), 16.0f)
				//.AddPositionKey({ 0.0f, 0.0f, 0.0f }, 16.0f)
				//.AddPositionKey({ 15.0f, 0.0f, 0.0f }, 31.0f)
				//.AddPositionKey({ 0.0f, 0.0f, 0.0f }, 100.0f)
				.Build();
		}
	}

	Mesh obj = MeshBuilder::CreateHorizontalPlane(40, 40, 1.0f);
	mGround.meshBuffer.Initialize(obj);
	mGround.diffuseMapID = TextureManager::Get()->LoadTexture("misc/Concrete.jpg");
}

void GameState::Terminate()
{
	CleanRenderGroup(mBiker);
	CleanRenderGroup(mGuy);
	mStandardEffect.Terminate();
}

void GameState::Update(const float deltaTime)
{
	{
		float prevTime = mBikerEventTime;
		mBikerEventTime += deltaTime * mAnimSpeed;

		mBikerEvent.PlayEvents(prevTime, mBikerEventTime);
		while (mBikerEventTime >= mBikerEvent.GetDuration())
		{
			mBikerEventTime -= mBikerEvent.GetDuration();
		}
	}
	{
		float prevTime = mGuyEventTime;
		mGuyEventTime += deltaTime * mAnimSpeed;

		mGuyEvent.PlayEvents(prevTime, mGuyEventTime);
		while (mGuyEventTime >= mGuyEvent.GetDuration())
		{
			mGuyEventTime -= mGuyEvent.GetDuration();
		}
	}

	mBikerAnimator.Update(deltaTime);
	mGuyAnimator.Update(deltaTime);

	UpdateCameraControl(deltaTime);
}

void GameState::Render()
{
	for (auto& ro : mBiker)
	{
		ro.transform = mBikerEvent.GetTransform(mBikerEventTime);
	}
	for (auto& ro : mGuy)
	{
		ro.transform = mGuyEvent.GetTransform(mGuyEventTime);
	}

	mStandardEffect.Begin();
	{
		DrawRenderGroup(mStandardEffect, mBiker);
		DrawRenderGroup(mStandardEffect, mGuy);
		mStandardEffect.Render(mGround);
	}
	mStandardEffect.End();
}

void GameState::DebugUI()
{
	ImGui::Begin("Debug control", nullptr, ImGuiWindowFlags_AlwaysAutoResize);
	if (ImGui::CollapsingHeader("Debug", ImGuiTreeNodeFlags_DefaultOpen))
	{
		ImGui::DragFloat("AnimSpeed", &mAnimSpeed, 0.1f);
	}
	if (ImGui::CollapsingHeader("Light", ImGuiTreeNodeFlags_DefaultOpen))
	{
		if (ImGui::DragFloat3("Direction", &mDirectionalLight.direction.x, 0.1f))
		{
			mDirectionalLight.direction = Math::Normalize(mDirectionalLight.direction);
		}

		ImGui::ColorEdit4("Ambient##Light", &mDirectionalLight.ambient.r);
		ImGui::ColorEdit4("Diffuse##Light", &mDirectionalLight.diffuse.r);
		ImGui::ColorEdit4("Specular##Light", &mDirectionalLight.specular.r);
	}
	ImGui::End();
}