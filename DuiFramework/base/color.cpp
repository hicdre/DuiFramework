#include "stdafx.h"
#include "color.h"

namespace ui
{
	static int ComponentValue(const char* aColorSpec, int aLen, int color, int dpc)
	{
		int component = 0;
		int index = (color * dpc);
		if (2 < dpc) {
			dpc = 2;
		}
		while (--dpc >= 0) {
			char ch = ((index < aLen) ? aColorSpec[index++] : '0');
			if (('0' <= ch) && (ch <= '9')) {
				component = (component * 16) + (ch - '0');
			}
			else if ((('a' <= ch) && (ch <= 'f')) ||
				(('A' <= ch) && (ch <= 'F'))) {
				// "ch&7" handles lower and uppercase hex alphabetics
				component = (component * 16) + (ch & 7) + 9;
			}
			else {  // not a hex digit, treat it like 0
				component = (component * 16);
			}
		}
		return component;
	}

	static float HSL_HueToRGB(float m1, float m2, float h)
	{
		if (h < 0.0f)
			h += 1.0f;
		if (h > 1.0f)
			h -= 1.0f;
		if (h < (float)(1.0 / 6.0))
			return m1 + (m2 - m1)*h*6.0f;
		if (h < (float)(1.0 / 2.0))
			return m2;
		if (h < (float)(2.0 / 3.0))
			return m1 + (m2 - m1)*((float)(2.0 / 3.0) - h)*6.0f;
		return m1;
	}

	bool HexToRGB(const std::string& aBuf, Color& aResult)
	{
		const char* buffer = aBuf.c_str();

		int nameLen = aBuf.size();
		if ((nameLen == 3) || (nameLen == 6)) {
			// Make sure the digits are legal
			for (int i = 0; i < nameLen; i++) {
				char ch = buffer[i];
				if (((ch >= '0') && (ch <= '9')) ||
					((ch >= 'a') && (ch <= 'f')) ||
					((ch >= 'A') && (ch <= 'F'))) {
					// Legal character
					continue;
				}
				// Whoops. Illegal character.
				return false;
			}

			// Convert the ascii to binary
			int dpc = ((3 == nameLen) ? 1 : 2);
			// Translate components from hex to binary
			int r = ComponentValue(buffer, nameLen, 0, dpc);
			int g = ComponentValue(buffer, nameLen, 1, dpc);
			int b = ComponentValue(buffer, nameLen, 2, dpc);
			if (dpc == 1) {
				// Scale single digit component to an 8 bit value. Replicate the
				// single digit to compute the new value.
				r = (r << 4) | r;
				g = (g << 4) | g;
				b = (b << 4) | b;
			}
			assert((r >= 0) && (r <= 255));
			assert((g >= 0) && (g <= 255));
			assert((b >= 0) && (b <= 255));
			aResult = ColorSetRGB(r, g, b);
			return true;
		}

		// Improperly formatted color value
		return false;
	}

	Color HSL2RGB(float h, float s, float l)
	{
		uint8 r, g, b;
		float m1, m2;
		if (l <= 0.5f) {
			m2 = l*(s + 1);
		}
		else {
			m2 = l + s - l*s;
		}
		m1 = l * 2 - m2;
		r = uint8(255 * HSL_HueToRGB(m1, m2, h + 1.0f / 3.0f));
		g = uint8(255 * HSL_HueToRGB(m1, m2, h));
		b = uint8(255 * HSL_HueToRGB(m1, m2, h - 1.0f / 3.0f));
		return ColorSetRGB(r, g, b);
	}

}