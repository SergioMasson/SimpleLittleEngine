#pragma once

#include "drawableElement.h"
#include "../../colors.h"

namespace sle
{
	enum class TextAlignment
	{
		Leading,
		Trailing,
		Center,
		Justified
	};

	namespace graphics
	{
		namespace UI
		{
			class GuiText : public DrawableElement
			{
			public:
				SLE_EXPORTS GuiText();
				SLE_EXPORTS GuiText(DrawableElement* parent, float x, float y, float width, float height, float fontSize);
				SLE_EXPORTS virtual void Draw(void) override;

				void SetText(std::wstring text) { m_text = text; }
				std::wstring GetText() const { return m_text; };

				SLE_EXPORTS void SetColor(Color color);
				Color GetColor() const { return m_color; };

				SLE_EXPORTS void SetFontSize(float fontSize);
				float GetFontSize() const { return m_fontSize; }

				SLE_EXPORTS void SetTextAlignment(TextAlignment alignment);
				TextAlignment GetTextAlignment() const { return m_alignment; }

			private:
				Microsoft::WRL::ComPtr<IDWriteTextFormat> m_writer = nullptr;
				Microsoft::WRL::ComPtr<ID2D1SolidColorBrush> m_brush = nullptr;
				std::wstring m_text;
				Color m_color;
				TextAlignment m_alignment;
				float m_fontSize;
			};
		}
	}
}