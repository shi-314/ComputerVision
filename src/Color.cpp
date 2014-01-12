// ========================================================== //
// Color
// RGB Farbe (32Bit)
//
// Shivan Taher
// 12.04.2008
// Contact: shi.taher@gmail.com
// ========================================================== //
// Color.h
// ========================================================== //

#include <cv/ComputerVision.h>

namespace ComputerVision
{
	// ========================================================== //
	// Color
	// ========================================================== //
	Color::Color()
	{
		this->r = 0;
		this->g = 0;
		this->b = 0;
	}

	Color::Color(unsigned char r, unsigned char g, unsigned char b)
	{
		this->r = r;
		this->g = g;
		this->b = b;
	}

	Color::Color(unsigned char v, unsigned char w)
	{
		this->b = v;
		this->g = w;
	}

	Color::Color(unsigned char v)
	{
		this->b = v;
	}

	// ========================================================== //
	// operator <
	// ========================================================== //
	bool Color::operator < (Color color)
	{
		if(this->r < color.r && this->g < color.g && this->b < color.b)
		{
			return true;
		}
		return false;
	}

	bool Color::operator <= (Color color)
	{
		if(this->r <= color.r && this->g <= color.g && this->b <= color.b)
		{
			return true;
		}
		return false;
	}

	// ========================================================== //
	// operator >
	// ========================================================== //
	bool Color::operator > (Color color)
	{
		if(this->r > color.r && this->g > color.g && this->b > color.b)
		{
			return true;
		}
		return false;
	}

	bool Color::operator >= (Color color)
	{
		if(this->r >= color.r && this->g >= color.g && this->b >= color.b)
		{
			return true;
		}
		return false;
	}

	// ========================================================== //
	// getHSV
	// ========================================================== //
	HSVColor Color::getHSV()
	{
		HSVColor hsv;

		double maxC = b;
		if (maxC < g) maxC = g;
		if (maxC < r) maxC = r;
		double minC = b;
		if (minC > g) minC = g;
		if (minC > r) minC = r;

		double delta = maxC - minC;

		hsv.v = maxC/255;
		hsv.s = 0;
		hsv.h = 0;

		if (delta == 0) {
			hsv.h = 0;
			hsv.s = 0;
		} else {
			hsv.s = delta / maxC;
			double dR = 60*(maxC - r)/delta + 180;
			double dG = 60*(maxC - g)/delta + 180;
			double dB = 60*(maxC - b)/delta + 180;
			if (r == maxC)
				hsv.h = dB - dG;
			else if (g == maxC)
				hsv.h = 120 + dR - dB;
			else
				hsv.h = 240 + dG - dR;
		}

		if (hsv.h<0)
			hsv.h+=360;
		if (hsv.h>=360)
			hsv.h-=360;

		return hsv;
	}
}