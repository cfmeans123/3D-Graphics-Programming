#pragma once

#include "MeshTypes.h"

namespace MEngine::Graphics
{
	class MeshBuilder
	{
	public:
		//Pyramid
		static MeshPC CreatePyramidPC(float size);

		//Cube
		static MeshPC CreateCubePC(float size);

		//Rectangle
		static MeshPC CreateRectPC(float width, float height, float depth);

		//Vertical Plane
		static MeshPC CreateVerticalPlanePC(uint32_t numRows, uint32_t numCols, float spacing);
		static MeshPX CreateVerticalPlanePX(uint32_t numRows, uint32_t numCols, float spacing);

		//Horizontal Plane
		static MeshPC CreateHorizontalPlanePC(uint32_t numRows, uint32_t numCols, float spacing);
		static MeshPX CreateHorizontalPlanePX(uint32_t numRows, uint32_t numCols, float spacing);

		//Cylinder
		static MeshPC CreateCylinderPC(uint32_t slices, uint32_t rings);

		//Sphere
		static MeshPC CreateSpherePC(uint32_t slices, uint32_t rings, float radius);
		static MeshPX CreateSpherePX(uint32_t slices, uint32_t rings, float radius);

		//Sky
		static MeshPX CreateSkySpherePX(uint32_t slices, uint32_t rings, float radius);
		static MeshPX CreateSkyBoxPX(float size);
	};
}