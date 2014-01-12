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

#include "ComputerVision.h"

namespace ComputerVision
{
	// ========================================================== //
	// Color
	// ========================================================== //
	HSVColor::HSVColor()
	{
		this->h = 0;
		this->s = 0;
		this->v = 0;
	}

	HSVColor::HSVColor(double h, double s, double v)
	{
		this->h = h;
		this->s = s;
		this->v = v;
	}


	// ========================================================== //
	// operator <
	// ========================================================== //
	bool HSVColor::operator < (HSVColor hsvcolor)
	{
		if(this->h < hsvcolor.h && this->s < hsvcolor.s && this->v < hsvcolor.v)
		{
			return true;
		}
		return false;
	}

	bool HSVColor::operator <= (HSVColor hsvcolor)
	{
		if(this->h <= hsvcolor.h && this->s <= hsvcolor.s && this->v <= hsvcolor.v)
		{
			return true;
		}
		return false;
	}

	// ========================================================== //
	// operator >
	// ========================================================== //
	bool HSVColor::operator > (HSVColor hsvcolor)
	{
		if(this->h > hsvcolor.h && this->s > hsvcolor.s && this->v > hsvcolor.v)
		{
			return true;
		}
		return false;
	}

	bool HSVColor::operator >= (HSVColor hsvcolor)
	{
		if(this->h >= hsvcolor.h && this->s >= hsvcolor.s && this->v >= hsvcolor.v)
		{
			return true;
		}
		return false;
	}
}