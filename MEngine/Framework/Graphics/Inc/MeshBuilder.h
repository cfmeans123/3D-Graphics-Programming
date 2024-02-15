#pragma once

#include "MeshTypes.h"

namespace MEngine::Graphics
{
    class MeshBuilder
    {
    public:
        //Pyramid
        static MeshPC CreatePyramid(float size);

        static MeshPC CreateVerticalPlanePC(uint32_t numRows, uint32_t numCols, float spacing);
        static MeshPC CreateHorizontalPlanePC(uint32_t numRows, uint32_t numCols, float spacing);

        void CreatePlaneIndices(std::vector<uint32_t> indices, )
    };
}