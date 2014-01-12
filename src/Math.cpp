// ========================================================== //
// Math
// - Enthält wichtige mathematische Funktionen in einem
//   eigenen Namespace
//
// Shivan Taher <shi.taher@gmail.com>
// Created: 07.02.2011
// ========================================================== //

#include <cv/ComputerVision.h>

namespace ComputerVision {
	using namespace std;

	namespace Math {
		int signum(int value) 
		{
			if (value > 0)
				return 1;
			else if (value == 0) 
				return 0;
			else
				return -1;
		}

		// ============================================================= //
		// Generiert eine Zufallszahl (int) zwischen min und max
		// ============================================================= //
		int randomInt(int min, int max)
		{
			return min + rand() * (max - min) / RAND_MAX;
		}

		// ============================================================= //
		// Generiert eine Zufallszahl (float) zwischen min und max
		// ============================================================= //
		float randomFloat(float min, float max)
		{
			return ((max-min)*((float)rand()/RAND_MAX))+min;
		}

		// ============================================================= //
		// Generiert eine Zufallszahl (double) zwischen min und max
		// ============================================================= //
		double randomDouble(double min, double max)
		{
			return ((max-min)*((double)rand()/RAND_MAX))+min;
		}
	}
}