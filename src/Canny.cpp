// ========================================================== //
// Canny
// - Canny-Algorithmus
//
// Shivan Taher <shi.taher@gmail.com>
// Created: 18.05.2011
// ========================================================== //

#include <cv/ComputerVision.h>

namespace ComputerVision {
	using namespace std;

	// ========================================================== //
	// Canny
	// ========================================================== //
	Canny::Canny(Bitmap* src) {
		if(src->getBPP() != BITMAP_8)
			throw exception("Canny::Canny -- This method works only with 8 bit grayscale bitmaps");

		this->src = src;
		this->width = src->getWidth();
		this->height = src->getHeight();

		this->theta = new Matrix2<float>(width, height);
		this->gradP = new Matrix2<char>(width, height);
		this->gradQ = new Matrix2<char>(width, height);
		this->gradM = new Matrix2<char>(width, height);
		this->delta = new Matrix2<char>(width, height);
		this->nms   = new Matrix2<char>(width, height);

		this->marked   = new Matrix2<bool>(width, height);
		this->tracked  = new Matrix2<char>(width, height);
	}

	Canny::~Canny() {
		delete this->theta;
		delete this->gradP;
		delete this->gradQ;
		delete this->gradM;
		delete this->delta;
		delete this->nms;
		delete this->marked;
		delete this->tracked;
	}

	// ========================================================== //
	// apply
	// ========================================================== //
	void Canny::apply(int lowThresh, int highThresh) {
		// Thx to: http://homepages.inf.ed.ac.uk/rbf/HIPR2/flatjavasrc/Canny.java

		this->lowThresh = lowThresh;
		this->highThresh = highThresh;

		// (1) Gauss-Filter anwenden
		Filter::gaussianBlur(src);

		unsigned char* buffer = src->getBuffer();
		unsigned int nPosOnBuffer = 0;
		
		// (2) Gradienten mithilfe des Sobel-Operators ermitteln
		unsigned char v1, v2, v3, v4;
		for(int y=0; y<height-1; y++) {
			for(int x=0; x<width-1; x++) {
				// x, y
				v1 = src->getPixel(Vec2(x,y)).b;
				nPosOnBuffer = src->getPosOnBuffer();
				// x+1, y
				v2 = buffer[nPosOnBuffer+1];
				nPosOnBuffer -= src->getLineBytes();
				// x, y+1
				v3 = buffer[nPosOnBuffer];
				// x+1, y+1
				v4 = buffer[nPosOnBuffer+1];

				// ACHTUNG: DAS IST NICHT DER SOBEL OPERATOR (ZUMINDEST NICHT MIT EINER 3X3 MATRIX !!!!!!!!!!!!!!!)
				// Sobeloperator in X-Richtung
				(*this->gradP)[x][y] = (char)((v1-v1+v4-v2)/4);
				// Sobeloperator in Y-Richtung
				(*this->gradQ)[x][y] = (char)((v1-v2+v3-v4)/4);
				
				// Absolute Kantenstärke
				//(*this->gradM)[x][y] = (char)sqrtf(pow((float)(*this->gradP)[x][y],2) + pow((float)(*this->gradQ)[x][y], 2));
				// etwas schneller als die Formel oben
				(*this->gradM)[x][y] = (char)(abs((int)(*this->gradP)[x][y]) + abs((int)(*this->gradQ)[x][y]));
				
				// Kombination aus den beiden Richtungen des Sobeloperators 
				(*this->theta)[x][y] = atan2((float)(*this->gradQ)[x][y], (float)(*this->gradP)[x][y]);
			}
		}
		
		this->width--;
		this->height--;

		// (3) Bild der absoluten Kantenstärken berechnen
		// Anstieg der potentiellen Kanten berechnen
		double t;
		
		for(int y=1; y<height; y++) {
			for(int x=1; x<width; x++) {
				// Der Winkel soll zwischen 0° und 360° liegen
				t = radToDeg((*this->theta)[x][y]);
				t += 270;
				t = fmod(t,360);

				if((t >= 337.5) || (t < 22.5) || ((t >= 157.5) && (t < 202.5)))
					(*this->delta)[x][y] = 0;
				else  if(((t >= 22.5) && (t < 67.5)) || ((t >=202.5) && (t < 247.5)))
					(*this->delta)[x][y] = 1;
				else if(((t >= 67.5) && (t < 112.5)) || ((t >=247.5) && (t < 292.5)))
					(*this->delta)[x][y] = 2;
				else if(((t >= 112.5) && (t < 157.5)) || ((t >= 292.5) && (t < 337.5)))
					(*this->delta)[x][y] = 3;
				else
					(*this->delta)[x][y] = 0;
			}
		}

		// (4) Nonmaxima Suppression
		for(int y=1; y<height-1; y++) {
			for(int x=1; x<width-1; x++) {
				(*this->nms)[x][y] = suppress(x, y, (*this->delta)[x][y]);
			}
		}

		width -= 2;
		height -= 2;

		// (5) Hysterese
		this->applyTrack();
		
		// (6) Ausgabe berechnen
		int result;
		
		src->fill(Color(0));
		for(int y=0; y<height; y++) {
			for(int x=0; x<width; x++) {
				result = (int)((*this->tracked)[x][y]);
				if(result > 0)
					result = 255;
				else
					result = 0;
				src->setPixel(Vec2(x,y), Color((unsigned char)result));
			}
		}
	}

	// ========================================================== //
	// suppress
	// ========================================================== //
	char Canny::suppress(int x, int y, char sector) {
		int tmp = (*this->gradM)[x][y];
			
		if (tmp < lowThresh) return 0;

		if(sector == 0) {
			if((*this->gradM)[x+1][y] >= tmp || (*this->gradM)[x-1][y] > tmp)
				return 0;
			else
				return tmp;
		}
			
		if(sector == 1) {
			if((*this->gradM)[x+1][y+1] >= tmp || ((*this->gradM))[x-1][y-1] > tmp)
				return 0;
			else
				return tmp;
		}
			
		if(sector == 2) {
			if((*this->gradM)[x][y+1] >= tmp || (*this->gradM)[x][y-1] > tmp)
				return 0;
			else
				return tmp;
		}

		if(sector == 3){
			if((*this->gradM)[x+1][y-1] >= tmp || (*this->gradM)[x-1][y+1] > tmp)
				return 0;
			else
				return tmp;
		}

		return 0;
	}

	// ========================================================== //
	// applyTrack
	// ========================================================== //
	void Canny::applyTrack() {
   		stack<Vec2> toTrack;
			
		// Startpunkte zum Tracking suchen
		for(int y=0; y < height; y++){
			for(int x=0; x < width; x++){
				// Wenn der Punkt noch nicht markiert wurde und über dem oberen Schwellwert liegt, dann verfolgen
				if(((*this->nms)[x][y] > this->highThresh) && ((*this->marked)[x][y] == 0)) {
					this->track(&toTrack, x, y);
				}
			}
		}
		
		// Alle Punkte im Input löschen, die markiert sind
		for(int y = 0; y < height; y++){
			for(int x = 0; x < width; x++){
				if((*this->marked)[x][y] == 0) {
					(*this->tracked)[x][y] = 0;
				} else {
					(*this->tracked)[x][y] = (*this->nms)[x][y];
				}
			}
		}
	}

	// ========================================================== //
	// track
	// ========================================================== //
	void Canny::track(stack<Vec2>* toTrack, int x, int y) {
		int a;
		int b;
			
		// Punkt, von wo aus das Tracking starten soll
		Vec2 current(x,y);
		toTrack->push(current);

		while(!toTrack->empty()){
			// Den ersten Punkt vom Stack nehmen
			current = toTrack->top();
			toTrack->pop();
				
			a = current.x;
			b = current.y;
				
			// Nachbarn überprüfen und zum Stack hinzufügen, wenn sie über dem oberen Schwellwert sind
			// Nur wenn die Nachbarn noch nicht markiert sind
			if((*marked)[a][b] == 0){
				// Alle Nachbaren überprüfen und verfolgen
				if(a > 0 && b > 0) {
					if((*this->nms)[a-1][b-1] > this->lowThresh) {
						current = Vec2(a-1, b-1);
						toTrack->push(current);
					}
				}
	  
				if(b > 0) {
					if((*this->nms)[a][b-1] > this->lowThresh){
						current = Vec2(a, b-1);
						toTrack->push(current);
					}
				}
	  
				if(a < width-1 && b > 0) {
					if((*this->nms)[a+1][b-1] > this->lowThresh) {
						current = Vec2(a+1, b-1);
						toTrack->push(current);
					}
				}
	  
				if(a > 0) {
					if((*this->nms)[a-1][b] > this->lowThresh) {
						current = Vec2(a-1, b);
						toTrack->push(current);
					}
				}
	  
				if(a < width-1) {
					if((*this->nms)[a+1][b] > this->lowThresh) {
						current = Vec2(a+1, b);
						toTrack->push(current);
					}
				}
	  
				if(a > 0 && b < height-1) {
					if((*this->nms)[a-1][b+1] > this->lowThresh) {
						current = Vec2(a-1, b+1);
						toTrack->push(current);
					}
				}
	  
				if(b < height-1) {
					if((*this->nms)[a][b+1] > this->lowThresh) {
						current = Vec2(a, b+1);
						toTrack->push(current);
					}
				}
	  
				if(a < width-1 && b < height-1) {
					if((*this->nms)[a+1][b+1] > this->lowThresh) {
						current = Vec2(a+1, b+1);
						toTrack->push(current);
					}
				}
	  
				// Diesen Pixel markieren, weil er schon verfolgt wurde
				(*marked)[a][b] = 1;
			} // if marked
		} // while
	}
}