#pragma once

#include "ConstantBuffer.h"
#include "PixelShader.h"
#include "Sampler.h"
#include "VertexShader.h"

namespace MEngine::Graphics
{
    struct RenderObject;
    class Texture;

    class PostProcessingEffect
    {
    public:
        void Initialize(const std::filesystem::path& filePath);
        void Terminate();

        void Begin();
        void End();

        void Render(const RenderObject& renderObject);

        void DebugUI();

        void SetTexture(const Texture* texture, uint32_t slot = 0);

    private:
        Sampler mSampler;
        VertexShader mVertexShader;
        PixelShader mPixelShader;
        std::array<const Texture*, 4> mTextures;
    };

}