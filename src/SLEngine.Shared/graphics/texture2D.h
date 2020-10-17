#pragma once

namespace sle
{
	namespace graphics
	{
		class RenderPipeline;
		class MeshRenderer;

		class Texture2D
		{
		public:
			SLE_EXPORTS Texture2D(const wchar_t* filePath);
			SLE_EXPORTS Texture2D() = default;
		private:
			Microsoft::WRL::ComPtr<ID3D11ShaderResourceView> m_resourceView = nullptr;
			Microsoft::WRL::ComPtr<ID3D11Resource> m_resource = nullptr;
			Microsoft::WRL::ComPtr<ID3D11SamplerState> m_samplerState = nullptr;
			friend RenderPipeline;
			friend MeshRenderer;
		};
	}
}