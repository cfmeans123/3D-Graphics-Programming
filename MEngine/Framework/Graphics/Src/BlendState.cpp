#include "Precompiled.h"
#include "BlendState.h"

#include "GraphicsSystem.h"

using namespace MEngine;
using namespace MEngine::Graphics;

namespace
{
	D3D11_BLEND GetSrcBlend(BlendState::Mode mode)
	{
		switch (mode)
		{
		case BlendState::Mode::Additive:		   return D3D11_BLEND_SRC_ALPHA;
		case BlendState::Mode::AlphaBlend:		   return D3D11_BLEND_SRC_ALPHA;
		case BlendState::Mode::AlphaPremultiplied: return D3D11_BLEND_ONE;
		case BlendState::Mode::Opaque:			   return D3D11_BLEND_ONE;
		default:
			ASSERT(false, "BlendState: Unmapped Blend Mode");
			break;
		}

		return D3D11_BLEND_ONE;
	}

	D3D11_BLEND GetDestBlend(BlendState::Mode mode)
	{
		switch (mode)
		{
		case BlendState::Mode::Additive:		   return D3D11_BLEND_ONE;
		case BlendState::Mode::AlphaBlend:		   return D3D11_BLEND_INV_SRC_ALPHA;
		case BlendState::Mode::AlphaPremultiplied: return D3D11_BLEND_INV_SRC_ALPHA;
		case BlendState::Mode::Opaque:			   return D3D11_BLEND_ZERO;
		default:
			ASSERT(false, "BlendState: Unmapped Blend Mode");
			break;
		}

		return D3D11_BLEND_ZERO;
	}
}

void BlendState::ClearState()
{
	auto context = GraphicsSystem::Get()->GetContext();
	context->OMSetBlendState(nullptr, nullptr, UINT_MAX);
	context->OMSetDepthStencilState(nullptr, UINT_MAX);
}

BlendState::~BlendState()
{
	ASSERT(mBlendState == nullptr, "BlendState: terminate must be called");
}

void BlendState::Initialize(Mode mode)
{
	D3D11_BLEND srcBlend = GetSrcBlend(mode);
	D3D11_BLEND destBlend = GetDestBlend(mode);
	D3D11_DEPTH_STENCIL_DESC dsDesc;
	ZeroMemory(&dsDesc, sizeof(dsDesc));
	dsDesc.DepthEnable = true;
	dsDesc.DepthWriteMask = D3D11_DEPTH_WRITE_MASK_ALL;
	dsDesc.DepthFunc = D3D11_COMPARISON_NOT_EQUAL;
	//dsDesc.StencilEnable = false;


	D3D11_BLEND_DESC desc{};
	desc.RenderTarget[0].BlendEnable = (srcBlend != D3D11_BLEND_ONE) || (destBlend != D3D11_BLEND_ZERO);
	desc.RenderTarget[0].SrcBlend = desc.RenderTarget[0].SrcBlendAlpha = srcBlend;
	desc.RenderTarget[0].DestBlend = desc.RenderTarget[0].DestBlendAlpha = destBlend;
	desc.RenderTarget[0].BlendOp = desc.RenderTarget[0].BlendOpAlpha = D3D11_BLEND_OP_ADD;
	desc.RenderTarget[0].RenderTargetWriteMask = D3D11_COLOR_WRITE_ENABLE_ALL;

	auto device = GraphicsSystem::Get()->GetDevice();
	HRESULT hr = device->CreateBlendState(&desc, &mBlendState);
	HRESULT dhr = device->CreateDepthStencilState(&dsDesc, &mDepthStencilState);
	ASSERT(SUCCEEDED(hr), "BlendState: failed to create blend state");
	ASSERT(SUCCEEDED(dhr), "Depth Stencil State: failed to create depth stencil state");
}

void BlendState::Terminate()
{
	SafeRelease(mBlendState);
}

void BlendState::Set()
{
	auto context = GraphicsSystem::Get()->GetContext();
	context->OMSetDepthStencilState(mDepthStencilState, 1);
	context->OMSetBlendState(mBlendState, nullptr, UINT_MAX);
}