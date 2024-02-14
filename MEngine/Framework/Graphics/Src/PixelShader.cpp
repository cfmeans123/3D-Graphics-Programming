#include "Precompile.h"
#include "../inc/PixelShader.h"

#include "../inc/GraphicsSystem.h"

using namespace MEngine;
using namespace MEngine::Graphics;

void MEngine::Graphics::PixelShader::Initialize(const std::filesystem::path& filePath)
{
	auto device = GraphicsSystem::Get()->GetDevice();

	DWORD shaderFlags = D3DCOMPILE_ENABLE_STRICTNESS | D3DCOMPILE_DEBUG;
	ID3DBlob* shaderBlob = nullptr;
	ID3DBlob* errorBlob = nullptr;

	HRESULT hr = D3DCompileFromFile(
		filePath.c_str(),
		nullptr,
		D3D_COMPILE_STANDARD_FILE_INCLUDE,
		"PS", "ps_5_0",
		shaderFlags, 0,
		&shaderBlob,
		&errorBlob
	);
	if (errorBlob != nullptr && errorBlob->GetBufferPointer() != nullptr)
	{
		LOG("%s", static_cast<const char*>(errorBlob->GetBufferPointer()));
	}
	ASSERT(SUCCEEDED(hr), "Failed to compile pixel shader");

	hr = device->CreatePixelShader(
		shaderBlob->GetBufferPointer(),
		shaderBlob->GetBufferSize(),
		nullptr,
		&mPixelShader
	);
	ASSERT(SUCCEEDED(hr), "Failed to create pixel shader");
	SafeRelease(shaderBlob);
	SafeRelease(errorBlob);
}

void MEngine::Graphics::PixelShader::Terminate()
{
	SafeRelease(mPixelShader);
}

void MEngine::Graphics::PixelShader::Bind()
{
	auto context = GraphicsSystem::Get()->GetContext();
	context->PSSetShader(mPixelShader, nullptr, 0);
}