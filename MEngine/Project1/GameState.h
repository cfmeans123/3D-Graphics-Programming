#pragma once

#include "..\Engine\MEngine\Inc\MEngine.h"



class GameState : public MEngine::AppState
{
public:
	void Initialize();
	void Terminate();
	virtual void Update(float deltaTime);
	void Render();
	void HandleInput();

protected:
	virtual void CreateShape() {};

	MEngine::Graphics::MeshPC mMesh;
	MEngine::Graphics::MeshBuffer mMeshBuffer;
	//auto 
	ID3D11Buffer* mVertexBuffer = nullptr;
	ID3D11Buffer* mIndexBuffer = nullptr;
	ID3D11VertexShader* mVertexShader = nullptr;
	ID3D11InputLayout* mInputLayout = nullptr;
	ID3D11PixelShader* mPixelShader = nullptr;
};

