#pragma once
#include "foundation/basictypes.h"
#include <string>


namespace ui
{
	typedef uint32 Color;

	static inline Color ColorSetARGBInline(uint8 a, uint8 r, uint8 g, uint8 b)
	{
		return (a << 24) | (r << 16) | (g << 8) | (b << 0);
	}

#define SkColorSetARGBMacro(a, r, g, b) \
	static_cast<Color>(\
	(static_cast<uint8>(a) << 24) | \
	(static_cast<uint8>(r) << 16) | \
	(static_cast<uint8>(g) << 8) | \
	(static_cast<uint8>(b) << 0))

#if defined(NDEBUG)
#define ColorSetARGB(a, r, g, b) ColorSetARGBMacro(a, r, g, b)
#else
#define ColorSetARGB(a, r, g, b) ColorSetARGBInline(a, r, g, b)
#endif

	/** Return a Color value from 8 bit component values, with an implied value
	of 0xFF for alpha (fully opaque)
	*/
	#define ColorSetRGB(r, g, b)  ColorSetARGB(0xFF, r, g, b)

	/** return the alpha byte from a Color value */
	#define ColorGetA(color)      (((color) >> 24) & 0xFF)
	/** return the red byte from a Color value */
	#define ColorGetR(color)      (((color) >> 16) & 0xFF)
	/** return the green byte from a Color value */
	#define ColorGetG(color)      (((color) >>  8) & 0xFF)
	/** return the blue byte from a Color value */
	#define ColorGetB(color)      (((color) >>  0) & 0xFF)

	enum
	{
		Color_Transparent  = 0x00000000, 
		Color_Black        = 0xFF000000,  //!< black SkColor value
		Color_Darkgray     = 0xFF444444,  //!< dark gray SkColor value
		Color_Gray         = 0xFF888888,  //!< gray SkColor value
		Color_Lightgray    = 0xFFCCCCCC,  //!< light gray SkColor value
		Color_White        = 0xFFFFFFFF,  //!< white SkColor value

		Color_Green		= 0xFF00FF00,  //!< green SkColor value
		Color_Red       = 0xFFFF0000,  //!< red SkColor value
		Color_Blue      = 0xFF0000FF,  //!< blue SkColor value
		Color_Yellow    = 0xFFFFFF00,  //!< yellow SkColor value
		Color_Cyan      = 0xFF00FFFF,  //!< cyan SkColor value
		Color_Magenta   = 0xFFFF00FF,  //!< magenta SkColor value

		
	};


	bool HexToRGB(const std::string& aBuf, Color& aResult);
	Color HSL2RGB(float h, float s, float l);
}