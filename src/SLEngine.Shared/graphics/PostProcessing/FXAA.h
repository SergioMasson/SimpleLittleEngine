#pragma once

namespace sle
{
	namespace graphics
	{
		namespace fxaa
		{
			SLE_EXPORTS void Initialize();
			SLE_EXPORTS void Resize(UINT width, UINT height);
			SLE_EXPORTS void Shutdown();
			SLE_EXPORTS void Render(ID3D11RenderTargetView* const* target);

			SLE_EXPORTS extern bool FXAAEnable;
			SLE_EXPORTS extern Microsoft::WRL::ComPtr<ID3D11RenderTargetView> g_pProxyTextureRTV;
		}
	}
}