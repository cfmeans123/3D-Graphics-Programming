#include "Precompiled.h"
#include "PostProcessingEffects.h"
#include "RenderObject.h"
#include "GraphicsSystem.h"
#include "VertexTypes.h"

using namespace MEngine;
using namespace MEngine::Graphics;

void PostProcessingEffect::Initialize(const std::filesystem::path& filePath)
{
    mVertexShader.Initialize<VertexPX>(filePath);
    mPixelShader.Initialize(filePath);
    
    mSampler.Initialize(Sampler::Filter::Point, Sampler::AddressMode::Wrap);
}

void PostProcessingEffect::Terminate()
{
    mSampler.Terminate();
    mVertexShader.Terminate();
    mPixelShader.Terminate();
}

void PostProcessingEffect::Begin()
{
    mVertexShader.Bind();
    mPixelShader.Bind();
    mSampler.BindPS(0);
    for (uint32_t i = 0; i < mTextures.size(); ++i)
    {
        if (mTextures[i] != nullptr)
        {
            mTextures[i]->BindPS(i);
        }
    }
}

void PostProcessingEffect::End()
{
    for (uint32_t i = 0; i < mTextures.size(); ++i)
    {
        Texture::UnBindPS(i);
    }
}

void PostProcessingEffect::Render(const RenderObject& renderObject)
{
    renderObject.meshBuffer.Render();
}

void PostProcessingEffect::DebugUI()
{
    if (ImGui::CollapsingHeader("PostProcessingEffect", ImGuiTreeNodeFlags_DefaultOpen))
    {
        
    }
}

void PostProcessingEffect::SetTexture(const Texture* texture, uint32_t slot)
{
    ASSERT(slot < mTextures.size(), "PostProcessingEffect: invalid slot index");
    mTextures[slot] = texture;
}
