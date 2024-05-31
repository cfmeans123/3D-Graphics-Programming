#include "Precompiled.h"
#include "ShadowEffect.h"

#include "RenderObject.h"
#include "VertexTypes.h"

using namespace MEngine;
using namespace MEngine::Graphics;

void ShadowEffect::Initialize()
{
    std::filesystem::path shaderFile = L"../../Assets/Shaders/Shadow.fx";
    mVertexShader.Initialize<Vertex>(shaderFile);
    mPixelShader.Initialize(shaderFile);

    mTransformBuffer.Initialize();

    mLightCamera.SetMode(Camera::ProjectionMode::Orthographic);
    mLightCamera.SetNearPlane(1.0f);
    mLightCamera.SetFarPlane(2000.0f);

    constexpr uint32_t depthMapResolution = 4096;
    mDepthMapRenderTarget.Initialize(depthMapResolution, depthMapResolution, Texture::Format::RGBA_U32);
    mDepthMapRenderTargetFar.Initialize(depthMapResolution, depthMapResolution, Texture::Format::RGBA_U32);
}

void ShadowEffect::Terminate()
{
    mDepthMapRenderTarget.Terminate();
    mDepthMapRenderTargetFar.Terminate();
    mTransformBuffer.Terminate();
    mPixelShader.Terminate();
    mVertexShader.Terminate();
}

void ShadowEffect::Begin(bool farDepth)
{
    UpdateLightCamera(farDepth);
    
    mVertexShader.Bind();
    mPixelShader.Bind();
    mTransformBuffer.BindVS(0);

    if (farDepth)
    {
        mDepthMapRenderTargetFar.BeginRender();

    }
    else
    {
        mDepthMapRenderTarget.BeginRender();
    }
}

void ShadowEffect::End(bool farDepth)
{
    if (farDepth)
    {
        mDepthMapRenderTargetFar.EndRender();
    }
    else
    {
        mDepthMapRenderTarget.EndRender();
    }
}

void ShadowEffect::Render(const RenderObject& renderObject)
{
    const Math::Matrix4 matWorld = renderObject.transform.GetMatrix4();
    const Math::Matrix4 matView = mLightCamera.GetViewMatrix();
    const Math::Matrix4 matProj = mLightCamera.GetProjectionMatrix();

    TransformData data;
    data.wvp = Transpose(matWorld * matView * matProj);
    mTransformBuffer.Update(data);

    renderObject.meshBuffer.Render();
}

void ShadowEffect::DebugUI()
{
    if (ImGui::CollapsingHeader("ShadowEffect", ImGuiTreeNodeFlags_DefaultOpen))
    {
        ImGui::Text("DepthMap");
        ImGui::Image(
            mDepthMapRenderTarget.GetRawData(),
            { 144, 144 },
            { 0, 0 },
            { 1, 1 },
            { 1, 1, 1, 1 },
            { 1, 1, 1, 1 });
        ImGui::Text("DepthMapFar");
        ImGui::Image(
            mDepthMapRenderTargetFar.GetRawData(),
            { 144, 144 },
            { 0, 0 },
            { 1, 1 },
            { 1, 1, 1, 1 },
            { 1, 1, 1, 1 });
        ImGui::DragFloat("Size##Shadow", &mSize, 1.0f, 1.0f, 1000.0f);
    }
}

void ShadowEffect::SetDirectionalLight(const DirectionalLight& directionalLight)
{
    mDirectionalLight = &directionalLight;
}

void ShadowEffect::SetFocus(const Math::Vector3& focusPosition)
{
    mFocusPosition = focusPosition;
}

void ShadowEffect::SetSize(float size)
{
    mSize = size;
}

const Camera& ShadowEffect::GetLightCamera() const
{
    return mLightCamera;
}

const Texture& ShadowEffect::GetDepthMap() const
{
    return mDepthMapRenderTarget;
}

const Texture& ShadowEffect::GetDepthMapFar() const
{
    return mDepthMapRenderTargetFar;
}

void ShadowEffect::UpdateLightCamera(bool farDepth)
{
    ASSERT(mDirectionalLight != nullptr, "ShadowEffect: no light set!");
    const Math::Vector3& direction = mDirectionalLight->direction;
    mLightCamera.SetDirection(direction);
    mLightCamera.SetPosition(mFocusPosition - (direction * 1000.0f));
    float size = (farDepth) ? mSize + 500.0f : mSize;
    mLightCamera.SetSize(size, size);
}
