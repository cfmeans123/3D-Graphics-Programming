#include "Precompiled.h"
#include "../Inc/SimpleDraw.h"

#include "../Inc/Camera.h"
#include "../Inc/ConstantBuffer.h"
#include "../Inc/MeshBuffer.h"
#include "../Inc/PixelShader.h"
#include "../Inc/VertexShader.h"
#include "../Inc/VertexTypes.h"

using namespace MEngine;
using namespace MEngine::Graphics;
using namespace MEngine::Math::Constants;

namespace
{
	class SimpleDrawImpl
	{
	public:
		void Initialize(uint32_t maxVertexCount);
		void Terminate();

		void AddLine(const Vector3& v0, const Vector3& v1, const Color& color);
		void AddFace(const Vector3& v0, const Vector3& v1, const Vector3& v2, const Color& color);

		void Render(const Camera& camera);

	private:
		VertexShader mVertexShader;
		PixelShader mPixelShader;
		ConstantBuffer mConstantBuffer;
		MeshBuffer mMeshBuffer;

		std::unique_ptr<VertexPC[]> mLineVertices;
		std::unique_ptr<VertexPC[]> mFaceVertices;
		uint32_t mLineVertexCount = 0;
		uint32_t mFaceVertexCount = 0;
		uint32_t mMaxVertexCount = 0;
	};
	void SimpleDrawImpl::Initialize(uint32_t maxVertexCount)
	{
		std::filesystem::path shaderPath = L"../../Assets/Shaders/DoTransform.fx"; //chance to SimpleDraw.fx
		mVertexShader.Initialize<VertexPC>(shaderPath);
		mPixelShader.Initialize(shaderPath);
		mConstantBuffer.Initialize(sizeof(Matrix4));
		mMeshBuffer.Initialize(nullptr, sizeof(VertexPC), maxVertexCount);

		mLineVertices = std::make_unique<VertexPC[]>(maxVertexCount);
		mFaceVertices = std::make_unique<VertexPC[]>(maxVertexCount);
		mLineVertexCount = 0;
		mFaceVertexCount = 0;
		mMaxVertexCount = maxVertexCount;
	}
	void SimpleDrawImpl::Terminate()
	{
		mMeshBuffer.Terminate();
		mConstantBuffer.Terminate();
		mPixelShader.Terminate();
		mVertexShader.Terminate();
	}
	void SimpleDrawImpl::AddLine(const Vector3& v0, const Vector3& v1, const Color& color)
	{
		if (mLineVertexCount + 2 <= mMaxVertexCount)
		{
			mLineVertices[mLineVertexCount++] = VertexPC{ v0, color };
			mLineVertices[mLineVertexCount++] = VertexPC{ v1, color };
		}
	}
	void SimpleDrawImpl::AddFace(const Vector3& v0, const Vector3& v1, const Vector3& v2, const Color& color)
	{
		if (mFaceVertexCount + 3 <= mMaxVertexCount)
		{
			mFaceVertices[mFaceVertexCount++] = VertexPC{ v0, color };
			mFaceVertices[mFaceVertexCount++] = VertexPC{ v1, color };
			mFaceVertices[mFaceVertexCount++] = VertexPC{ v2, color };
		}
	}
	void SimpleDrawImpl::Render(const Camera& camera)
	{
		const Matrix4 matView = camera.GetViewMatrix();
		const Matrix4 matProj = camera.GetProjectionMatrix();
		const Matrix4 transform = Transpose(matView * matProj);

		mConstantBuffer.Update(&transform);
		mConstantBuffer.BindVS(0);

		mVertexShader.Bind();
		mPixelShader.Bind();

		mMeshBuffer.Update(mFaceVertices.get(), mFaceVertexCount);
		mMeshBuffer.SetTopology(MeshBuffer::Topology::Triangles);
		mMeshBuffer.Render();

		mMeshBuffer.Update(mLineVertices.get(), mLineVertexCount);
		mMeshBuffer.SetTopology(MeshBuffer::Topology::Lines);
		mMeshBuffer.Render();

		mLineVertexCount = 0;
		mFaceVertexCount = 0;
	}

	std::unique_ptr<SimpleDrawImpl> sInstance;
}

void SimpleDraw::StaticInitialize(uint32_t maxVertexCount)
{
	sInstance = std::make_unique<SimpleDrawImpl>();
	sInstance->Initialize(maxVertexCount);
}

void SimpleDraw::StaticTerminate()
{
	sInstance->Terminate();
	sInstance.reset();
}

void SimpleDraw::AddLine(const Vector3& v0, const Vector3& v1, const Color& color)
{
	sInstance->AddLine(v0, v1, color);
}

void SimpleDraw::AddFace(const Vector3& v0, const Vector3& v1, const Vector3& v2, const Color& color)
{
	sInstance->AddFace(v0, v1, v2, color);
}

void SimpleDraw::AddAABB(const Vector3& min, const Vector3& max, const Color& color)
{
	AddAABB(min.x, min.y, min.z, max.x, max.y, max.z, color);
}

void SimpleDraw::AddAABB(float minX, float minY, float minZ, float maxX, float maxY, float maxZ, const Color& color)
{
	const Vector3 trf = { maxX, maxY, minZ };
	const Vector3 brf = { maxX, minY, minZ };
	const Vector3 tlf = { minX, maxY, minZ };
	const Vector3 blf = { minX, minY, minZ };

	const Vector3 trb = { maxX, maxY, maxZ };
	const Vector3 brb = { maxX, minY, maxZ };
	const Vector3 tlb = { minX, maxY, maxZ };
	const Vector3 blb = { minX, minY, maxZ };

	//front
	AddLine(trf, brf, color);
	AddLine(brf, blf, color);
	AddLine(blf, tlf, color);
	AddLine(tlf, trf, color);

	//back
	AddLine(trb, brb, color);
	AddLine(brb, blb, color);
	AddLine(blb, tlb, color);
	AddLine(tlb, trb, color);

	//top
	AddLine(trb, trf, color);
	AddLine(tlb, tlf, color);

	//bottom
	AddLine(brb, brf, color);
	AddLine(blb, blf, color);
}

void SimpleDraw::AddFilledAABB(const Vector3& min, const Vector3& max, const Color& color)
{
	AddFilledAABB(min.x, min.y, min.z, max.x, max.y, max.z, color);
}

void SimpleDraw::AddFilledAABB(float minX, float minY, float minZ, float maxX, float maxY, float maxZ, const Color& color)
{
	const Vector3 trf = { maxX, maxY, minZ };
	const Vector3 brf = { maxX, minY, minZ };
	const Vector3 tlf = { minX, maxY, minZ };
	const Vector3 blf = { minX, minY, minZ };
	;
	const Vector3 trb = { maxX, maxY, maxZ };
	const Vector3 brb = { maxX, minY, maxZ };
	const Vector3 tlb = { minX, maxY, maxZ };
	const Vector3 blb = { minX, minY, maxZ };

	//front
	AddFace(trf, brf, blf, color);
	AddFace(trf, blf, tlf, color);

	//back
	AddFace(trb, blb, brb, color);
	AddFace(trb, tlb, blb, color);

	//top
	AddFace(trb, trf, tlf, color);
	AddFace(trb, tlf, tlb, color);

	//bottom
	AddFace(brb, blf, brf, color);
	AddFace(brb, blb, blf, color);

	//right
	AddFace(trb, brb, brf, color);
	AddFace(trb, brf, trf, color);

	//left
	AddFace(tlb, blf, blb, color);
	AddFace(tlb, tlf, blf, color);
}

void SimpleDraw::AddSphere(uint32_t slices, uint32_t rings, float radius, const Color& color)
{
	Vector3 v0 = Vector3::Zero;
	Vector3 v1 = Vector3::Zero;

	const float vertRotation = (TwoPi / static_cast<float>(rings - 1));
	const float horzRotation = (TwoPi / static_cast<float>(slices - 1));

	for (uint32_t r = 0; r < rings; ++r)
	{
		float rPos0 = static_cast<float>(r);
		float rPos1 = static_cast<float>(r + 1);
		float phi0 = rPos0 * vertRotation;
		float phi1 = rPos1 * vertRotation;
		for (uint32_t s = 0; s < slices; ++s)
		{
			float sPos0 = static_cast<float>(s);
			float sPos1 = static_cast<float>(s + 1);
			float rot0 = sPos0 * horzRotation;
			float rot1 = sPos1 * horzRotation;

			v0 = {
				radius * sin(rot0) * sin(phi0),
				radius * cos(phi0),
				radius * cos(rot0) * sin(phi0)
			};
			v1 = {
				radius * sin(rot1) * sin(phi0),
				radius * cos(phi0),
				radius * cos(rot1) * sin(phi0)
			};
			AddLine(v0, v1, color);

			v1 = {
				radius * sin(rot0) * sin(phi1),
				radius * cos(phi1),
				radius * cos(rot0) * sin(phi1)
			};
			AddLine(v0, v1, color);
		}
	}
}

void SimpleDraw::AddGroundPlane(float size, const Color& color)
{
	const float hs = size * 0.5f;
	const uint32_t iSize = static_cast<uint32_t>(size);
	for (uint32_t i = 0; i <= iSize; ++i)
	{
		AddLine({ i - hs, 0.0f, -hs }, { i - hs, 0.0f, hs }, color);
		AddLine({ -hs, 0.0f, i - hs }, { hs, 0.0f, i - hs }, color);
	}
}

void SimpleDraw::AddGroundCircle(uint32_t slices, float radius, const Color& color)
{
	Vector3 v0 = Vector3::Zero;
	Vector3 v1 = Vector3::Zero;
	float horizRotation = (TwoPi / static_cast<float>(slices - 1));
	for (uint32_t s = 0; s < slices; ++s)
	{
		float sPos0 = static_cast<float>(s);
		float sPos1 = static_cast<float>(s + 1);
		float rot0 = sPos0 * horizRotation;
		float rot1 = sPos1 * horizRotation;

		v0 = {
			radius * sin(rot0),
			0.0f,
			radius * cos(rot0)
		};
		v1 = {
			radius * sin(rot1),
			0.0f,
			radius * cos(rot1)
		};
		AddLine(v0, v1, color);
	}
}

void SimpleDraw::AddTransform(const Matrix4& matrix)
{
	const Vector3 side = { matrix._11, matrix._12, matrix._13 };
	const Vector3 up = { matrix._21, matrix._22, matrix._23 };
	const Vector3 fwd = { matrix._31, matrix._32, matrix._33 };
	const Vector3 pos = { matrix._41, matrix._42, matrix._43 };

	AddLine(pos, pos + side, Colors::Red);
	AddLine(pos, pos + up, Colors::Green);
	AddLine(pos, pos + fwd, Colors::Blue);
}

void SimpleDraw::Render(const Camera& camera)
{
	sInstance->Render(camera);
}