#include "Precompile.h"
#include "../Inc/MeshBuilder.h"

using namespace MEngine;
using namespace MEngine::Graphics;


namespace
{
	Color GetNextColor(int& index)
	{
		constexpr Color colorTable[] = {
			Colors::AntiqueWhite,
			Colors::Chocolate,
			Colors::LightPink,
			Colors::LightGoldenrodYellow,
			Colors::DeepSkyBlue,
			Colors::MediumVioletRed,
			Colors::SandyBrown,
			Colors::Salmon,
			Colors::Purple,
			Colors::Bisque,
			Colors::GhostWhite
		};

		index = (index + 1) % std::size(colorTable);
		return colorTable[index];
	}

	void CreateCubeIndices(std::vector<uint32_t>& indices)
	{
		indices = {
			//front
			0, 1, 2,
			0, 2, 3,
			//back
			7, 5, 4,
			7, 6, 5,
			//right
			3, 2, 6,
			3, 6, 7,
			//left
			4, 5, 1,
			4, 1, 0,
			//top
			1, 5, 6,
			1, 6, 2,
			//bottom
			0, 3, 7,
			0, 7, 4
		};
	}

	void CreatePlaneIndicies(std::vector<uint32_t>& indices, uint32_t numRows, uint32_t numCols)
	{
		for (uint32_t r = 0; r < numRows; ++r)
		{
			for (uint32_t c = 0; c < numCols; ++c)
			{
				uint32_t i = (r * (numCols + 1)) + c;

				//triangle 1
				indices.push_back(i);
				indices.push_back(i + numCols + 2);
				indices.push_back(i + 1);

				//triangle 2
				indices.push_back(i);
				indices.push_back(i + numCols + 1);
				indices.push_back(i + numCols + 2);
			}
		}
	}
}

MeshPC MeshBuilder::CreatePyramidPC(float size)
{
	MeshPC mesh;
	const float hs = size * 0.5f;

	srand(time(nullptr));
	int index = rand() % 100;

	// front
	mesh.vertices.push_back({ { -hs, -hs, -hs }, GetNextColor(index) });
	mesh.vertices.push_back({ { 0.0f, hs, 0.0f }, GetNextColor(index) });
	mesh.vertices.push_back({ { hs, -hs, -hs }, GetNextColor(index) });

	//back
	mesh.vertices.push_back({ {-hs, -hs, hs}, GetNextColor(index) });
	mesh.vertices.push_back({ {hs, -hs, hs}, GetNextColor(index) });

	//indices
	mesh.indices = {
		//front
		0, 1, 2,
		//right
		2, 1, 4,
		//back
		4, 1, 3,
		//left
		0, 3, 1,
		//bottom
		0, 2, 4,
		0, 4, 3
	};

	return mesh;
}

MeshPC MeshBuilder::CreateCubePC(float size)
{
	MeshPC mesh;
	const float hs = size * 0.5f;

	srand(time(nullptr));
	int index = rand() % 100;

	//front
	mesh.vertices.push_back({ {-hs, -hs, -hs}, GetNextColor(index) });
	mesh.vertices.push_back({ {-hs,  hs, -hs}, GetNextColor(index) });
	mesh.vertices.push_back({ { hs,  hs, -hs}, GetNextColor(index) });
	mesh.vertices.push_back({ { hs, -hs, -hs}, GetNextColor(index) });

	mesh.vertices.push_back({ {-hs, -hs,  hs}, GetNextColor(index) });
	mesh.vertices.push_back({ {-hs,  hs,  hs}, GetNextColor(index) });
	mesh.vertices.push_back({ { hs,  hs,  hs}, GetNextColor(index) });
	mesh.vertices.push_back({ { hs, -hs,  hs}, GetNextColor(index) });

	CreateCubeIndices(mesh.indices);

	return mesh;
}

MeshPC MeshBuilder::CreateRectPC(float width, float height, float depth)
{
	MeshPC mesh;
	const float hw = width * 0.5f;
	const float hh = height * 0.5f;
	const float hd = depth * 0.5f;

	srand(time(nullptr));
	int index = rand() % 100;

	//front
	mesh.vertices.push_back({ {-hw, -hh, -hd}, GetNextColor(index) });
	mesh.vertices.push_back({ {-hw,  hh, -hd}, GetNextColor(index) });
	mesh.vertices.push_back({ { hw,  hh, -hd}, GetNextColor(index) });
	mesh.vertices.push_back({ { hw, -hh, -hd}, GetNextColor(index) });

	mesh.vertices.push_back({ {-hw, -hh,  hd}, GetNextColor(index) });
	mesh.vertices.push_back({ {-hw,  hh,  hd}, GetNextColor(index) });
	mesh.vertices.push_back({ { hw,  hh,  hd}, GetNextColor(index) });
	mesh.vertices.push_back({ { hw, -hh,  hd}, GetNextColor(index) });

	CreateCubeIndices(mesh.indices);

	return mesh;
}

MeshPC MeshBuilder::CreateVerticalPlanePC(uint32_t numRows, uint32_t numCols, float spacing)
{
	MeshPC mesh;
	const float hpw = static_cast<float>(numCols) * spacing * 0.5f;
	const float hph = static_cast<float>(numRows) * spacing * 0.5f;

	srand(time(nullptr));
	int index = rand() % 100;

	float x = -hpw;
	float y = -hph;

	for (uint32_t r = 0; r <= numRows; ++r)
	{
		for (uint32_t c = 0; c <= numCols; ++c)
		{
			mesh.vertices.push_back({ {x, y, 0.0f}, GetNextColor(index) });
			x += spacing;
		}
		x = -hpw;
		y += spacing;
	}

	CreatePlaneIndicies(mesh.indices, numRows, numCols);

	return mesh;
}

MeshPC MeshBuilder::CreateHorizontalPlanePC(uint32_t numRows, uint32_t numCols, float spacing)
{
	MeshPC mesh;
	const float hpw = static_cast<float>(numCols) * spacing * 0.5f;
	const float hph = static_cast<float>(numRows) * spacing * 0.5f;

	srand(time(nullptr));
	int index = rand() % 100;

	float x = -hpw;
	float z = -hph;

	for (uint32_t r = 0; r <= numRows; ++r)
	{
		for (uint32_t c = 0; c <= numCols; ++c)
		{
			mesh.vertices.push_back({ {x, 0.0f, z}, GetNextColor(index) });
			x += spacing;
		}
		x = -hpw;
		z += spacing;
	}

	CreatePlaneIndicies(mesh.indices, numRows, numCols);

	return mesh;
}

MeshPC MeshBuilder::CreateCylinderPC(uint32_t slices, uint32_t rings)
{
	MeshPC mesh;
	const float hh = static_cast<float>(rings) * 0.5f;

	srand(time(nullptr));
	int index = rand() % 100;

	for (uint32_t r = 0; r <= rings; ++r)
	{
		float ringPos = static_cast<float>(r);
		for (uint32_t s = 0; s <= slices; ++s)
		{
			float slicePos = static_cast<float>(s);
			float rotation = (slicePos / static_cast<float>(slices)) * Math::Constants::TwoPi;

			mesh.vertices.push_back({ {
				cos(rotation),
				ringPos - hh,
				sin(rotation) },
				GetNextColor(index) });
		}
	}

	//caps
	mesh.vertices.push_back({ {0.0f, hh, 0.0f}, GetNextColor(index) });
	mesh.vertices.push_back({ {0.0f, -hh, 0.0f}, GetNextColor(index) });

	CreatePlaneIndicies(mesh.indices, rings, slices);

	//caps
	uint32_t bottomIndex = mesh.vertices.size() - 1;
	uint32_t topIndex = bottomIndex - 1;
	for (uint32_t s = 0; s < slices; ++s)
	{
		//bottom cap
		mesh.indices.push_back(bottomIndex);
		mesh.indices.push_back(s);
		mesh.indices.push_back(s + 1);

		//top cap
		uint32_t topRowIndex = topIndex - slices - 1 + s;
		mesh.indices.push_back(topIndex);
		mesh.indices.push_back(topRowIndex + 1);
		mesh.indices.push_back(topRowIndex);
	}


	return mesh;
}

MeshPC MeshBuilder::CreateSpherePC(uint32_t slices, uint32_t rings, float radius)
{
	MeshPC mesh;

	srand(time(nullptr));
	int index = rand() % 100;

	const float vertRotation = (Math::Constants::Pi / static_cast<float>(rings));
	const float horzRotation = (Math::Constants::TwoPi / static_cast<float>(slices));

	for (uint32_t r = 0; r <= rings; ++r)
	{
		float ringPos = static_cast<float>(r);
		float phi = ringPos * vertRotation;
		for (uint32_t s = 0; s <= slices; ++s)
		{
			float slicePos = static_cast<float>(s);
			float rotation = slicePos * horzRotation;

			mesh.vertices.push_back({ {
				radius * sin(rotation) * sin(phi),
				radius * cos(phi),
				radius * cos(rotation) * sin(phi)},
				GetNextColor(index) });
		}
	}

	CreatePlaneIndicies(mesh.indices, rings, slices);

	return mesh;
}