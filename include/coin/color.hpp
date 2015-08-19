#pragma once

#include <string>
#include <cstdint>

namespace coin {

namespace _impl_color {

constexpr uint32_t k_max_color = 256;

struct RGB {
	enum ergb { red, green, blue };
	uint8_t rgb[blue+1];
	constexpr uint8_t& operator[](ergb x) {
		// assert(x >= 0 && x < blue+1);
		return rgb[x];
	}
};

struct Color {
	uint32_t  color;

	Color() : color{} {} // non-class value-initialization, value is 0
	Color(const uint8_t red, const uint8_t green, const uint8_t blue)
		: color{((red % k_max_color) << 16) + ((green % k_max_color) << 8) + (blue % k_max_color)}
		{}

	explicit Color(const uint32_t c) : color(c) {}

	RGB computeRGB() const {
		RGB rgb;
		rgb[RGB::red]   = (color >> 16) & (k_max_color - 1);
		rgb[RGB::green] = (color >> 8) & (k_max_color - 1);
		rgb[RGB::blue]  = color & (k_max_color - 1);
		return rgb;
	}

	std::string to_string() const {
		RGB rgb = computeRGB();
		return std::string("{" + std::to_string(rgb[RGB::red]) + "," + std::to_string(rgb[RGB::green]) 
			+ "," + std::to_string(rgb[RGB::blue]) + "}:" + std::to_string(color));
	}
};

} // namespace _impl_color

using _impl_color::RGB;
using _impl_color::Color;

namespace color {
	
	constexpr uint32_t aliceblue       = 0xF0F8FF;
	constexpr uint32_t antiquewhite    = 0xFAEBD7;
	constexpr uint32_t aqua            = 0x00FFFF;
	constexpr uint32_t aquamarine      = 0x7FFFD4;
	constexpr uint32_t azure           = 0xF0FFFF;
	constexpr uint32_t beige           = 0xF5F5DC;
	constexpr uint32_t bisque          = 0xFFE4C4;
	constexpr uint32_t black           = 0x000000;
	constexpr uint32_t blanchedalmond  = 0xFFEBCD;
	constexpr uint32_t blue            = 0x0000FF;
	constexpr uint32_t blueviolet      = 0x8A2BE2;
	constexpr uint32_t brown           = 0xA52A2A;
	constexpr uint32_t burlywood       = 0xDEB887;
	constexpr uint32_t cadetblue       = 0x5F9EA0;
	constexpr uint32_t chartreuse      = 0x7FFF00;
	constexpr uint32_t chocolate       = 0xD2691E;
	constexpr uint32_t coral           = 0xFF7F50;
	constexpr uint32_t cornflowerblue  = 0x6495ED;
	constexpr uint32_t cornsilk        = 0xFFF8DC;
	constexpr uint32_t crimson         = 0xDC143C;
	constexpr uint32_t cyan            = 0x00FFFF;
	constexpr uint32_t darkblue        = 0x00008B;
	constexpr uint32_t darkcyan        = 0x008B8B;
	constexpr uint32_t darkgoldenrod   = 0xB8860B;
	constexpr uint32_t darkgray        = 0xA9A9A9;
	constexpr uint32_t darkgreen       = 0x006400;
	constexpr uint32_t darkkhaki       = 0xBDB76B;
	constexpr uint32_t darkmagenta     = 0x8B008B;
	constexpr uint32_t darkolivegreen  = 0x556B2F;
	constexpr uint32_t darkorange      = 0xFF8C00;
	constexpr uint32_t darkorchid      = 0x9932CC;
	constexpr uint32_t darkred         = 0x8B0000;
	constexpr uint32_t darksalmon      = 0xE9967A;
	constexpr uint32_t darkseagreen    = 0x8FBC8F;
	constexpr uint32_t darkslateblue   = 0x483D8B;
	constexpr uint32_t darkslategray   = 0x2F4F4F;
	constexpr uint32_t darkturquoise   = 0x00CED1;
	constexpr uint32_t darkviolet      = 0x9400D3;
	constexpr uint32_t deeppink        = 0xFF1493;
	constexpr uint32_t deepskyblue     = 0x00BFFF;
	constexpr uint32_t dimgray         = 0x696969;
	constexpr uint32_t dodgerblue      = 0x1E90FF;
	constexpr uint32_t firebrick       = 0xB22222;
	constexpr uint32_t floralwhite     = 0xFFFAF0;
	constexpr uint32_t forestgreen     = 0x228B22;
	constexpr uint32_t fuchsia         = 0xFF00FF;
	constexpr uint32_t gainsboro       = 0xDCDCDC;
	constexpr uint32_t ghostwhite      = 0xF8F8FF;
	constexpr uint32_t gold            = 0xFFD700;
	constexpr uint32_t goldenrod       = 0xDAA520;
	constexpr uint32_t gray            = 0x808080;
	constexpr uint32_t green           = 0x008000;
	constexpr uint32_t greenyellow     = 0xADFF2F;
	constexpr uint32_t honeydew        = 0xF0FFF0;
	constexpr uint32_t hotpink         = 0xFF69B4;
	constexpr uint32_t indianred       = 0xCD5C5C;
	constexpr uint32_t indigo          = 0x4B0082;
	constexpr uint32_t ivory           = 0xFFFFF0;
	constexpr uint32_t khaki           = 0xF0E68C;
	constexpr uint32_t lavender        = 0xE6E6FA;
	constexpr uint32_t lavenderblush   = 0xFFF0F5;
	constexpr uint32_t lawngreen       = 0x7CFC00;
	constexpr uint32_t lemonchiffon    = 0xFFFACD;
	constexpr uint32_t lightblue       = 0xADD8E6;
	constexpr uint32_t lightcoral      = 0xF08080;
	constexpr uint32_t lightcyan       = 0xE0FFFF;
	constexpr uint32_t lightgoldenrodyellow = 0xFAFAD2;
	constexpr uint32_t lightgreen      = 0x90EE90;
	constexpr uint32_t lightgrey       = 0xD3D3D3;
	constexpr uint32_t lightpink       = 0xFFB6C1;
	constexpr uint32_t lightsalmon     = 0xFFA07A;
	constexpr uint32_t lightseagreen   = 0x20B2AA;
	constexpr uint32_t lightskyblue    = 0x87CEFA;
	constexpr uint32_t lightslategray  = 0x778899;
	constexpr uint32_t lightsteelblue  = 0xB0C4DE;
	constexpr uint32_t lightyellow     = 0xFFFFE0;
	constexpr uint32_t lime            = 0x00FF00;
	constexpr uint32_t limegreen       = 0x32CD32;
	constexpr uint32_t linen           = 0xFAF0E6;
	constexpr uint32_t magenta         = 0xFF00FF;
	constexpr uint32_t maroon          = 0x800000;
	constexpr uint32_t mediumaquamarine= 0x66CDAA;
	constexpr uint32_t mediumblue      = 0x0000CD;
	constexpr uint32_t mediumorchid    = 0xBA55D3;
	constexpr uint32_t mediumpurple    = 0x9370DB;
	constexpr uint32_t mediumseagreen  = 0x3CB371;
	constexpr uint32_t mediumslateblue = 0x7B68EE;
	constexpr uint32_t mediumspringgreen=0x00FA9A;
	constexpr uint32_t mediumturquoise = 0x48D1CC;
	constexpr uint32_t mediumvioletred = 0xC71585;
	constexpr uint32_t midnightblue    = 0x191970;
	constexpr uint32_t mintcream       = 0xF5FFFA;
	constexpr uint32_t mistyrose       = 0xFFE4E1;
	constexpr uint32_t moccasin        = 0xFFE4B5;
	constexpr uint32_t navajowhite     = 0xFFDEAD;
	constexpr uint32_t navy            = 0x000080;
	constexpr uint32_t oldlace         = 0xFDF5E6;
	constexpr uint32_t olive           = 0x808000;
	constexpr uint32_t olivedrab       = 0x6B8E23;
	constexpr uint32_t orange          = 0xFFA500;
	constexpr uint32_t orangered       = 0xFF4500;
	constexpr uint32_t orchid          = 0xDA70D6;
	constexpr uint32_t palegoldenrod   = 0xEEE8AA;
	constexpr uint32_t palegreen       = 0x98FB98;
	constexpr uint32_t paleturquoise   = 0xAFEEEE;
	constexpr uint32_t palevioletred   = 0xDB7093;
	constexpr uint32_t papayawhip      = 0xFFEFD5;
	constexpr uint32_t peachpuff       = 0xFFDAB9;
	constexpr uint32_t peru            = 0xCD853F;
	constexpr uint32_t pink            = 0xFFC0CB;
	constexpr uint32_t plum            = 0xDDA0DD;
	constexpr uint32_t powderblue      = 0xB0E0E6;
	constexpr uint32_t purple          = 0x800080;
	constexpr uint32_t red             = 0xFF0000;
	constexpr uint32_t rosybrown       = 0xBC8F8F;
	constexpr uint32_t royalblue       = 0x4169E1;
	constexpr uint32_t saddlebrown     = 0x8B4513;
	constexpr uint32_t salmon          = 0xFA8072;
	constexpr uint32_t sandybrown      = 0xF4A460;
	constexpr uint32_t seagreen        = 0x2E8B57;
	constexpr uint32_t seashell        = 0xFFF5EE;
	constexpr uint32_t sienna          = 0xA0522D;
	constexpr uint32_t silver          = 0xC0C0C0;
	constexpr uint32_t skyblue         = 0x87CEEB;
	constexpr uint32_t slateblue       = 0x6A5ACD;
	constexpr uint32_t slategray       = 0x708090;
	constexpr uint32_t snow            = 0xFFFAFA;
	constexpr uint32_t springgreen     = 0x00FF7F;
	constexpr uint32_t steelblue       = 0x4682B4;
	constexpr uint32_t tancolor        = 0xD2B48C;
	constexpr uint32_t teal            = 0x008080;
	constexpr uint32_t thistle         = 0xD8BFD8;
	constexpr uint32_t tomato          = 0xFF6347;
	constexpr uint32_t turquoise       = 0x40E0D0;
	constexpr uint32_t violetlight     = 0xEE82EE;
	constexpr uint32_t wheat           = 0xF5DEB3;
	constexpr uint32_t white           = 0xFFFFFF;
	constexpr uint32_t whitesmoke      = 0xF5F5F5;
	constexpr uint32_t yellow          = 0xFFFF00;
	constexpr uint32_t yellowgreen     = 0x9ACD32;

} // namespace color


} // namespace coin

