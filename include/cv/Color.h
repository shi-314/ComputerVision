// ========================================================== //
// Color
// RGB Farbe (32Bit)
//
// Shivan Taher
// 12.04.2008
// Contact: shi-89@gmx.net
// ========================================================== //
// Color.h
// ========================================================== //

// Alles public wegen schnellerem Zugriff

#ifndef _COLOR_H
#define _COLOR_H

namespace ComputerVision
{
	class Color
	{
	public:
		Color();
		// 24 Bit Color
		Color(unsigned char r, unsigned char g, unsigned char b);
		// 16 Bit Color
		Color(unsigned char v, unsigned char w);
		// 8 Bit Color
		Color(unsigned char v);

		bool operator<(Color color);
		bool operator<=(Color color);

		bool operator>(Color color);
		bool operator>=(Color color);

		HSVColor getHSV();

		unsigned char r,g,b;
	};
}

#endif