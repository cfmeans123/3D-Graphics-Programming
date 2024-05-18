#include "Precompiled.h"
#include "GaussianBlurEffect.h"
#include "GraphicsSystem.h"
#include "Camera.h"
#include "RenderObject.h"
#include "VertexTypes.h"

using namespace MEngine;
using namespace MEngine::Graphics;

void GaussianBlurEffect::Initialize()
{
	GraphicsSystem* gs = GraphicsSystem::Get();
	const float screenWidth = gs->GetBackBufferWidth();
	const float screenHeight = gs->GetBackBufferHeight();
	mHorizontalBlurRenderTexture.Initialize(screenWidth, screenHeight, RenderTarget::Format::RGBA_U8);
	mVerticalBlurRenderTexture.Initialize(screenWidth, screenHeight, RenderTarget::Format::RGBA_U8);

	mSettingsBuffer.Initialize();
	mSampler.Initialize(Sampler::Filter::Linear, Sampler::AddressMode::Wrap);

	std::filesystem::path shaderFile = "../../Assets/Shaders/GaussianBlur.fx";
	mVertexShader.Initialize<VertexPX>(shaderFile);
	mHorizontalBlurPixelShader.Initialize(shaderFile, "HorizontalBlurPS");
	mVerticalBlurPixelShader.Initialize(shaderFile, "VerticalBlurPS");
}

void GaussianBlurEffect::Terminate()
{
	mVerticalBlurPixelShader.Terminate();
	mHorizontalBlurPixelShader.Terminate();
	mVertexShader.Terminate();
	mSampler.Terminate();
	mSettingsBuffer.Terminate();
	mVerticalBlurRenderTexture.Terminate();
	mHorizontalBlurRenderTexture.Terminate();
}

void GaussianBlurEffect::Begin()
{
	mVertexShader.Bind();

	GraphicsSystem* gs = GraphicsSystem::Get();

	SettingsData data;
	data.screenWidth = gs->GetBackBufferWidth();
	data.screenHeight = gs->GetBackBufferHeight();
	data.multiplier = mBlurSaturation;
	mSettingsBuffer.Update(data);

	mSettingsBuffer.BindPS(0);
}

void GaussianBlurEffect::End()
{
	GraphicsSystem* gs = GraphicsSystem::Get();
	gs->ResetRenderTarget();
	gs->ResetViewport();
}

void GaussianBlurEffect::Render(const RenderObject& renderObject)
{
	GraphicsSystem* gs = GraphicsSystem::Get();

	mHorizontalBlurRenderTexture.BeginRender();
		mSourceTexture->BindPS(0);
		mHorizontalBlurPixelShader.Bind();
		renderObject.meshBuffer.Render();
		Texture::UnBindPS(0);
	mHorizontalBlurRenderTexture.EndRender();

	for (uint32_t i = 1; i < mBlurIterations; ++i)
	{
		mVerticalBlurRenderTexture.BeginRender();
		mHorizontalBlurRenderTexture.BindPS(0);
		mVerticalBlurPixelShader.Bind();
		renderObject.meshBuffer.Render();
		Texture::UnBindPS(0);
		mVerticalBlurRenderTexture.EndRender();

		mHorizontalBlurRenderTexture.BeginRender();
		mVerticalBlurRenderTexture.BindPS(0);
		mHorizontalBlurPixelShader.Bind();
		renderObject.meshBuffer.Render();
		Texture::UnBindPS(0);
		mHorizontalBlurRenderTexture.EndRender();
	}
	mVerticalBlurRenderTexture.BeginRender();
	mHorizontalBlurRenderTexture.BindPS(0);
	mVerticalBlurPixelShader.Bind();
	renderObject.meshBuffer.Render();
	Texture::UnBindPS(0);
	mVerticalBlurRenderTexture.EndRender();
}

void GaussianBlurEffect::DebugUI()
{
	if (ImGui::CollapsingHeader("GaussianBlur", ImGuiTreeNodeFlags_DefaultOpen))
	{
		ImGui::DragInt("BlurIterations", &mBlurIterations, 1, 1, 100);
		ImGui::DragFloat("BlurSaturation", &mBlurSaturation, 0.001f, 1.0f, 10.0f);
	}
}

void GaussianBlurEffect::SetSourceTexture(const Texture& texture)
{
	mSourceTexture = &texture;
}

const Texture& GaussianBlurEffect::GetHorizontalBlurTexture() const
{
	return mHorizontalBlurRenderTexture;
}

const Texture& GaussianBlurEffect::GetVerticalBlurTexture() const
{
	return mVerticalBlurRenderTexture;
}

const Texture& GaussianBlurEffect::GetResultTexture() const
{
	return mVerticalBlurRenderTexture;
}
