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
    Mesh ground = MeshBuilder::CreateHorizontalPlane(100, 100, 1.0f);
    mGround.meshBuffer.Initialize(ground);
    mGround.diffuseMapID = TextureManager::Get()->LoadTexture("water/water_texture.jpg");
    InitIKChains();
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
    //SimpleDraw::AddGroundPlane(10.0f, Colors::White);
    SimpleDraw::Render(mCamera);

    mStandardEffect.Begin();

    mStandardEffect.Render(mGround);
    Matrix4 transform = mCharacter[0].transform.GetMatrix4();
        
    
    SimpleDraw::AddSphere(4, 4, 0.03f, mTarget, Colors::MediumOrchid);
    SimpleDraw::AddSphere(4, 4, 0.03f, mTarget2, Colors::MediumOrchid);
    SimpleDraw::AddSphere(4, 4, 0.03f, mTarget3, Colors::MediumOrchid);
    SimpleDraw::AddSphere(4, 4, 0.03f, mTarget4, Colors::MediumOrchid);
    for (int i = 0; i < boneTransforms.size() - 1; ++i)
    {
        ModelManager::Get()->GetModel(mModelID)->skeleton.get()->bones[i]->boneTransform = boneTransforms[i];
    }
       
    
    //populate bones with boneTransform values for solver

    if (two)
    {
        auto model = ModelManager::Get()->GetModel(mModelID);
        mIKChain2.SolveCCD(0.02f, 1, 10, mModelID, boneTransforms, &mCharacterAnimator);                
        mIKChain.SolveCCD(0.02f, 1, 10, mModelID, boneTransforms, &mCharacterAnimator);

        mIKChain4.SolveCCD(0.02f, 1, 10, mModelID, boneTransforms, &mCharacterAnimator);
        mIKChain3.SolveCCD(0.02f, 1, 10, mModelID, boneTransforms, &mCharacterAnimator);
    }
 

    if (mDrawSkeleton)
    {
        AnimationUtil::DrawSkeleton(mModelID, boneTransforms);
        for (auto& boneTransform : boneTransforms)
        {
            boneTransform = boneTransform * transform;
        }
    }
    else
    {
        DrawRenderGroup(mStandardEffect, mCharacter);
    }
    mStandardEffect.End();
}

void GameState::InitIKChains()
{
    Skeleton* skeleton = ModelManager::Get()->GetModel(mModelID)->skeleton.get();
//init IK Chain 1
    //Magic index numbers are bone joint indices, 9 for leftshoulder and 12 for lefthand
    selectedIndexEnd = 12;
    for (int i = 9; i <= 12; ++i)
    {
        mIKChain.AddJoint(skeleton->bones.at(i).get());       
        skeleton->bones[i].get()->SetBallConstraint(-Math::pi * 0.25, Math::pi * 0.25);        
    }
    mIKChain.SetAnnealingExponent(2);

    mIKChain.SetTarget(mTarget);
    std::reverse(mIKChain.mIKJoints.begin(), mIKChain.mIKJoints.end());
    mIKChain.SetRoot(skeleton);
    mIKChain.SetEndEffector(skeleton->bones.at(selectedIndexEnd).get());
    mIKChain.SetLocalTransform(mCharacter[0].transform.GetMatrix4());

//Init IK Chain 2
    //Magic index numbers are bone joint indices, 33 for rightshoulder and 36 for righthand
    selectedIndexEnd = 36;
    for (int i = 33; i <= 36; ++i)
    {
        mIKChain2.AddJoint(skeleton->bones.at(i).get());
        skeleton->bones[i].get()->SetBallConstraint(-Math::pi * 0.25, Math::pi * 0.25);       
    }
    mIKChain2.SetAnnealingExponent(2);

    mIKChain2.SetTarget(mTarget2);
    std::reverse(mIKChain2.mIKJoints.begin(), mIKChain2.mIKJoints.end());
    mIKChain2.SetRoot(skeleton);
    mIKChain2.SetEndEffector(skeleton->bones.at(selectedIndexEnd).get());
    mIKChain2.SetLocalTransform(mCharacter[0].transform.GetMatrix4());

//Init IK Chain 3
    //Magic index numbers are bone joint indices, 33 for rightshoulder and 36 for righthand   
    
    //56 is LeftLegThigh
    mIKChain3.AddJoint(skeleton->bones.at(56).get());    
    skeleton->bones[56].get()->SetBallConstraint(-Math::pi * 0.005, Math::pi * 0.005);
    
    //57 is LeftLegKnee
    mIKChain3.AddJoint(skeleton->bones.at(57).get());
    //skeleton->bones[57]->SetHingeConstraint(Math::Vector3::YAxis, -Math::pi , Math::pi);


    //58 is LeftLegFoot
    mIKChain3.AddJoint(skeleton->bones.at(58).get());
    skeleton->bones[57].get()->SetBallConstraint(-Math::pi * 0.005, Math::pi * 0.005);

    mIKChain3.SetAnnealingExponent(0);

    mIKChain3.SetTarget(mTarget3);
    std::reverse(mIKChain3.mIKJoints.begin(), mIKChain3.mIKJoints.end());
    mIKChain3.SetRoot(skeleton);
    mIKChain3.SetEndEffector(skeleton->bones.at(59).get());
    mIKChain3.SetLocalTransform(mCharacter[0].transform.GetMatrix4());

//Init IK Chain 4
    //Magic index numbers are bone joint indices, 33 for rightshoulder and 36 for righthand

    //61 is RightLegThigh
    mIKChain4.AddJoint(skeleton->bones.at(61).get());
    skeleton->bones[61].get()->SetBallConstraint(-Math::pi * 0.15, Math::pi * 0.5);

    //62 is RightLegThigh
    mIKChain4.AddJoint(skeleton->bones.at(62).get());
    //skeleton->bones[62].get()->SetBallConstraint(-Math::pi * 0.15, Math::pi * 0.5);
   
    //63 is RightLegKnee
    mIKChain4.AddJoint(skeleton->bones.at(63).get());
    skeleton->bones[63].get()->SetBallConstraint(-Math::pi * 0.25, Math::pi * 0.15);

   


    mIKChain4.SetAnnealingExponent(0);
    mIKChain4.SetTarget(mTarget4);
    std::reverse(mIKChain4.mIKJoints.begin(), mIKChain4.mIKJoints.end());
    mIKChain4.SetRoot(skeleton);
    mIKChain4.SetEndEffector(skeleton->bones.at(64).get());
    mIKChain4.SetLocalTransform(mCharacter[0].transform.GetMatrix4());

};

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
    ImGui::Checkbox("Activate IK", &two);
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
    if (ImGui::CollapsingHeader("Target Position Left Hand", ImGuiTreeNodeFlags_DefaultOpen))
    {
        ImGui::DragFloat("XP1", &mTarget.x, 0.01f, -20.0f, 20.0f);
        ImGui::DragFloat("YP1", &mTarget.y, 0.01f, -20.0f, 20.0f);
        ImGui::DragFloat("ZP1", &mTarget.z, 0.01f, -20.0f, 20.0f);
        mIKChain.SetTarget(mTarget);
    }
    if (ImGui::CollapsingHeader("Target Position Right Hand", ImGuiTreeNodeFlags_DefaultOpen))
    {
        ImGui::DragFloat("XP2", &mTarget2.x, 0.01f, -20.0f, 20.0f);
        ImGui::DragFloat("YP2", &mTarget2.y, 0.01f, -20.0f, 20.0f);
        ImGui::DragFloat("ZP2", &mTarget2.z, 0.01f, -20.0f, 20.0f);
        mIKChain.SetTarget(mTarget);
    }

    if (ImGui::CollapsingHeader("Target Position Both Hands", ImGuiTreeNodeFlags_DefaultOpen))
    {
        ImGui::DragFloat3("TargetPos", &offsetHands.x, 0.01f, -20.0f, 20.0f);
        mTarget += offsetHands;
        mTarget2 += offsetHands;
        mIKChain.SetTarget(mTarget);
        mIKChain2.SetTarget(mTarget2);
        offsetHands = Vector3::Zero;
    }

    if (ImGui::CollapsingHeader("Target Position Both Feet", ImGuiTreeNodeFlags_DefaultOpen))
    {
        ImGui::DragFloat3("TargetPosFeet", &offsetFeet.x, 0.01f, -20.0f, 20.0f);
        mTarget3 += offsetFeet;
        mTarget4 += offsetFeet;
        mIKChain3.SetTarget(mTarget3);
        mIKChain4.SetTarget(mTarget4);
        offsetFeet = Vector3::Zero;
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

    if (ImGui::Button("Init IK Arms"))
    {
        Skeleton* skeleton = ModelManager::Get()->GetModel(mModelID)->skeleton.get();

        //init IK Chain 1
        for (int i = 9; i <= 12; ++i)
        {
            mIKChain.AddJoint(skeleton->bones.at(i).get());
            skeleton->bones[i].get()->SetBallConstraint(-Math::pi * 0.25, Math::pi * 0.25);
        }
        mIKChain.SetAnnealingExponent(2);

        mIKChain.SetTarget(mTarget);
        std::reverse(mIKChain.mIKJoints.begin(), mIKChain.mIKJoints.end());
        mIKChain.SetRoot(skeleton);
        mIKChain.SetEndEffector(skeleton->bones.at(selectedIndexEnd).get());
        mIKChain.SetLocalTransform(mCharacter[0].transform.GetMatrix4());

        //Init IK Chain 2
        for (int i = 33; i <= 36; ++i)
        {
            mIKChain2.AddJoint(skeleton->bones.at(i).get());
            skeleton->bones[i].get()->SetBallConstraint(-Math::pi * 0.25, Math::pi * 0.25);
        }
        mIKChain2.SetAnnealingExponent(2);

        mIKChain2.SetTarget(mTarget2);
        std::reverse(mIKChain2.mIKJoints.begin(), mIKChain2.mIKJoints.end());
        mIKChain2.SetRoot(skeleton);
        mIKChain2.SetEndEffector(skeleton->bones.at(selectedIndexEnd).get());
        mIKChain2.SetLocalTransform(mCharacter[0].transform.GetMatrix4());
    }

    if (ImGui::Button("Init IK Chain1"))
    {
        Skeleton* skeleton = ModelManager::Get()->GetModel(mModelID)->skeleton.get();
        if (mIKChain.GetNumIKJoints() != 0)
        {
            mIKChain.mIKJoints.clear();
            //mIKChain.SetRoot(nullptr);
        }
        std::string hipJointName = "Left";
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
        std::string hipJointName = "Right";
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
   
    int annealing = mIKChain.GetAnnealingExponent();
    if (ImGui::DragFloat("Length", &length, 0.0f, 0.0f, 0.0f))
    {
        //mIKChain.SetAnnealingExponent(annealing);
    }
    if (ImGui::DragInt("Annealing", &annealing, 1, 0, 8))
    {
        mIKChain.SetAnnealingExponent(annealing);
    }

    if (ImGui::Button("Get End Effector length to target"))
    {
        length = Math::Vector3::Length(mTarget - Math::Matrix4::GetPosition(boneTransforms[selectedIndexEnd]));;
    }

    int annealing2 = mIKChain2.GetAnnealingExponent();
    if (ImGui::DragInt("Annealing", &annealing2, 1, 0, 8))
    {
        mIKChain2.SetAnnealingExponent(annealing2);
    }

    if (ImGui::Button("Get End Effector length to target"))
    {
        length = Math::Vector3::Length(mTarget2 - Math::Matrix4::GetPosition(boneTransforms[selectedIndexEnd]));;
    }

    mStandardEffect.DebugUI();
    ImGui::End();

   
}