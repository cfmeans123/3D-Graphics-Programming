#include "GameState.h"

using namespace MEngine;
using namespace MEngine::Graphics;
using namespace MEngine::Input;

void GameState::Initialize()
{
    //create a shape
    CreateShape();
    auto device = GraphicsSystem::Get()->GetDevice();
    //create a way to send data to the GPU
    //We need a Vertex Buffer to do this
    D3D11_BUFFER_DESC bufferDesc = {};
    bufferDesc.ByteWidth = static_cast<UINT>(mVertices.size()) * sizeof(Vertex);
    bufferDesc.Usage = D3D11_USAGE_DEFAULT;
    bufferDesc.BindFlags = D3D11_BIND_VERTEX_BUFFER;
    bufferDesc.MiscFlags = 0;
    bufferDesc.StructureByteStride = 0;

    D3D11_SUBRESOURCE_DATA initData = {};
    initData.pSysMem = mVertices.data();

    HRESULT hr = device->CreateBuffer(&bufferDesc, &initData, &mVertexBuffer);
    ASSERT(SUCCEEDED(hr), "Failed to create vertex data");
    //==================================================================================================
    //==================================================================================================
    std::filesystem::path shaderFilePath = L"../../Assets/Shaders/DoSomething.fx";

    DWORD shaderFlags = D3DCOMPILE_ENABLE_STRICTNESS | D3DCOMPILE_DEBUG;
    ID3DBlob* shaderBlob = nullptr;
    ID3DBlob* errorBlob = nullptr;
    hr = D3DCompileFromFile(
        shaderFilePath.c_str(),
        nullptr,
        D3D_COMPILE_STANDARD_FILE_INCLUDE,
        "VS", "vs_5_0", shaderFlags, 0,
        &shaderBlob,
        &errorBlob
    );
    if (errorBlob != nullptr && errorBlob->GetBufferPointer() != nullptr)
    {
        LOG("s", static_cast<const char*>(errorBlob->GetBufferPointer()));
    }
    ASSERT(SUCCEEDED(hr), "Failed to compile vertex shader");

    hr = device->CreateVertexShader(
        shaderBlob->GetBufferPointer(),
        shaderBlob->GetBufferSize(),
        nullptr,
        &mVertexShader
    );
    ASSERT(SUCCEEDED(hr), "Failed to create vertex shader");
    //==================================================================================================
    //==================================================================================================
    //Create the input layout
    std::vector<D3D11_INPUT_ELEMENT_DESC> vertexLayout;
    vertexLayout.push_back({ "POSITION", 0, DXGI_FORMAT_R32G32B32_FLOAT, 0, D3D11_APPEND_ALIGNED_ELEMENT });
    vertexLayout.push_back({ "COLOR", 0, DXGI_FORMAT_R32G32B32A32_FLOAT, 0, D3D11_APPEND_ALIGNED_ELEMENT });

    hr = device->CreateInputLayout(
        vertexLayout.data(),
        static_cast<UINT>(vertexLayout.size()),
        shaderBlob->GetBufferPointer(),
        shaderBlob->GetBufferSize(),
        &mInputLayout
    );
    ASSERT(SUCCEEDED(hr), "Failed to create input layout");
    SafeRelease(shaderBlob);
    SafeRelease(errorBlob);

    //==================================================================================================
    //==================================================================================================
    //Last thing is to create a pixel shader

    hr = D3DCompileFromFile(
        shaderFilePath.c_str(),
        nullptr,
        D3D_COMPILE_STANDARD_FILE_INCLUDE,
        "PS", "ps_5_0", shaderFlags, 0,
        &shaderBlob,
        &errorBlob
    );
    if (errorBlob != nullptr && errorBlob->GetBufferPointer() != nullptr)
    {
        LOG("s", static_cast<const char*>(errorBlob->GetBufferPointer()));
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
void GameState::Terminate()
{
    mVertices.clear();
    SafeRelease(mPixelShader);
    SafeRelease(mInputLayout);
    SafeRelease(mVertexShader);
    SafeRelease(mVertexBuffer);
}
void GameState::Update(float deltaTime)
{
}

void GameState::Render()
{
    auto context = GraphicsSystem::Get()->GetContext();

    context->VSSetShader(mVertexShader, nullptr, 0);
    context->IASetInputLayout(mInputLayout);
    context->PSSetShader(mPixelShader, nullptr, 0);

    context->IASetPrimitiveTopology(D3D11_PRIMITIVE_TOPOLOGY_TRIANGLELIST);

    UINT stride = sizeof(Vertex);
    UINT offset = 0;
    context->IASetVertexBuffers(0, 1, &mVertexBuffer, &stride, &offset);
    context->Draw(static_cast<UINT>(mVertices.size()), 0);

}

void TriangleState::Update(float deltaTime) 
{
    HandleInput();
}

void TriangleState::CreateShape()
{
    mVertices.push_back({ {-0.5f, 0.0f, 0.0f}, Colors::Red });
    mVertices.push_back({ {0.0f, 0.75f, 0.0f}, Colors::Blue });
    mVertices.push_back({ {0.5f, 0.0f, 0.0f}, Colors::Green });   
}

void SquareState::Update(float deltaTime)
{
    HandleInput();
}

void SquareState::CreateShape()
{
    mVertices.push_back({ {-0.5f, 0.0f, 0.0f}, Colors::Red });
    mVertices.push_back({ {-0.5f, 0.75f, 0.0f}, Colors::Blue });
    mVertices.push_back({ {0.5f, 0.0f, 0.0f}, Colors::Green });

    mVertices.push_back({ {-0.5f, 0.75f, 0.0f}, Colors::Red });
    mVertices.push_back({ {0.5f, 0.75f, 0.0f}, Colors::Blue });
    mVertices.push_back({ {0.5f, 0.0f, 0.0f}, Colors::Green });
}

void TriforceState::Update(float deltaTime)
{
    HandleInput();
}

void TriforceState::CreateShape()
{
    mVertices.push_back({ {0.0f, 0.0f, 0.0f}, Colors::IndianRed });
    mVertices.push_back({ {-0.25f, 0.375f, 0.0f}, Colors::BlueViolet });
    mVertices.push_back({ {0.25f, 0.375f, 0.0f}, Colors::Orange });

    mVertices.push_back({ {-0.5f, 0.0f, 0.0f}, Colors::Red });
    mVertices.push_back({ {0.0f, 0.75f, 0.0f}, Colors::Blue });
    mVertices.push_back({ {0.5f, 0.0f, 0.0f}, Colors::Green });
}

void DiamondState::Update(float deltaTime)
{
    HandleInput();
}

void DiamondState::CreateShape()
{
    mVertices.push_back({ {-0.25f, 0.0f, 0.0f}, Colors::Red });
    mVertices.push_back({ {0.0f, 0.75f, 0.0f}, Colors::Blue });
    mVertices.push_back({ {0.25f, 0.0f, 0.0f}, Colors::Green });

    mVertices.push_back({ {0.25f, 0.0f, 0.0f}, Colors::Green });
    mVertices.push_back({ {0.0f, -0.75f, 0.0f}, Colors::Blue });
    mVertices.push_back({ {-0.25f, 0.0f, 0.0f}, Colors::Red });
}

void GameState::HandleInput()
{
    if (InputSystem::Get()->IsKeyPressed(KeyCode::ONE))
    {
        MainApp().ChangeState("Triangle");
    }
    if (InputSystem::Get()->IsKeyPressed(KeyCode::TWO))
    {
        MainApp().ChangeState("Square");
    }
    if (InputSystem::Get()->IsKeyPressed(KeyCode::THREE))
    {
        MainApp().ChangeState("Triforce");
    }
    if (InputSystem::Get()->IsKeyPressed(KeyCode::FOUR))
    {
        MainApp().ChangeState("Diamond");
    }
}