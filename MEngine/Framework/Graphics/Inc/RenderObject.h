#pragma once

#include "Material.h"
#include "MeshBuffer.h"
#include "Material.h"
#include "TextureManager.h"
#include "Transform.h"
#include "ModelManager.h"

namespace MEngine::Graphics
{
    struct Model;

    struct RenderObject
    {
        void Terminate();

        Transform transform;
        MeshBuffer meshBuffer;

        Material material;

        TextureId diffuseMapID;
        TextureId normalMapID;
        TextureId specMapID;
        TextureId bumpMapID;

        ModelID modelID = 0;
        const Skeleton* skeleton = nullptr;
    };

    using RenderGroup = std::vector<RenderObject>;
    [[nodiscard]] RenderGroup CreateRenderGroup(ModelID id);
    [[nodiscard]] RenderGroup CreateRenderGroup(const Model& model, ModelID id = 0);
    void CleanRenderGroup(RenderGroup& renderGroup);

    void SetRenderGroupPosition(RenderGroup& renderGroup, const Math::Vector3& pos);

    template<class Effect>
    void DrawRenderGroup(Effect& effect, const RenderGroup& renderGroup)
    {
        for (const RenderObject& renderObject : renderGroup)
        {   
            effect.Render(renderObject);
        }
    }  
}