#pragma once

namespace sle
{
	namespace graphics
	{
		namespace fxaa
		{
			 void Initialize();
			 void Resize(UINT width, UINT height);
			 void Shutdown();
			 void Render(ID3D11RenderTargetView* const* target);

			 extern bool FXAAEnable;
			 extern Microsoft::WRL::ComPtr<ID3D11RenderTargetView> g_pProxyTextureRTV;
		}
	}
}