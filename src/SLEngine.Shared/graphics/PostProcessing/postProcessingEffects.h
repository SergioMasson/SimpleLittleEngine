#pragma once

namespace sle
{
	namespace graphics
	{
		namespace postEffects
		{
			SLE_EXPORTS void Initialize();
			SLE_EXPORTS void Render();
			SLE_EXPORTS void Resize(UINT width, UINT height);
			SLE_EXPORTS void Shutdown();

			SLE_EXPORTS ID3D11RenderTargetView** GetInitialRenderTarget();
		}
	}
}