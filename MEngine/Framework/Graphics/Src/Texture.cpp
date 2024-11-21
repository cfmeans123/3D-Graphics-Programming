#include "Precompiled.h"
#include "Texture.h"

#include "GraphicsSystem.h"
#include "DirectXTK/Inc/WICTextureLoader.h"

using namespace MEngine;
using namespace MEngine::Graphics;

void Texture::UnBindPS(uint32_t slot)
{
	static ID3D11ShaderResourceView* dummy = nullptr;
	GraphicsSystem::Get()->GetContext()->PSGetShaderResources(slot, 1, &dummy);
}

Texture::~Texture()
{
	ASSERT(mShaderResourceView == nullptr, "Texture: must call terminate");
}

Texture::Texture(Texture&& rhs) noexcept
	: mShaderResourceView(rhs.mShaderResourceView)
{
	rhs.mShaderResourceView = nullptr;
}

Texture& Texture::operator=(Texture&& rhs) noexcept
{
	mShaderResourceView = rhs.mShaderResourceView;
	rhs.mShaderResourceView = nullptr;
	return *this;
}

void Texture::Initialize(const std::filesystem::path& fileName)
{
	auto device = GraphicsSystem::Get()->GetDevice();
	auto context = GraphicsSystem::Get()->GetContext();
	HRESULT hr = DirectX::CreateWICTextureFromFile(device, context, fileName.c_str(), nullptr, &mShaderResourceView);
	ASSERT(SUCCEEDED(hr), "Texture: failed to ceate texture %ls", fileName.c_str());

	ID3D11Resource* resource = nullptr;
	mShaderResourceView->GetResource(&resource);

	ID3D11Texture2D* texture2D = nullptr;
	hr = resource->QueryInterface(&texture2D);
	ASSERT(SUCCEEDED(hr), "Texture: failed to find texture data");

	D3D11_TEXTURE2D_DESC desc;
	texture2D->GetDesc(&desc);
	mWidth = static_cast<uint32_t>(desc.Width);
	mHeight = static_cast<uint32_t>(desc.Height);

	SafeRelease(texture2D);
	SafeRelease(resource);
}

void Texture::Initialize(uint32_t width, uint32_t height, Format format)
{
	ASSERT(false, "Texture: not yet implemented");
}

void Texture::Terminate()
{
	SafeRelease(mShaderResourceView);
}

void Texture::BindVS(uint32_t slot) const
{
	auto context = GraphicsSystem::Get()->GetContext();
	context->VSSetShaderResources(slot, 1, &mShaderResourceView);
}

void Texture::BindPS(uint32_t slot) const
{
	auto context = GraphicsSystem::Get()->GetContext();
	context->PSSetShaderResources(slot, 1, &mShaderResourceView);
}

void* Texture::GetRawData() const
{
	return mShaderResourceView;
}

uint32_t Texture::GetWidth() const
{
	return mWidth;
}

uint32_t Texture::GetHeight() const
{
	return mHeight;
}

DXGI_FORMAT Texture::GetDXGIFormat(Format format)
{
	switch (format)
	{
	case Format::RGBA_U8: return DXGI_FORMAT_R8G8B8A8_UNORM;
	case Format::RGBA_U32: return DXGI_FORMAT_R32G32B32A32_UINT;
	default:
		break;
	}
	return DXGI_FORMAT_R8G8B8A8_UNORM;
}
