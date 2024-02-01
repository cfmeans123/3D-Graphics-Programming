#pragma once

#include "..\..\Engine\MEngine\Inc\MEngine.h"



class GameState : public MEngine::AppState
{
public:
	void Initialize();
	void Terminate();
	virtual void Update(float deltaTime);
	void Render();

protected:
	virtual void CreateShape() {};

	struct Vertex
	{
		MEngine::Math::Vector3 position;
		MEngine::Color color;
	};
	using Vertices = std::vector<Vertex>;
	Vertices mVertices;
	ID3D11Buffer* mVertexBuffer = nullptr;
	ID3D11VertexShader* mVertexShader = nullptr;
	ID3D11InputLayout* mInputLayout = nullptr;
	ID3D11PixelShader* mPixelShader = nullptr;
};

class TriangleState : public GameState
{
public:
	void Update(float deltaTime) override;

protected:
	void CreateShape() override;
};

class SquareState : public GameState
{
public:
	void Update(float deltaTime) override;

protected:
	void CreateShape() override;
};