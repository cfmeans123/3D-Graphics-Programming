#include "GameState.h"
#include "algorithm"

using namespace MEngine;
using namespace MEngine::Graphics;
using namespace MEngine::Math;
using namespace MEngine::Input;

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
}

void GameState::Initialize()
{
    mCamera.SetPosition({ 0.0f,1.0f,-4.0f });
    mCamera.SetLookAt({ 0.0f,0.0f,0.0f });

    mDirectionalLight.direction = Math::Normalize({ 1.0f,-1.0f,1.0f });
    mDirectionalLight.ambient = { 0.5f,0.5f,0.5f,1.0f };
    mDirectionalLight.diffuse = { 0.8f,0.8f,0.8f,1.0f };
    mDirectionalLight.specular = { 1.0f,1.0f,1.0f,1.0f };

    mModelID = ModelManager::Get()->LoadModel("../../Assets/Models/DrakeEnemy/Ch25_nonPBR.model");
    ModelManager::Get()->AddAnimation(mModelID, "../../Assets/Models/DrakeEnemy/Animations/BrutalAssassination.animset");
    ModelManager::Get()->AddAnimation(mModelID, "../../Assets/Models/DrakeEnemy/Animations/BrutalAssassination_2.animset");
    ModelManager::Get()->AddAnimation(mModelID, "../../Assets/Models/DrakeEnemy/Animations/FallingDown.animset");
    ModelManager::Get()->AddAnimation(mModelID, "../../Assets/Models/DrakeEnemy/Animations/FlyingKick.animset");
    mCharacter = CreateRenderGroup(mModelID, &mCharacterAnimator);
    mCharacterAnimator.Initialize(mModelID);
    mCharacterAnimator.SetNodeAnimation(BlendDirection::Idle, 0);
    mCharacterAnimator.SetNodeAnimation(BlendDirection::Forward, 1);
    mCharacterAnimator.SetNodeAnimation(BlendDirection::Left, 2);


    mStandardEffect.Initialize(L"../../Assets/Shaders/Standard.fx");
    mStandardEffect.SetCamera(mCamera);
    mStandardEffect.SetDirectionalLight(mDirectionalLight);
    AnimationUtil::ComputeBoneTransforms(mModelID, boneTransforms, &mCharacterAnimator);
    Mesh ground = MeshBuilder::CreateHorizontalPlane(10, 10, 1.0f);
    mGround.meshBuffer.Initialize(ground);
    mGround.diffuseMapID = TextureManager::Get()->LoadTexture("water/water_texture.jpg");
}

void GameState::Terminate()
{
    CleanRenderGroup(mCharacter);
    mStandardEffect.Terminate();
}

void GameState::Update(const float deltaTime)
{
    mCharacterAnimator.Update(deltaTime);

    UpdateCameraControl(deltaTime);
}

void GameState::Render()
{
    SimpleDraw::AddGroundPlane(10.0f, Colors::White);
    SimpleDraw::Render(mCamera);

    mStandardEffect.Begin();

    mStandardEffect.Render(mGround);
    Matrix4 transform = mCharacter[0].transform.GetMatrix4();
        
    
    //AnimationUtil::DrawSkeleton(mModelID, boneTransforms);
    SimpleDraw::AddSphere(4, 4, 0.03f, mTarget, Colors::MediumOrchid);
    SimpleDraw::AddSphere(4, 4, 0.03f, mTarget2, Colors::MediumOrchid);
    for (int i = 0; i < boneTransforms.size() - 1; ++i)
    {
        ModelManager::Get()->GetModel(mModelID)->skeleton.get()->bones[i]->boneTransform = boneTransforms[i];
    }
    if (mDrawSkeleton)
    {        
        /*for (auto& boneTransform : boneTransforms)
        {
            boneTransform = boneTransform * transform;
        }*/
        //populate bones with boneTransform values for solver
        if (startBoneIndex != 0 && endBoneIndex != 0)
        {
            if (two)
            {
                auto model = ModelManager::Get()->GetModel(mModelID);
                mIKChain2.SolveCCD(0.02f, 1, 10, mModelID, boneTransforms, &mCharacterAnimator);                
                mIKChain.SolveCCD(0.02f, 1, 10, mModelID, boneTransforms, &mCharacterAnimator);
            }
            DrawRenderGroup(mStandardEffect, mCharacter);
        }     
    }
    else
    {
        DrawRenderGroup(mStandardEffect, mCharacter);
    }
    mStandardEffect.End();
}

void GameState::DebugUI()
{
    ImGui::Begin("Debug control", nullptr, ImGuiWindowFlags_AlwaysAutoResize);
    if (ImGui::CollapsingHeader("Light", ImGuiTreeNodeFlags_DefaultOpen))
    {
        if (ImGui::DragFloat3("Direction", &mDirectionalLight.direction.x, 0.1f))
        {
            mDirectionalLight.direction = Math::Normalize(mDirectionalLight.direction);
        }

        ImGui::ColorEdit4("Ambient##Light", &mDirectionalLight.ambient.r);
        ImGui::ColorEdit4("Diffuse##Light", &mDirectionalLight.diffuse.r);
        ImGui::ColorEdit4("Specular##Light", &mDirectionalLight.specular.r);
        ImGui::DragFloat("Padding", &mDirectionalLight.padding, 0.1f, -2.0f, 2.0f);
    }
    ImGui::Checkbox("DrawSkeleton", &mDrawSkeleton);
    ImGui::Checkbox("Two", &two);
    if (ImGui::CollapsingHeader("Model Position", ImGuiTreeNodeFlags_DefaultOpen))
    {
        ImGui::DragFloat("X1", &mCharacter[0].transform.position.x, 0.1f, -2.0f, 2.0f);
        {
            mCharacter[1].transform.position.x = mCharacter[0].transform.position.x;
        }
        ImGui::DragFloat("Y1", &mCharacter[0].transform.position.y, 0.1f, -2.0f, 2.0f);
        {
            mCharacter[1].transform.position.y = mCharacter[0].transform.position.y;
        }
        ImGui::DragFloat("Z1", &mCharacter[0].transform.position.z, 0.1f, -2.0f, 2.0f);
        {
            mCharacter[1].transform.position.z = mCharacter[0].transform.position.z;
        }
    }
    if (ImGui::DragInt("AnimClip", &mAnimIndex, 1, -1, mCharacterAnimator.GetAnimationCount() - 1))
    {
        mCharacterAnimator.PlayAnimation(mAnimIndex, true, mBlendDuration);
    }

    if (ImGui::DragFloat("BlendDuration", &mBlendDuration, 0.1f, 0.0f, 3.0f))
    if (ImGui::DragFloat("BlendWeightIdle", &mBlendWeightIdle, 0.1f, 0.0f, 1.0f))
    {
        mCharacterAnimator.SetBlendWeight(BlendDirection::Idle, mBlendWeightIdle);
    }
    if (ImGui::DragFloat("BlendWeightForward", &mBlendWeightForward, 0.1f, 0.0f, 1.0f))
    {
        mCharacterAnimator.SetBlendWeight(BlendDirection::Forward, mBlendWeightForward);
    }
    if (ImGui::DragFloat("BlendWeightLeft", &mBlendWeightLeft, 0.1f, 0.0f, 1.0f))
    {
        mCharacterAnimator.SetBlendWeight(BlendDirection::Left, mBlendWeightLeft);
    }
    if (ImGui::CollapsingHeader("Target Position", ImGuiTreeNodeFlags_DefaultOpen))
    {
        ImGui::DragFloat("X", &mTarget.x, 0.01f, -20.0f, 20.0f);
        ImGui::DragFloat("Y", &mTarget.y, 0.01f, -20.0f, 20.0f);
        ImGui::DragFloat("Z", &mTarget.z, 0.01f, -20.0f, 20.0f);
        mIKChain.SetTarget(mTarget);
    }   
    if (ImGui::CollapsingHeader("Target Position 2", ImGuiTreeNodeFlags_DefaultOpen))
    {
        ImGui::DragFloat("X", &mTarget2.x, 0.01f, -20.0f, 20.0f);
        ImGui::DragFloat("Y", &mTarget2.y, 0.01f, -20.0f, 20.0f);
        ImGui::DragFloat("Z", &mTarget2.z, 0.01f, -20.0f, 20.0f);
        mIKChain2.SetTarget(mTarget2);
    }


    if (ImGui::BeginCombo("StartBone",
        ModelManager::Get()->GetModel(mModelID)->skeleton.get()->bones.front().get()->name.c_str(), ImGuiTreeNodeFlags_DefaultOpen))
    {
        for (int i = 0; i < ModelManager::Get()->GetModel(mModelID)->skeleton.get()->bones.size(); ++i)
        {
            const bool isSelected = (this->selectedIndexStart == i);
            if (ImGui::Selectable((ModelManager::Get()->GetModel(mModelID)->skeleton.get()->bones.at(i).get()->name.c_str()), isSelected)) {
                selectedIndexStart = i;
            }

            if (isSelected)
            {
                ImGui::SetItemDefaultFocus();
                startBoneIndex = selectedIndexStart;
                
            }
        }
    ImGui::EndCombo();
    }
    if (ImGui::BeginCombo("EndBone",
        ModelManager::Get()->GetModel(mModelID)->skeleton.get()->bones.front().get()->name.c_str(), ImGuiTreeNodeFlags_DefaultOpen))
    {
        for (int i = 0; i < ModelManager::Get()->GetModel(mModelID)->skeleton.get()->bones.size(); ++i)
        {
            const bool isSelected = (this->selectedIndexEnd == i);
            if (ImGui::Selectable((ModelManager::Get()->GetModel(mModelID)->skeleton.get()->bones.at(i).get()->name.c_str()), isSelected)) {
                selectedIndexEnd = i;
            }

            if (isSelected)
            {
                ImGui::SetItemDefaultFocus();
                endBoneIndex = selectedIndexEnd;

            }
        }
        ImGui::EndCombo();
    }

    if (ImGui::Button("Init IK Chain1"))
    {
        Skeleton* skeleton = ModelManager::Get()->GetModel(mModelID)->skeleton.get();
        if (mIKChain.GetNumIKJoints() != 0)
        {
            mIKChain.mIKJoints.clear();
            //mIKChain.SetRoot(nullptr);
        }
        std::string hipJointName = "Arm";
        std::string elbowJointName = "Arm";
        for (int i = selectedIndexStart; i <= selectedIndexEnd; ++i)
        {
            mIKChain.AddJoint(skeleton->bones.at(i).get());
            std::string found = mIKChain.mIKJoints[i - selectedIndexStart]->name;
            //skeleton->bones[i].get()->SetHingeConstraint(skeleton->bones.at(i).get()->GetAxis());
            skeleton->bones[i].get()->SetBallConstraint(-Math::pi * 0.25, Math::pi * 0.25);
            if (found.find(hipJointName) != std::string::npos)
            {
                //skeleton->bones[i].get()->SetHingeConstraint(Math::Vector3::ZAxis, -Math::pi * 0.15, Math::pi * 0.5);
                if (found.find(elbowJointName) != std::string::npos)
                {
                    //skeleton->bones[i].get()->SetHingeConstraint(Math::Vector3::ZAxis, 0, Math::pi * 0.5);
                }
            }
            else
            {
                mIKChain.mIKJoints[i-selectedIndexStart]->SetStatic();
            }
        }
        mIKChain.SetAnnealingExponent(0);

        mIKChain.SetTarget(mTarget);
        std::reverse(mIKChain.mIKJoints.begin(), mIKChain.mIKJoints.end());
        mIKChain.SetRoot(skeleton);
        mIKChain.SetEndEffector(skeleton->bones.at(selectedIndexEnd).get());
        mIKChain.SetLocalTransform(mCharacter[0].transform.GetMatrix4());
    }

    if (ImGui::Button("Init IK Chain2"))
    {
        Skeleton* skeleton = ModelManager::Get()->GetModel(mModelID)->skeleton.get();
        if (mIKChain2.GetNumIKJoints() != 0)
        {
            mIKChain2.mIKJoints.clear();
            //mIKChain.SetRoot(nullptr);
        }
        std::string hipJointName = "Arm";
        std::string elbowJointName = "Arm";
        for (int i = selectedIndexStart; i <= selectedIndexEnd; ++i)
        {
            mIKChain2.AddJoint(skeleton->bones.at(i).get());
            std::string found = mIKChain2.mIKJoints[i - selectedIndexStart]->name;
            //skeleton->bones[i].get()->SetHingeConstraint(skeleton->bones.at(i).get()->GetAxis());
            skeleton->bones[i].get()->SetBallConstraint(-Math::pi * 0.25, Math::pi * 0.25);
            if (found.find(hipJointName) != std::string::npos)
            {
                //skeleton->bones[i].get()->SetHingeConstraint(Math::Vector3::ZAxis, -Math::pi * 0.15, Math::pi * 0.5);
                if (found.find(elbowJointName) != std::string::npos)
                {
                    //skeleton->bones[i].get()->SetHingeConstraint(Math::Vector3::ZAxis, 0, Math::pi * 0.5);
                }
            }
            else
            {
                mIKChain2.mIKJoints[i - selectedIndexStart]->SetStatic();
            }
        }
        mIKChain2.SetAnnealingExponent(0);

        mIKChain2.SetTarget(mTarget2);
        std::reverse(mIKChain2.mIKJoints.begin(), mIKChain2.mIKJoints.end());
        mIKChain2.SetRoot(skeleton);
        mIKChain2.SetEndEffector(skeleton->bones.at(selectedIndexEnd).get());
        mIKChain2.SetLocalTransform(mCharacter[0].transform.GetMatrix4());
    }

    if (ImGui::Button("Get End Effector length to target"))
    {
        length = Math::Vector3::Length(mTarget - Math::Matrix4::GetPosition(boneTransforms[selectedIndexEnd]));;
    }
    int annealing = mIKChain.GetAnnealingExponent();
    if (ImGui::DragFloat("Length", &length, 0.0f, 0.0f, 0.0f))
    {
        mIKChain.SetAnnealingExponent(annealing);
    }
    if (ImGui::DragInt("Annealing", &annealing, 1, 0, 8))
    {
        mIKChain.SetAnnealingExponent(annealing);
    }
    
    //if (ImGui::ListBox("Joints", 3, ))
        
    mStandardEffect.DebugUI();
    ImGui::End();
}