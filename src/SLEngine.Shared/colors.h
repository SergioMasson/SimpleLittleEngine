#pragma once

namespace sle
{
	class Color
	{
	public:

		inline Color()
		{
			m_color.x = 0.0f;
			m_color.y = 0.0f;
			m_color.z = 0.0f;
			m_color.w = 0.0f;
		}

		inline Color(DirectX::XMFLOAT4 vec)
		{
			m_color = vec;
		}

		inline Color(DirectX::XMFLOAT3 vec, float alpha)
		{
			m_color.x = vec.x;
			m_color.y = vec.y;
			m_color.z = vec.z;
			m_color.w = alpha;
		}

		inline Color(UINT32 rgb, FLOAT a = 1.0)
		{
			Init(rgb, a);
		}

		inline Color(Color color, float alpha)
		{
			m_color.x = color.m_color.x;
			m_color.y = color.m_color.y;
			m_color.z = color.m_color.z;
			m_color.w = alpha;
		}

		inline Color(
			FLOAT red,
			FLOAT green,
			FLOAT blue,
			FLOAT alpha = 1.0
		)
		{
			m_color.x = red;
			m_color.y = green;
			m_color.z = blue;
			m_color.w = alpha;
		}

		operator D2D1::ColorF() const { return D2D1::ColorF(m_color.x, m_color.y, m_color.z, m_color.w); };
		operator DirectX::XMFLOAT4() const { return m_color; };

	private:

		inline void Init(UINT32 rgb, FLOAT alpha)
		{
			m_color.x = static_cast<FLOAT>((rgb & sc_redMask) >> sc_redShift) / 255.f;
			m_color.y = static_cast<FLOAT>((rgb & sc_greenMask) >> sc_greenShift) / 255.f;
			m_color.z = static_cast<FLOAT>((rgb & sc_blueMask) >> sc_blueShift) / 255.f;
			m_color.w = alpha;
		}

		DirectX::XMFLOAT4 m_color;

		static const UINT32 sc_redShift = 16;
		static const UINT32 sc_greenShift = 8;
		static const UINT32 sc_blueShift = 0;

		static const UINT32 sc_redMask = 0xff << sc_redShift;
		static const UINT32 sc_greenMask = 0xff << sc_greenShift;
		static const UINT32 sc_blueMask = 0xff << sc_blueShift;

	public:
		SLE_EXPORTS static const Color AliceBlue;
		SLE_EXPORTS static const Color AntiqueWhite;
		SLE_EXPORTS static const Color Aqua;
		SLE_EXPORTS static const Color Aquamarine;
		SLE_EXPORTS static const Color Azure;
		SLE_EXPORTS static const Color Beige;
		SLE_EXPORTS static const Color Bisque;
		SLE_EXPORTS static const Color Black;
		SLE_EXPORTS static const Color BlanchedAlmond;
		SLE_EXPORTS static const Color Blue;
		SLE_EXPORTS static const Color BlueViolet;
		SLE_EXPORTS static const Color Brown;
		SLE_EXPORTS static const Color BurlyWood;
		SLE_EXPORTS static const Color CadetBlue;
		SLE_EXPORTS static const Color Chartreuse;
		SLE_EXPORTS static const Color Chocolate;
		SLE_EXPORTS static const Color Coral;
		SLE_EXPORTS static const Color CornflowerBlue;
		SLE_EXPORTS static const Color Cornsilk;
		SLE_EXPORTS static const Color Crimson;
		SLE_EXPORTS static const Color Cyan;
		SLE_EXPORTS static const Color DarkBlue;
		SLE_EXPORTS static const Color DarkCyan;
		SLE_EXPORTS static const Color DarkGoldenrod;
		SLE_EXPORTS static const Color DarkGray;
		SLE_EXPORTS static const Color DarkGreen;
		SLE_EXPORTS static const Color DarkKhaki;
		SLE_EXPORTS static const Color DarkMagenta;
		SLE_EXPORTS static const Color DarkOliveGreen;
		SLE_EXPORTS static const Color DarkOrange;
		SLE_EXPORTS static const Color DarkOrchid;
		SLE_EXPORTS static const Color DarkRed;
		SLE_EXPORTS static const Color DarkSalmon;
		SLE_EXPORTS static const Color DarkSeaGreen;
		SLE_EXPORTS static const Color DarkSlateBlue;
		SLE_EXPORTS static const Color DarkSlateGray;
		SLE_EXPORTS static const Color DarkTurquoise;
		SLE_EXPORTS static const Color DarkViolet;
		SLE_EXPORTS static const Color DeepPink;
		SLE_EXPORTS static const Color DeepSkyBlue;
		SLE_EXPORTS static const Color DimGray;
		SLE_EXPORTS static const Color DodgerBlue;
		SLE_EXPORTS static const Color Firebrick;
		SLE_EXPORTS static const Color FloralWhite;
		SLE_EXPORTS static const Color ForestGreen;
		SLE_EXPORTS static const Color Fuchsia;
		SLE_EXPORTS static const Color Gainsboro;
		SLE_EXPORTS static const Color GhostWhite;
		SLE_EXPORTS static const Color Gold;
		SLE_EXPORTS static const Color Goldenrod;
		SLE_EXPORTS static const Color Gray;
		SLE_EXPORTS static const Color Green;
		SLE_EXPORTS static const Color GreenYellow;
		SLE_EXPORTS static const Color Honeydew;
		SLE_EXPORTS static const Color HotPink;
		SLE_EXPORTS static const Color IndianRed;
		SLE_EXPORTS static const Color Indigo;
		SLE_EXPORTS static const Color Ivory;
		SLE_EXPORTS static const Color Khaki;
		SLE_EXPORTS static const Color Lavender;
		SLE_EXPORTS static const Color LavenderBlush;
		SLE_EXPORTS static const Color LawnGreen;
		SLE_EXPORTS static const Color LemonChiffon;
		SLE_EXPORTS static const Color LightBlue;
		SLE_EXPORTS static const Color LightCoral;
		SLE_EXPORTS static const Color LightCyan;
		SLE_EXPORTS static const Color LightGoldenrodYellow;
		SLE_EXPORTS static const Color LightGreen;
		SLE_EXPORTS static const Color LightGray;
		SLE_EXPORTS static const Color LightPink;
		SLE_EXPORTS static const Color LightSalmon;
		SLE_EXPORTS static const Color LightSeaGreen;
		SLE_EXPORTS static const Color LightSkyBlue;
		SLE_EXPORTS static const Color LightSlateGray;
		SLE_EXPORTS static const Color LightSteelBlue;
		SLE_EXPORTS static const Color LightYellow;
		SLE_EXPORTS static const Color Lime;
		SLE_EXPORTS static const Color LimeGreen;
		SLE_EXPORTS static const Color Linen;
		SLE_EXPORTS static const Color Magenta;
		SLE_EXPORTS static const Color Maroon;
		SLE_EXPORTS static const Color MediumAquamarine;
		SLE_EXPORTS static const Color MediumBlue;
		SLE_EXPORTS static const Color MediumOrchid;
		SLE_EXPORTS static const Color MediumPurple;
		SLE_EXPORTS static const Color MediumSeaGreen;
		SLE_EXPORTS static const Color MediumSlateBlue;
		SLE_EXPORTS static const Color MediumSpringGreen;
		SLE_EXPORTS static const Color MediumTurquoise;
		SLE_EXPORTS static const Color MediumVioletRed;
		SLE_EXPORTS static const Color MidnightBlue;
		SLE_EXPORTS static const Color MintCream;
		SLE_EXPORTS static const Color MistyRose;
		SLE_EXPORTS static const Color Moccasin;
		SLE_EXPORTS static const Color NavajoWhite;
		SLE_EXPORTS static const Color Navy;
		SLE_EXPORTS static const Color OldLace;
		SLE_EXPORTS static const Color Olive;
		SLE_EXPORTS static const Color OliveDrab;
		SLE_EXPORTS static const Color Orange;
		SLE_EXPORTS static const Color OrangeRed;
		SLE_EXPORTS static const Color Orchid;
		SLE_EXPORTS static const Color PaleGoldenrod;
		SLE_EXPORTS static const Color PaleGreen;
		SLE_EXPORTS static const Color PaleTurquoise;
		SLE_EXPORTS static const Color PaleVioletRed;
		SLE_EXPORTS static const Color PapayaWhip;
		SLE_EXPORTS static const Color PeachPuff;
		SLE_EXPORTS static const Color Peru;
		SLE_EXPORTS static const Color Pink;
		SLE_EXPORTS static const Color Plum;
		SLE_EXPORTS static const Color PowderBlue;
		SLE_EXPORTS static const Color Purple;
		SLE_EXPORTS static const Color Red;
		SLE_EXPORTS static const Color RosyBrown;
		SLE_EXPORTS static const Color RoyalBlue;
		SLE_EXPORTS static const Color SaddleBrown;
		SLE_EXPORTS static const Color Salmon;
		SLE_EXPORTS static const Color SandyBrown;
		SLE_EXPORTS static const Color SeaGreen;
		SLE_EXPORTS static const Color SeaShell;
		SLE_EXPORTS static const Color Sienna;
		SLE_EXPORTS static const Color Silver;
		SLE_EXPORTS static const Color SkyBlue;
		SLE_EXPORTS static const Color SlateBlue;
		SLE_EXPORTS static const Color SlateGray;
		SLE_EXPORTS static const Color Snow;
		SLE_EXPORTS static const Color SpringGreen;
		SLE_EXPORTS static const Color	SteelBlue;
		SLE_EXPORTS static const Color Tan;
		SLE_EXPORTS static const Color Teal;
		SLE_EXPORTS static const Color Thistle;
		SLE_EXPORTS static const Color Tomato;
		SLE_EXPORTS static const Color Turquoise;
		SLE_EXPORTS static const Color Violet;
		SLE_EXPORTS static const Color Wheat;
		SLE_EXPORTS static const Color White;
		SLE_EXPORTS static const Color WhiteSmoke;
		SLE_EXPORTS static const Color Yellow;
		SLE_EXPORTS static const Color YellowGreen;
	};
}