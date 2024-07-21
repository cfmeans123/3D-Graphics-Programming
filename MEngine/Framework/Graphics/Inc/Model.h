#pragma once

#include "MeshTypes.h"
#include "Material.h"
#include "Skeleton.h"

namespace MEngine::Graphics
{
    struct Model
    {
        struct MeshData
        {
            Mesh mesh;
            uint32_t materialIndex = 0;
        };

        struct MeterialData
        {
            Material material;
            std::string diffuseMapName;
            std::string normalMapName;
            std::string specularMapName;
            std::string bumpMapName;
        };

        std::vector<MeshData> meshData;
        std::vector<MeterialData> meterialData;
        std::unique_ptr<Skeleton> skeleton;
    };
}