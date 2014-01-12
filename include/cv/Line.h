// ========================================================== //
// Line
// - Repräsentiert eine Line durch zwei Punkte
//
// Shivan Taher <shi.taher@gmail.com>
// Created: 15.05.2011
// ========================================================== //

#ifndef _LINE
#define _LINE

#include "ComputerVision.h"

namespace ComputerVision {
	using namespace std;

	class Line {
		public:
			Line() {}
			Line(Vec2 a, Vec2 b);
			~Line();

			void draw(Bitmap* img, Color color);
			// Schreibt die Koordinaten der Punkte in ein vector<Vec2> Objekt (Bresenhams Linienalgorithmus)
			vector<Vec2> drawToVektor();

			Vec2 a, b;
	};
}

#endif
