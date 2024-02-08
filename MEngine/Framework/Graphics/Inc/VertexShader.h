#pragma once


namespace MEngine::Graphics
{
    class VertexShader final
    {
    public:
        template<class VertexType>
        void Initialize(const std::filesystem::path& filePath, uint32_t format)
        {

        }
        void Initialize(const std::filesystem::path& filePath, uint32_t format);
        void Terminate();
        void Bind();
    private:
        ID3D11VertexShader* mVertexShader = nullptr;
        ID3D11InputLayout* mInputLayout = nullptr;
    };
}