#include "SkySphere.h"

using namespace MEngine::Graphics;

SkySphere::SkySphere(const std::string fpath, float size, float distance)
{
	mMesh = mMesh = MEngine::Graphics::MeshBuilder::CreateSkySpherePX(100, 100, size);
	mTexture.Initialize(fpath);
	mMeshBuffer.Initialize(mMesh);

	mPosition = { 0.0f, 0.0f, distance };
	mDirection = { 0.0f, 0.0f, 1.0f };

	mWorldDirection = { 0.0f, 0.0f, 1.0f };
}

//mMesh = MeshBuilder::CreateSkySpherePX(100, 100, 100.0f);
//
//std::filesystem::path shaderFilePath = L"../../Assets/Shaders/DoTexturing.fx";
//mConstantBuffer.Initialize(sizeof(Math::Matrix4));
//mMeshBuffer.Initialize(mMesh);
//mVertexShader.Initialize<VertexPX>(shaderFilePath);
//mPixelShader.Initialize(shaderFilePath);
//mTexture.Initialize(L"../../Assets/Images/skysphere/space.jpg");
//mSampler.Initialize(Sampler::Filter::Linear, Sampler::AddressMode::Wrap);


void SkySphere::Terminate()
{
	mTexture.Terminate();
	mMeshBuffer.Terminate();
}

void SkySphere::Update(float dt, float rotSpeed, float worldRot)
{
	/*Math::Matrix4 matRotate = Math::Matrix4::RotationY(1.0f * dt * rotSpeed);
	mDirection = Math::TransformNormal(mDirection, matRotate);

	matRotate = Math::Matrix4::RotationY(1.0f * dt * worldRot);
	mWorldDirection = Math::TransformNormal(mWorldDirection, matRotate);*/
}

void SkySphere::Render(const MEngine::Graphics::Camera& camera, MEngine::Graphics::ConstantBuffer& constantBuffer, Math::Matrix4& matView, Math::Matrix4& matProj)
{
	mTexture.BindPS(0);

	Math::Matrix4 matWorld = WorldRotation();
	Math::Matrix4 local = Transform();

	Math::Matrix4 matFinal = matWorld * local * matView * matProj;
	Math::Matrix4 wvp = Transpose(matFinal);

	constantBuffer.Update(&wvp);
	constantBuffer.BindVS(0);

	mMeshBuffer.Render();
}

Math::Matrix4 SkySphere::Transform()
{
	const Math::Vector3 l = mDirection;
	const Math::Vector3 r = Math::Normalize(Math::Cross(Math::Vector3::YAxis, l));
	const Math::Vector3 u = Math::Normalize(Math::Cross(l, r));
	const float x = -Math::Dot(r, mPosition);
	const float y = -Math::Dot(u, mPosition);
	const float z = -Math::Dot(l, mPosition);

	return
	{
		r.x, u.x, l.x, 0.0f,
		r.y, u.y, l.y, 0.0f,
		r.z, u.z, l.z, 0.0f,
		x,   y,   z,   1.0f
	};
}

Math::Matrix4 SkySphere::WorldRotation()
{
	Math::Vector3 position(0.0f, 0.0f, 0.0f);

	const Math::Vector3 l = mWorldDirection;
	const Math::Vector3 r = Math::Normalize(Math::Cross(Math::Vector3::YAxis, l));
	const Math::Vector3 u = Math::Normalize(Math::Cross(l, r));
	const float x = -Math::Dot(r, position);
	const float y = -Math::Dot(u, position);
	const float z = -Math::Dot(l, position);

	return
	{
		r.x, u.x, l.x, 0.0f,
		r.y, u.y, l.y, 0.0f,
		r.z, u.z, l.z, 0.0f,
		x,   y,   z,   1.0f
	};
}
