#include "pch.h"
#include "postProcessingEffects.h"
#include "../coreGraphics.h"
#include "FXAA.h"
#include "Bloom.h"

using namespace sle;

void graphics::postEffects::Initialize()
{
	fxaa::Initialize();
	bloom::Initialize();
}

void graphics::postEffects::Render()
{
	if (graphics::fxaa::FXAAEnable)
	{
		if (graphics::bloom::Enable)
			graphics::fxaa::Render(graphics::bloom::g_inputRenderTarget.GetAddressOf());
		else
			graphics::fxaa::Render(graphics::g_ScreenRenderTargetView.GetAddressOf());
	}

	if (graphics::bloom::Enable)
	{
		graphics::bloom::Render(graphics::g_ScreenRenderTargetView.GetAddressOf());
	}
}

void graphics::postEffects::Resize(UINT width, UINT height)
{
	graphics::fxaa::Resize(width, height);
	graphics::bloom::Resize(width, height);
}

void graphics::postEffects::Shutdown()
{
	fxaa::Shutdown();
	bloom::Shutdown();
}

ID3D11RenderTargetView** graphics::postEffects::GetInitialRenderTarget()
{
	if (fxaa::FXAAEnable)
		return fxaa::g_pProxyTextureRTV.GetAddressOf();

	if (bloom::Enable)
		return bloom::g_inputRenderTarget.GetAddressOf();

	return graphics::g_ScreenRenderTargetView.GetAddressOf();
}