#pragma once

#include "Material.h"
#include "MeshBuffer.h"
#include "TextureManager.h"
#include "Transform.h"

namespace MEngine::Graphics
{
	struct RenderObject
	{
		void Terminate();

		Transform mTransform;
		MeshBuffer meshBuffer;
		
		Material material;

		TextureId diffuseTextureMapId;
		TextureId normalTextureMapId;
		TextureId specTextureMapId;
		TextureId bumpTextureMapId;
	};
}