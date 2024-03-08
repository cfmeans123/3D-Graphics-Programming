#pragma once

#include <MEngine/Inc/MEngine.h>

using namespace MEngine;

class SpaceOrb
{
public:
    enum OrbName
    {
        Sun,
        Mercury,
        Venus,
        Earth,
        Mars,
        Jupiter,
        Saturn,
        Uranus,
        Neptune,
        Pluto
    };

    SpaceOrb(const std::string fpath, float size, float distance, OrbName name);

    void Terminate();
    void Update(float dt, float rotSpeed, float worldRot);
    void Render(const MEngine::Graphics::Camera& camera, MEngine::Graphics::ConstantBuffer& constantBuffer, Math::Matrix4& matView, Math::Matrix4& matProj);
    OrbName mName;


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