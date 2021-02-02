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
				 GuiPanel(DrawableElement* parent, float x, float y, float width, float height);
				 virtual void Draw(void) override;
				 void SetColor(Color color);
				 Color GetColor() const;

			private:
				Microsoft::WRL::ComPtr<ID2D1SolidColorBrush> m_brush = nullptr;
				Color m_color;
			};
		}
	}
}