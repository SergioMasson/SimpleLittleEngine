#pragma once

namespace sle {
	namespace graphics
	{
		static const int SwapChainBufferCount = 3;

		SLE_EXPORTS void Initialize(uint32_t width, uint32_t heigth);
		SLE_EXPORTS void Resize(uint32_t width, uint32_t heigth);
		SLE_EXPORTS void BeginDraw();
		SLE_EXPORTS void Present();
		SLE_EXPORTS void ShutDown();

		SLE_EXPORTS extern UINT					g_4xMsaaQuality;
		SLE_EXPORTS extern LONG					g_windowWidth;
		SLE_EXPORTS extern LONG					g_windowHeight;
		SLE_EXPORTS extern D3D11_VIEWPORT g_ScreenViewport;
		SLE_EXPORTS extern D3D_DRIVER_TYPE g_d3dDriverType;
		SLE_EXPORTS extern bool g_Enable4xMsaa;

		SLE_EXPORTS extern Microsoft::WRL::ComPtr<ID3D11Device> g_d3dDevice;
		SLE_EXPORTS extern Microsoft::WRL::ComPtr<ID3D11DeviceContext> g_d3dImmediateContext;
		SLE_EXPORTS extern Microsoft::WRL::ComPtr<IDXGISwapChain1> g_SwapChain;
		SLE_EXPORTS extern Microsoft::WRL::ComPtr<ID3D11Texture2D> g_DepthStencilBuffer;

		SLE_EXPORTS extern Microsoft::WRL::ComPtr<ID3D11RenderTargetView> g_ScreenRenderTargetView;
		SLE_EXPORTS extern Microsoft::WRL::ComPtr<ID3D11DepthStencilView> g_ScreenDepthStencilView;

		//D2DDevices
		SLE_EXPORTS extern Microsoft::WRL::ComPtr<ID2D1Device> g_d2dDevice;
		SLE_EXPORTS extern Microsoft::WRL::ComPtr<ID2D1DeviceContext> g_d2dDeviceContext;
	}
}
