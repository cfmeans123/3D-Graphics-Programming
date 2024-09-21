#include "GameState.h"

using namespace MEngine;
using namespace MEngine::Graphics;
using namespace MEngine::Physics;
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

		mCreatureOneID = ModelManager::Get()->LoadModel("../../Assets/Models/DrakeEnemy/Drakey.model");
		ModelManager::Get()->AddAnimation(mCreatureOneID, "../../Assets/Models/DrakeEnemy/Animations/StrutWalking.animset");
		ModelManager::Get()->AddAnimation(mCreatureOneID, "../../Assets/Models/DrakeEnemy/Animations/Insult.animset");
		ModelManager::Get()->AddAnimation(mCreatureOneID, "../../Assets/Models/DrakeEnemy/Animations/HeadHit.animset");
		ModelManager::Get()->AddAnimation(mCreatureOneID, "../../Assets/Models/DrakeEnemy/Animations/Idle.animset");
		ModelManager::Get()->AddAnimation(mCreatureOneID, "../../Assets/Models/DrakeEnemy/Animations/Kicking.animset");
		ModelManager::Get()->AddAnimation(mCreatureOneID, "../../Assets/Models/DrakeEnemy/Animations/WarriorIdle.animset");
		ModelManager::Get()->AddAnimation(mCreatureOneID, "../../Assets/Models/DrakeEnemy/Animations/FallingDown.animset");
		ModelManager::Get()->AddAnimation(mCreatureOneID, "../../Assets/Models/DrakeEnemy/Animations/StandingUp.animset");
		ModelManager::Get()->AddAnimation(mCreatureOneID, "../../Assets/Models/DrakeEnemy/Animations/BrutalAssassination.animset");
		ModelManager::Get()->AddAnimation(mCreatureOneID, "../../Assets/Models/DrakeEnemy/Animations/Laughing.animset");
		ModelManager::Get()->AddAnimation(mCreatureOneID, "../../Assets/Models/DrakeEnemy/Animations/StumbleBackwards.animset");
		ModelManager::Get()->AddAnimation(mCreatureOneID, "../../Assets/Models/DrakeEnemy/Animations/LayingVomiting.animset");
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
				.AddEventKey([&]() {mCreatureOneAnimator.PlayAnimation(9, false); }, 28.0f)
				.AddEventKey([&]() {mCreatureOneAnimator.PlayAnimation(10, false); }, 34.1f)
				.AddEventKey([&]() {mCreatureOneAnimator.PlayAnimation(10, false); }, 34.1f)
				.AddEventKey([&]() {mCreatureOneAnimator.PlayAnimation(11, false); }, 37.0f)
				.AddEventKey([&]() {mCamera.SetPosition({ -1.0f, 1.5f, 0.0f }); mCamera.Yaw(-1.0f); }, 37.0f)
				.AddPositionKey({ -1.0f, 0.0f, 0.0f }, 37.0f)
				.AddPositionKey({ -1.8f, 0.0f, 0.0f }, 37.0f)
				.AddEventKey([&]() {mCreatureOneAnimator.PlayAnimation(3, true); }, 105.0f)
				//.AddPositionKey({ 0.0f, 0.0f, 0.0f }, 100.0f)
				.Build();
		}
	}
	{
		mCreatureTwoID = ModelManager::Get()->LoadModel("../../Assets/Models/DrakeEnemy/Drakey2.model");
		ModelManager::Get()->AddAnimation(mCreatureTwoID, "../../Assets/Models/DrakeEnemy/Animations/Walking.animset");
		ModelManager::Get()->AddAnimation(mCreatureTwoID, "../../Assets/Models/DrakeEnemy/Animations/ZombieAttack.animset");
		ModelManager::Get()->AddAnimation(mCreatureTwoID, "../../Assets/Models/DrakeEnemy/Animations/Idle.animset");
		ModelManager::Get()->AddAnimation(mCreatureTwoID, "../../Assets/Models/DrakeEnemy/Animations/LivershotKnockdown.animset");
		ModelManager::Get()->AddAnimation(mCreatureTwoID, "../../Assets/Models/DrakeEnemy/Animations/WarriorIdle.animset");
		ModelManager::Get()->AddAnimation(mCreatureTwoID, "../../Assets/Models/DrakeEnemy/Animations/RoundhouseKick.animset");
		ModelManager::Get()->AddAnimation(mCreatureTwoID, "../../Assets/Models/DrakeEnemy/Animations/BrutalAssassination_2.animset");
		ModelManager::Get()->AddAnimation(mCreatureTwoID, "../../Assets/Models/DrakeEnemy/Animations/GettingUpFront.animset");
		ModelManager::Get()->AddAnimation(mCreatureTwoID, "../../Assets/Models/DrakeEnemy/Animations/FlyingKick.animset");
		ModelManager::Get()->AddAnimation(mCreatureTwoID, "../../Assets/Models/DrakeEnemy/Animations/HardLanding.animset");
		mCreatureTwo = CreateRenderGroup(mCreatureTwoID, &mCreatureTwoAnimator);
		mCreatureTwoAnimator.Initialize(mCreatureTwoID);
		{
			mCreatureTwoEventTime = 0.0f;
			mCreatureTwoEvent = AnimationBuilder()
				.AddPositionKey({ 15.0f, 0.0f, 0.0f }, 0.0f)
				.AddRotationKey(Quaternion::CreateFromAxisAngle({ 0.0f, 1.0f, 0.0f }, -80.0f), 0.0f)
				.AddPositionKey({ 15.0f, 0.0f, 0.0f }, 0.1f)
				.AddEventKey([&]() {mCreatureTwoAnimator.PlayAnimation(0, true); }, 0.1f)
				.AddPositionKey({ 0.5f, 0.0f, 0.0f }, 7.5f)
				.AddEventKey([&]() {mCreatureTwoAnimator.PlayAnimation(2, false); }, 8.0f)
				.AddEventKey([&]() {mCreatureTwoAnimator.PlayAnimation(1, false); }, 9.3f)
				.AddEventKey([&]() {mCreatureTwoAnimator.PlayAnimation(2, true); }, 11.0f)
				.AddEventKey([&]() {mCreatureTwoAnimator.PlayAnimation(3, false); }, 13.8f)
				.AddEventKey([&]() {mCreatureTwoAnimator.PlayAnimation(4, true); }, 16.0f)
				.AddEventKey([&]() {mCreatureTwoAnimator.PlayAnimation(5, false); }, 17.0f)
				.AddEventKey([&]() {mCreatureTwoAnimator.PlayAnimation(2, true); }, 18.5f)
				.AddPositionKey({ 0.5f, 0.0f, 0.0f }, 19.f)
				.AddPositionKey({ 1.0f, 0.0f, 0.2f }, 20.0f)
				.AddEventKey([&]() {mCreatureTwoAnimator.PlayAnimation(6, false); }, 23.0f)
				.AddPositionKey({ 0.9f, 0.0f, 0.2f }, 23.0f)
				.AddPositionKey({ 0.5f, 0.0f, 0.0f }, 28.1f)
				.AddEventKey([&]() {mCreatureTwoAnimator.PlayAnimation(7, false); }, 28.1f)
				.AddEventKey([&]() {mCreatureTwoAnimator.PlayAnimation(8, false); }, 34.0f)
				.AddEventKey([&]() {mCreatureTwoAnimator.PlayAnimation(9, false); }, 37.0f)
				.AddEventKey([&]() {mCreatureTwoAnimator.PlayAnimation(9, false); }, 105.0f)
				
				//.AddRotationKey(Quaternion::CreateFromAxisAngle({ 0.0f, 1.0f, 0.0f }, 80.0f), 16.0f)
				//.AddPositionKey({ 0.0f, 0.0f, 0.0f }, 16.0f)
				//.AddPositionKey({ 15.0f, 0.0f, 0.0f }, 31.0f)
				//.AddPositionKey({ 0.0f, 0.0f, 0.0f }, 100.0f)
				.Build();
		}
	}
	{
		mParticleEffect.Initialize();
		mParticleEffect.SetCamera(mCamera);

		ParticleSystemInfo info;
		info.maxParticle = 100;
		info.particleTextureId = TextureManager::Get()->LoadTexture("splat.png");
		info.spawnPosition = Math::Vector3::Zero;
		info.spawnDirection = Math::Vector3::YAxis;
		info.spawnDelay = 0.05f;
		info.spawnLifeTime = 99999999999.0f;
		info.minParticlesPerEmit = 30;
		info.maxParticlesPerEmit = 40;
		info.minTimeBetweenEmit = 0.05f;
		info.maxTimeBetweenEmit = 0.45f;
		info.minSpawnAngle = 1 * MEngine::Math::Constants::DegToRad;
		info.maxSpawnAngle = 360 * MEngine::Math::Constants::DegToRad;
		info.minSpeed = 1.0f;
		info.maxSpeed = 5.0f;
		info.minParticleLifeTime = 0.5f;
		info.maxParticleLifeTime = 2.0f;
		/*info.minStartColor = Colors::Red;
		info.maxStartColor = Colors::Red;*/
		info.minEndColor.a = 0.0f;
		info.maxEndColor = Colors::Red;
		info.minStartScale = Math::Vector3::One;
		info.maxStartScale = { 1.5f, 1.5f,1.5f };
		info.minEndScale = { 0.15f, 0.15f, 0.15f };
		info.maxEndScale = { 1.5f, 1.5f,1.5f };
		mParticleSystem.Initialize(info);
		mParticleSystem.SetCamera(mCamera);
		mParticleSystem.SetPosition({ -1.8f, 1.0f, 0.0f } );
	}

	Mesh obj = MeshBuilder::CreateHorizontalPlane(70, 70, 1.0f);
	Mesh spaceObj = MeshBuilder::CreateSkySphere(200, 200, 50.0f);
	mGround.meshBuffer.Initialize(obj);
	mSpace.meshBuffer.Initialize(spaceObj);
	mGround.diffuseMapID = TextureManager::Get()->LoadTexture("misc/Concrete.jpg");
	mSpace.diffuseMapID = TextureManager::Get()->LoadTexture("../../Assets/Images/skysphere/space.jpg");
}

void GameState::Terminate()
{
	mGround.Terminate();
	mSpace.Terminate();
	CleanRenderGroup(mCreatureOne);
	CleanRenderGroup(mCreatureTwo);
	mStandardEffect.Terminate();
	mParticleEffect.Terminate();
	mParticleSystem.Terminate();
}

void GameState::Update(const float deltaTime)
{
	{
		float prevTime = mCreatureOneEventTime;
		mCreatureOneEventTime += deltaTime * mAnimSpeed;

		mCreatureOneEvent.PlayEvents(prevTime, mCreatureOneEventTime);
		while (mCreatureOneEventTime >= mCreatureOneEvent.GetDuration())
		{
			mCreatureOneEventTime -= mCreatureOneEvent.GetDuration();
		}
	}
	{
		float prevTime = mCreatureTwoEventTime;
		mCreatureTwoEventTime += deltaTime * mAnimSpeed;

		mCreatureTwoEvent.PlayEvents(prevTime, mCreatureTwoEventTime);
		while (mCreatureTwoEventTime >= mCreatureTwoEvent.GetDuration())
		{
			mCreatureTwoEventTime -= mCreatureTwoEvent.GetDuration();
		}
	}

	mCreatureOneAnimator.Update(deltaTime);
	mCreatureTwoAnimator.Update(deltaTime);	
	mParticleSystem.Update(deltaTime);
	UpdateCameraControl(deltaTime);
}

void GameState::Render()
{
	for (auto& ro : mCreatureOne)
	{
		ro.transform = mCreatureOneEvent.GetTransform(mCreatureOneEventTime);
	}
	for (auto& ro : mCreatureTwo)
	{
		ro.transform = mCreatureTwoEvent.GetTransform(mCreatureTwoEventTime);
	}

	mStandardEffect.Begin();
	{
		DrawRenderGroup(mStandardEffect, mCreatureOne);
		DrawRenderGroup(mStandardEffect, mCreatureTwo);
		mStandardEffect.Render(mGround);
		mStandardEffect.Render(mSpace);
	}

	mStandardEffect.End();

	if (mCreatureOneEventTime > 37.3f && mCreatureOneEventTime < 38.3f)
	{
		mParticleEffect.Begin();
		mParticleSystem.Render(mParticleEffect);
		mParticleEffect.End();
	}
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