// ========================================================== //
// Math
// - Enthält wichtige mathematische Funktionen in einem
//   eigenen Namespace
//
// Shivan Taher <shi.taher@gmail.com>
// Created: 07.02.2011
// ========================================================== //

#ifndef _MATH_H
#define _MATH_H

#include "ComputerVision.h"

namespace ComputerVision {
	using namespace std;
	
	namespace Math {
		// ============================================================= //
		// Konstanten
		// ============================================================= //
		const double pi = 3.1415926535;
		const double sqrtPi  = 1.77245385;		// Für Radiusberechnung (Wurzel aus Pi)

		// ============================================================= //
		// Umwandlung von Radiants in Degree
		// ============================================================= //
		#define radToDeg(x)		((x) * 57.295736)

		// ============================================================= //
		// Umwandlung von Degree in Radiants
		// ============================================================= //
		#define degToRad(x)		((x) * 0.0174532925)

		#define min3(x, y, z)	(((x)<(y) && (x)<(z))?(x):((y)<(z)?(y):(z)))
		#define max3(x, y, z)	(((x)>(y) && (x)>(z))?(x):((y)>(z)?(y):(z)))

		int signum(int value);

		// ============================================================= //
		// Gibt true zurück, wenn die doubles (in etwa) den selben Wert haben
		// ============================================================= //
		inline bool feq(const double& a, const double& b) {
			return (fabs(a-b)<=a*0.0000001);
		}

		// ============================================================= //
		// Generiert eine Zufallszahl (int) zwischen min und max
		// ============================================================= //
		int		randomInt(int min, int max);

		// ============================================================= //
		// Generiert eine Zufallszahl (float) zwischen min und max
		// ============================================================= //
		float	randomFloat(float min, float max);

		// ============================================================= //
		// Generiert eine Zufallszahl (double) zwischen min und max
		// ============================================================= //
		double	randomDouble(double min, double max);

	}
}

#endif