#pragma once

namespace MEngine::Graphics
{
	class PixelShader final
	{
		
	public:
		void Initialize(const std::filesystem::path& filePath, const char* entryPoint = "PS");
		void Terminate();
		void Bind();

	private:
		ID3D11PixelShader* mPixelShader = nullptr;
	};
}