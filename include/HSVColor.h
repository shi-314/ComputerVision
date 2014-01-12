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

//Alles public wegen schnellerem Zugriff

#ifndef _HSVCOLOR_H
#define _HSVCOLOR_H

namespace ComputerVision
{
	class HSVColor
	{
	public:
		HSVColor();
		HSVColor(double h, double s, double v);

		bool operator<(HSVColor hsvcolor);
		bool operator<=(HSVColor hsvcolor);

		bool operator>(HSVColor hsvcolor);
		bool operator>=(HSVColor hsvcolor);

		double h, s, v;
	};
}

#endif