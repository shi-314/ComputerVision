// ========================================================== //
// Line
// - Repräsentiert eine Line durch zwei Punkte
//
// Shivan Taher <shi-89@gmx.net>
// Created: 15.05.2011
// ========================================================== //

#include "ComputerVision.h"

namespace ComputerVision {
	using namespace std;

	// ========================================================== //
	// HoughLine
	// ========================================================== //
	Line::Line(Vec2 a, Vec2 b) {
		this->a = a;
		this->b = b;
	}

	Line::~Line() {
	
	}

	// ========================================================== //
	// draw
	// ========================================================== //
	void Line::draw(Bitmap* img, Color color) {
		img->drawLine(this->a, this->b, color);
	}

	// ========================================================== //
	// drawToVektor
	// ========================================================== //
	vector<Vec2> Line::drawToVektor() {
		vector<Vec2> points;
		Vec2 from=a, to=b;

		// Bresenhams Linienalgorithmus
		int lg_delta, sh_delta, cycle, lg_step, sh_step;

		lg_delta = to.x - from.x;
		sh_delta = to.y - from.y;
		lg_step = Math::signum(lg_delta);
		lg_delta = abs(lg_delta);
		sh_step = Math::signum(sh_delta);
		sh_delta = abs(sh_delta);
		if (sh_delta < lg_delta) {
			cycle = lg_delta >> 1;
			while (from.x != to.x) {
				points.push_back(from);
				cycle += sh_delta;
				if (cycle > lg_delta) {
					cycle -= lg_delta;
					from.y += sh_step;
				}
				from.x += lg_step;
			}
			points.push_back(from);
		}
		cycle = sh_delta >> 1;
		while (from.y != to.y) {
			points.push_back(from);
			cycle += lg_delta;
			if (cycle > sh_delta) {
				cycle -= sh_delta;
				from.x += lg_step;
			}
			from.y += sh_step;
		}
		points.push_back(from);

		return points;
	}
}