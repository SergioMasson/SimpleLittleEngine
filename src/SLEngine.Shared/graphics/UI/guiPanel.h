#pragma once

#include "drawableElement.h"
#include "../../colors.h"

namespace sle {
	namespace graphics
	{
		namespace UI
		{
			class GuiPanel : public DrawableElement
			{
			public:
				SLE_EXPORTS GuiPanel(DrawableElement* parent, float x, float y, float width, float height);
				SLE_EXPORTS virtual void Draw(void) override;
				SLE_EXPORTS void SetColor(Color color);
				SLE_EXPORTS Color GetColor() const;

			private:
				Microsoft::WRL::ComPtr<ID2D1SolidColorBrush> m_brush = nullptr;
				Color m_color;
			};
		}
	}
}