#pragma once

namespace MEngine::Graphics
{
	class Texture
	{
	public:

		enum class Format
		{
			RGBA_U8,
			RGBA_U32
		};

		Texture() = default;
		virtual ~Texture();

		//delete copy
		Texture(const Texture&) = delete;
		Texture& operator=(const Texture&) = delete;
		//allow the move
		Texture(Texture&& rhs) noexcept;
		Texture& operator=(Texture&& rhs) noexcept;

		virtual void Initialize(const std::filesystem::path& fileName);
		virtual void Initialize(uint32_t width, uint32_t height, Format format);
		virtual void Terminate();

		void BindVS(uint32_t slot) const;
		void BindPS(uint32_t slot) const;

		void* GetRawData() const;

	protected:
		DXGI_FORMAT GetDXGIFormat(Format format);
		ID3D11ShaderResourceView* mShaderResourceView = nullptr;

	};
}