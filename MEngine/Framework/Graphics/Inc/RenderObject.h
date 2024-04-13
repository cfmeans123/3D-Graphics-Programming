#pragma once

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
		
		TextureId diffuseTextureId;
	};
}