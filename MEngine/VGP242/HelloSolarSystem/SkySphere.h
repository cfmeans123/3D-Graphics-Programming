#pragma once
#pragma once

#include <MEngine/Inc/MEngine.h>

using namespace MEngine;

class SkySphere
{
public:

    SkySphere(const std::string fpath, float size, float distance);

    void Terminate();
    void Update(float dt, float rotSpeed, float worldRot);
    void Render(const MEngine::Graphics::Camera& camera, MEngine::Graphics::ConstantBuffer& constantBuffer, Math::Matrix4& matView, Math::Matrix4& matProj);    


protected:
    Math::Matrix4 Transform();
    Math::Matrix4 WorldRotation();

private:
    Graphics::MeshPX mMesh;
    Graphics::MeshBuffer mMeshBuffer;
    Graphics::Texture mTexture;

    Math::Vector3 mPosition;
    Math::Vector3 mDirection;

    Math::Vector3 mWorldDirection;
};