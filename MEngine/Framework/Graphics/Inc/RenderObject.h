#pragma once

#include "MeshBuffer.h"
#include "Material.h"
#include "TextureManager.h"
#include "Transform.h"
#include "ModelManager.h"

namespace MEngine::Graphics
{
    struct Model;
    class Animator;

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
        const Animator* animator = nullptr;
    };

    using RenderGroup = std::vector<RenderObject>;
    [[nodiscard]] RenderGroup CreateRenderGroup(ModelID id, const Animator* animator = nullptr);
    [[nodiscard]] RenderGroup CreateRenderGroup(const Model& model, ModelID id = 0, const Animator* animator = nullptr);
    void CleanRenderGroup(RenderGroup& renderGroup);

    void SetRenderGroupPosition(RenderGroup& renderGroup, const Math::Vector3& position);

    template<class Effect>
    void DrawRenderGroup(Effect& effect, const RenderGroup& renderGroup)
    {
        for (const RenderObject& renderObject : renderGroup)
        {
            effect.Render(renderObject);
        }
    }
}