#pragma once

namespace MEngine::Graphics
{
	class Sampler final
	{
	public:
		enum class Filter
		{
			Point,
			Linear,
			Anisotropic
		};
		enum class AddressMode
		{
			Border,
			Clamp,
			Mirror,
			Wrap
		};

		Sampler() = default;
		~Sampler();

		Sampler(const Sampler&) = delete;
		Sampler(const Sampler&&) = delete;
		Sampler& operator=(Sampler&) = delete;
		Sampler& operator=(Sampler&&) = delete;

		void Initialize(Filter filter, AddressMode addressMode);
		void Terminate();

		void BindVS(uint32_t slot);
		void BindPS(uint32_t slot);

	private:
		ID3D11SamplerState* mSampler = nullptr;
	};
}