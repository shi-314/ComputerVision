// ========================================================== //
// Canny
// - Canny-Algorithmus
//
// Shivan Taher <shi-89@gmx.net>
// Created: 18.05.2011
// ========================================================== //

#ifndef _CANNY_H
#define _CANNY_H

#include "ComputerVision.h"

namespace ComputerVision {
	using namespace std;

	class Canny {
		public:
			Canny(Bitmap* src);
			~Canny();

			void apply(int lowThresh, int highThresh);

		private:
			char suppress(int x, int y, char sector);
			void applyTrack();
			void track(stack<Vec2>* toTrack, int x, int y);

			Bitmap* src;
			int width, height;
			int lowThresh, highThresh;

			Matrix2<float>*		theta;	// Anstieg
			Matrix2<char>*		gradP;	// Sobeloperator in X-Richtung
			Matrix2<char>*		gradQ;	// Sobeloperator in Y-Richtung
			Matrix2<char>*		gradM;	// Absolute Kantenstärke
			Matrix2<char>*		delta;	// Gerundeter Anstieg (0°, 45°, 90°, 135°) <= (0, 1, 2, 3)
			Matrix2<char>*		nms;	// Nonmaxima suppression
			Matrix2<bool>*		marked;	// Markierte Pixel
			Matrix2<char>*		tracked;// Verfolgte Pixel
	};
}

#endif