#include "precompiled.h"
#include "StandardEffect.h"
#include "Camera.h"
#include "RenderObject.h"
#include "VertexTypes.h"


using namespace MEngine;
using namespace MEngine::Graphics;

void StandardEffect::Initialize(const std::filesystem::path& filePath)
{
	mTransformBuffer.Initialize();
	mVertexShader.Initialize<Vertex>(filePath);
	mPixelShader.Initialize(filePath);
	mLightBuffer.Initialize();
	mMaterialBuffer.Initialize();
	mSampler.Initialize(Sampler::Filter::Linear, Sampler::AddressMode::Wrap);
	mSettingsBuffer.Initialize();
}

void StandardEffect::Terminate()
{
	mSampler.Terminate();
	mPixelShader.Terminate();
	mVertexShader.Terminate();
	mLightBuffer.Terminate();
	mMaterialBuffer.Terminate();
	mTransformBuffer.Terminate();
	mSettingsBuffer.Terminate();
}

void StandardEffect::Begin()
{
	ASSERT(mCamera != nullptr, "StandardEffect: no camera set!");
	ASSERT(mDirectionalLight != nullptr, "No Directional Light!");
	
	mVertexShader.Bind();
	mPixelShader.Bind();
	
	mSampler.BindVS(0);
	mSampler.BindPS(0);

	mTransformBuffer.BindVS(0);
	mSettingsBuffer.BindPS(1);
	mSettingsBuffer.BindVS(1);
	mLightBuffer.BindVS(2);
	mLightBuffer.BindPS(2);

	mMaterialBuffer.BindPS(3);
}

void StandardEffect::End()
{
	//nothing until shadows are implemented
}

void StandardEffect::Render(const RenderObject& renderObject, const Math::Matrix4& pos)
{
	const Math::Matrix4 matWorld = renderObject.transform.GetMatrix4();
	const Math::Matrix4 matView = mCamera->GetViewMatrix();
	const Math::Matrix4 matProj = mCamera->GetProjectionMatrix();

	Math::Matrix4 matFinal = pos * matWorld * matView * matProj;

	TransformData transformData;
	transformData.wvp = Math::Transpose(matFinal);
	transformData.world = Math::Transpose(matWorld);
	transformData.viewPosition = mCamera->GetPosition();
	mTransformBuffer.Update(transformData);

	SettingsData settingsData;
	settingsData.useDiffuseMap = renderObject.diffuseMapID > 0 && mSettingsData.useDiffuseMap > 0 ? 1 : 0;
	settingsData.useNormalMap = renderObject.normalMapID > 0 && mSettingsData.useNormalMap > 0 ? 1 : 0;
	settingsData.useSpecMap = renderObject.specMapID > 0 && mSettingsData.useSpecMap > 0 ? 1 : 0;
	settingsData.useLightingMap = mSettingsData.useLightingMap;
	settingsData.useBumpMap = renderObject.bumpMapID > 0 && mSettingsData.useBumpMap > 0;
	settingsData.bumpWeight = mSettingsData.bumpWeight;
	mSettingsBuffer.Update(settingsData);

	mLightBuffer.Update(*mDirectionalLight);
	mMaterialBuffer.Update(renderObject.material);

	TextureManager* tm = TextureManager::Get();
	tm->BindPS(renderObject.diffuseMapID, 0);
	tm->BindPS(renderObject.normalMapID, 1);
	tm->BindPS(renderObject.specMapID, 2);
	tm->BindVS(renderObject.bumpMapID, 3);

	renderObject.meshBuffer.Render();
}

void StandardEffect::SetCamera(const Camera& camera)
{
	mCamera = &camera;
}

void StandardEffect::SetDirectionalLight(const DirectionalLight& directionalLight)
{
	mDirectionalLight = &directionalLight;
}

void StandardEffect::DebugUI()
{
	if (ImGui::CollapsingHeader("StandardEffect", ImGuiTreeNodeFlags_DefaultOpen))
	{
		bool useDiffuseMap = mSettingsData.useDiffuseMap > 0;
		if (ImGui::Checkbox("UseDiffuseMap", &useDiffuseMap))
		{
			mSettingsData.useDiffuseMap = useDiffuseMap ? 1 : 0;
		}
		bool useNormalMap = mSettingsData.useNormalMap > 0;
		if (ImGui::Checkbox("UseNormalMap", &useNormalMap))
		{
			mSettingsData.useNormalMap = useNormalMap ? 1 : 0;
		}
		bool useSpecMap = mSettingsData.useSpecMap > 0;
		if (ImGui::Checkbox("UseSpecMap", &useSpecMap))
		{
			mSettingsData.useSpecMap = useSpecMap ? 1 : 0;
		}
		bool useLighting = mSettingsData.useLightingMap > 0;
		if (ImGui::Checkbox("Use Lighting", &useLighting))
		{
			mSettingsData.useLightingMap = useLighting ? 1 : 0;
		}
		bool useBumpMap = mSettingsData.useBumpMap > 0;
		if (ImGui::Checkbox("Use Bump Map", &useBumpMap))
		{
			mSettingsData.useBumpMap = useBumpMap ? 1 : 0;
		}
		ImGui::DragFloat("Bump Weight", &mSettingsData.bumpWeight, 0.01f, 0.0f, 100.0f);
	}
}
