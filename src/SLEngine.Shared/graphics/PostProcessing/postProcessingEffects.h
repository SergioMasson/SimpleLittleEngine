#pragma once

namespace sle
{
	namespace graphics
	{
		namespace postEffects
		{
			void Initialize();
			void Render();
			void Resize(UINT width, UINT height);
			void Shutdown();

			ID3D11RenderTargetView** GetInitialRenderTarget();
		}
	}
}