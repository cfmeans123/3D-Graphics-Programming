#include "Precompile.h"
#include "../Inc/MeshBuffer.h"

#include "../Inc/MeshTypes.h"
#include "../Inc/GraphicsSystem.h"

using namespace MEngine;
using namespace MEngine::Graphics;


void MEngine::Graphics::MeshBuffer::Initialize(const void* vertices, uint32_t vertexSize, uint32_t vertexCount)
{
    CreateVertexBuffer(vertices, vertexSize, vertexCount);
}

void MEngine::Graphics::MeshBuffer::Initialize(const void* vertices, uint32_t vertexSize, uint32_t vertexCount, const uint32_t* indices, uint32_t indexCount)
{
    CreateVertexBuffer(vertices, vertexSize, vertexCount);
    CreateIndexBuffer(indices, indexCount);
}

void MEngine::Graphics::MeshBuffer::Terminate()
{
    SafeRelease(mVertexBuffer);
    SafeRelease(mIndexBuffer);
}

void MEngine::Graphics::MeshBuffer::SetTopology(Topology topology)
{
    switch (topology)
    {
    case Topology::Points:
        mTopology = D3D11_PRIMITIVE_TOPOLOGY_POINTLIST;
        break;
    case Topology::Lines:
        mTopology = D3D11_PRIMITIVE_TOPOLOGY_LINELIST;
        break;
    case Topology::Triangles:
        mTopology = D3D11_PRIMITIVE_TOPOLOGY_TRIANGLELIST;
        break;
    default:
        break;
    }
}

void MEngine::Graphics::MeshBuffer::Render()
{
    auto context = GraphicsSystem::Get()->GetContext();
    context->IASetPrimitiveTopology(mTopology);

    UINT offset = 0;
    context->IASetVertexBuffers(0, 1, &mVertexBuffer, &mVertexSize, &offset);
    if (mIndexBuffer != nullptr)
    {
        context->IASetIndexBuffer(mIndexBuffer, DXGI_FORMAT_R32_UINT, 0);
        context->DrawIndexed((UINT)mIndexCount, 0, 0);
    }
    else
    {
        context->Draw(static_cast<UINT>(mVertexCount), 0);
    }

}

void MEngine::Graphics::MeshBuffer::CreateVertexBuffer(const void* vertices, uint32_t vertexSize, uint32_t vertexCount)
{
    mVertexSize = vertexSize;
    mVertexCount = vertexCount;
    auto device = GraphicsSystem::Get()->GetDevice();

    D3D11_BUFFER_DESC bufferDesc = {};
    bufferDesc.ByteWidth = static_cast<UINT>(vertexCount) * vertexSize;
    bufferDesc.Usage = D3D11_USAGE_DEFAULT;
    bufferDesc.BindFlags = D3D11_BIND_VERTEX_BUFFER;
    bufferDesc.MiscFlags = 0;
    bufferDesc.StructureByteStride = 0;

    D3D11_SUBRESOURCE_DATA initData = {};
    initData.pSysMem = vertices;

    HRESULT hr = device->CreateBuffer(&bufferDesc, &initData, &mVertexBuffer);
    ASSERT(SUCCEEDED(hr), "Failed to create vertex data");
}

void MEngine::Graphics::MeshBuffer::CreateIndexBuffer(const void* indices, uint32_t indexCount)
{
    if (indexCount == 0)
    {
        return;
    }
    mIndexCount = indexCount;

    auto device = GraphicsSystem::Get()->GetDevice();

    D3D11_BUFFER_DESC bufferDesc = {};
    bufferDesc.ByteWidth = static_cast<UINT>(indexCount) * sizeof(uint32_t);
    bufferDesc.Usage = D3D11_USAGE_DEFAULT;
    bufferDesc.BindFlags = D3D11_BIND_VERTEX_BUFFER;
    bufferDesc.MiscFlags = 0;
    bufferDesc.StructureByteStride = 0;

    D3D11_SUBRESOURCE_DATA initData = {};
    initData.pSysMem = indices;

    HRESULT hr = device->CreateBuffer(&bufferDesc, &initData, &mIndexBuffer);
    ASSERT(SUCCEEDED(hr), "Failed to create index data");
}
