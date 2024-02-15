#include "Precompile.h"
#include "../Inc/MeshBuilder.h"

using namespace MEngine;
using namespace MEngine::Graphics;

namespace
{
    Color GetNextColor(int& index)
    {
        constexpr Color colorTable[] = {
        Colors::AliceBlue,
        Colors::DeepPink,
        Colors::Aquamarine,
        Colors::IndianRed,
        Colors::Salmon,
        Colors::Chocolate,
        Colors::BlanchedAlmond,
        Colors::Cornsilk,
        Colors::DarkSalmon,
        Colors::Azure
        };

        index = (index + 1) % std::size(colorTable);
        return colorTable[index];
    }
}

MeshPC MeshBuilder::CreatePyramid(float size)
{
    MeshPC mesh;
    const float hs = size * 0.5f;

    srand(time(nullptr));
    int index = rand() % 100;
    //front
    mesh.vertices.push_back({ { -hs, -hs, -hs }, GetNextColor(index)});
    mesh.vertices.push_back({ {0.0f, hs, 0.0f }, GetNextColor(index) });
    mesh.vertices.push_back({ { hs, -hs, -hs }, GetNextColor(index) });

    //back
    mesh.vertices.push_back({ { -hs, -hs, hs }, GetNextColor(index) });
    mesh.vertices.push_back({ { hs, -hs, hs }, GetNextColor(index) });

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

MeshPC MEngine::Graphics::MeshBuilder::CreateVerticalPlanePC(uint32_t numRows, uint32_t numCols, float spacing)
{
    srand(time(nullptr));
    int index = rand() % 100;

    MeshPC mesh;

    const float hpw = static_cast<float>(numCols) * spacing * 0.5f;
    const float hph = static_cast<float>(numRows) * spacing * 0.5f;

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

    for (uint32_t r = 0; r < numRows; ++r)
    {
        for (uint32_t c = 0; c < numCols; ++c)
        {
            uint32_t i = (r * (numCols + 1)) + c;

            //triangle 1
            mesh.indices.push_back(i);
            mesh.indices.push_back(i + numCols + 2);
            mesh.indices.push_back(i + 1);

            //triangle 2
            mesh.indices.push_back(i);
            mesh.indices.push_back(i + numCols + 1);
            mesh.indices.push_back(i + numCols + 2);
        }
    }
    return mesh;
}

MeshPC MEngine::Graphics::MeshBuilder::CreateHorizontalPlanePC(uint32_t numRows, uint32_t numCols, float spacing)
{
    srand(time(nullptr));
    int index = rand() % 100;

    MeshPC mesh;

    const float hpw = static_cast<float>(numCols) * spacing * 0.5f;
    const float hph = static_cast<float>(numRows) * spacing * 0.5f;

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

    for (uint32_t r = 0; r < numRows; ++r)
    {
        for (uint32_t c = 0; c < numCols; ++c)
        {
            uint32_t i = (r * (numCols + 1)) + c;

            //triangle 1
            mesh.indices.push_back(i);
            mesh.indices.push_back(i + numCols + 2);
            mesh.indices.push_back(i + 1);

            //triangle 2
            mesh.indices.push_back(i);
            mesh.indices.push_back(i + numCols + 1);
            mesh.indices.push_back(i + numCols + 2);
        }
    }
    return mesh;
}
