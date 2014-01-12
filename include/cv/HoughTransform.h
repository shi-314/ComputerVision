// ========================================================== //
// HoughTransform
// - Führt die Hough Tranformation auf ein Bitmap aus
//
// Shivan Taher <shi.taher@gmail.com>
// Created: 15.05.2011
// ========================================================== //

#ifndef HOUGH_TRANSFORM_H
#define HOUGH_TRANSFORM_H

#include "ComputerVision.h"

namespace ComputerVision {
	using namespace std;

	class HoughTransform {
		public:
			HoughTransform(Bitmap* img);
			~HoughTransform();

			// Berechnet das Hough-Raum für das ganze Bild
			void calculate();

			// Wandelt eine Hough-Gerade (t,r) in ein Line-Objekt (a,b) um
			Line houghToLine(Vec2 houghLine);

			// Analysiert den Hough-Raum um die lokalen Extrema zu bestimmen und gibt eine Liste von Geraden zurück
			// Je höher der Threshold umso mehr Linien, die nicht genau im Bild zu erkennen waren werden ignoriert
			// Hough-Geraden werden durch ein 2d Vektor dargestellt mit V(t,r)
			vector<Vec2> getHoughLines(int threshold = 127);

			// Durchsucht das Bild nach dem größten Segment einer Geraden (in Hesse'scher Normalform)
			vector<Line> getLineSegments(Vec2 houghLine, bool removeHits=true);

			// Progressive Probabilistic Hough Transform
			vector<Line> ppht();

			// Gibt den Punkt im Hough-Raum mit dem höchsten Wert zurück (t,r,wert)
			Vec3 getHighestValue();

			// Speichert den Hough-Raum als Bilddatei (zum debuggen usw)
			void saveHoughImage(string filename);

		private:
			// Inkrementiert pos im Hough-Raum
			void vote(Vec2 pos);
			// Dekrementiert pos im Hough-Raum
			void unvote(Vec2 pos);

			// Winkelbereich, der berechnet werden soll
			int maxTheta;
			double thetaStep;
			double* sinCache;
			double* cosCache;

			// Bitmap
			Bitmap* img;
			Vec2 imgSize;
			Vec2 imgCenter;

			// Hough Raum (erstmal als Bitmap)
			//Bitmap* houghSpace;
			Matrix2<int>* houghSpace;
			Vec2 houghSize;
			int doubleHeight;
			int numPoints;
	};
}

#endif
