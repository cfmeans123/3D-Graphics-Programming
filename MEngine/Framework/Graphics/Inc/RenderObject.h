#pragma once

#include "Material.h"
#include "MeshBuffer.h"
#include "Material.h"
#include "TextureManager.h"
#include "Transform.h"

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
    };

    using RenderGroup = std::vector<RenderObject>;
    [[nodiscard]] RenderGroup CreateRenderGroup(const Model& model);
    void CleanRenderGroup(RenderGroup& renderGroup);

    template<class Effect>
    void DrawRenderGroup(Effect& effect, const RenderGroup& renderGroup, const Math::Matrix4& pos)
    {
        for (const RenderObject& renderObject : renderGroup)
        {
            effect.Render(renderObject, pos);
        }
    }
}