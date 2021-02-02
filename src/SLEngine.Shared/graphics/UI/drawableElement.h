#pragma once

namespace sle {
	namespace graphics
	{
		namespace UI
		{
			enum class ParentAnchorType
			{
				TopLeft,
				TopRight,
				BottomLeft,
				BottomRight,
				Center
			};

			class  DrawableElement
			{
			public:
				DrawableElement() {};
				DrawableElement(DrawableElement* parent, float x, float y, float width, float height, ParentAnchorType anchorType = ParentAnchorType::TopLeft) :
					m_parent{ parent },
					m_anchorType{ anchorType },
					m_X{ x },
					m_Y{ y },
					m_Width{ width },
					m_Height{ height }{};

				DrawableElement(const DrawableElement&) = delete;
				DrawableElement& operator=(const DrawableElement&) = delete;

				virtual void Draw(void) = 0;
				void GetScreenPoition(float& x, float& y);
				void SetLocalPosition(float x, float y);
				void SetAnchorType(ParentAnchorType type) { m_anchorType = type; }

				float GetWidth() const
				{
					return m_Width;
				};
				void SetWidth(float width)
				{
					m_Width = width;
				}
				float GetHeight() const {
					return m_Height;
				}
				void SetHeight(float height)
				{
					m_Height = height;
				}
				void SetParent(DrawableElement* parent)
				{
					m_parent = parent;
				}

			protected:

				float m_X = 0.0f;
				float m_Y = 0.0f;
				float m_Width = 0.0f;
				float m_Height = 0.0f;
				DrawableElement* m_parent = nullptr;
				ParentAnchorType m_anchorType = ParentAnchorType::BottomLeft;
			};
		}
	}
}