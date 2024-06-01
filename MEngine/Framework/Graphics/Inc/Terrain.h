#pragma once

#include "MeshTypes.h"

namespace MEngine::Graphics
{
	class Terrain final
	{
	public:
		void Initialize(const std::filesystem::path& filename, float heightScale);
		void SetHeightScale(float heightScale);
		float GetHeightScale() const;
		float GetHeight(const Math::Vector3& position) const;
		const Mesh& GetMesh() const;
		float GetWidth() const;
		float GetLength() const;

	private:
		Mesh mMesh;
		uint32_t mRows = 0;
		uint32_t mColumns = 0;
		float mHeightScale = 1.0f;
	};
}