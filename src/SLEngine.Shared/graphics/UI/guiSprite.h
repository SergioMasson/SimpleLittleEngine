#pragma once

#include "drawableElement.h"

namespace sle {
	namespace graphics
	{
		namespace UI
		{
			class GuiSprite : public DrawableElement
			{
			public:
				SLE_EXPORTS GuiSprite(DrawableElement* parent, float x, float y, float width, float height);
				SLE_EXPORTS void LoadBitmapFromFile(std::wstring filePath, bool setNativeSize = false);
				SLE_EXPORTS virtual void Draw(void) override;

			private:
				Microsoft::WRL::ComPtr<ID2D1Bitmap> m_bitmap;
			};
		}
	}
}