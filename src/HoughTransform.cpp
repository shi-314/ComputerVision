// ========================================================== //
// HoughTransform
// - Führt die Hough Tranformation auf ein Bitmap aus
//
// Shivan Taher <shi-89@gmx.net>
// Created: 15.05.2011
// ========================================================== //

#include "ComputerVision.h"

namespace ComputerVision {
	using namespace std;
	
	// ========================================================== //
	// HoughTransform
	// ========================================================== //
	HoughTransform::HoughTransform(Bitmap* img) {
		if(img->getBPP() != BITMAP_8)
			throw exception("HoughTransform::HoughTransform -- This method works only with 8 bit grayscale bitmaps");

		// Parameter initialisieren
		this->numPoints = 0;

		this->maxTheta = 180;
		this->thetaStep = Math::pi/this->maxTheta;
	
		// Parameter für das Bild initialisieren
		this->img = img;
		this->imgSize = img->getDimensions();
		this->imgCenter = this->imgSize/2;

		// Parameter für den Hough-Raum initialisieren
		this->houghSize.x = this->maxTheta;
		this->houghSize.y = (int)(sqrt(2.0f) * max(this->imgSize.x, this->imgSize.y) / 2);
		this->doubleHeight = houghSize.y * 2;

		// Hough-Raum erstellen
		//this->houghSpace = new Bitmap(Vec2(this->houghSize.x, this->doubleHeight), ColorDepth::BITMAP_8, Color(0,0,0));
		unsigned int houghLen = this->houghSize.x*this->doubleHeight;
		this->houghSpace = new Matrix2<int>(houghSize.x, doubleHeight);
		//memset(this->houghSpace, 0, houghLen*sizeof(int));

		// Cache vorbereiten
		this->sinCache = new double[this->maxTheta];
		this->cosCache = new double[this->maxTheta];
		double realTheta;
		for(int t=0; t<this->maxTheta; t++) {
			realTheta = t * this->thetaStep;
			this->sinCache[t] = sin(realTheta);
			this->cosCache[t] = cos(realTheta);
		}
	}

	HoughTransform::~HoughTransform() {
		delete houghSpace;
		delete[] this->sinCache;
		delete[] this->cosCache;
	}

	// ========================================================== //
	// calculate
	// ========================================================== //
	void HoughTransform::calculate() {
		// Alle Pixel durchlaufen...
		Vec2 pos;
		for(pos.y=0; pos.y<imgSize.y; pos.y++) {
			for(pos.x=0; pos.x<imgSize.x; pos.x++) {
				unsigned char w = this->img->getPixel(pos).b;
				if(w > 127) { // ACHTUNG: VORÜBERGEHEND !!!
					this->vote(pos);
				}
			}
		}
	}

	// ========================================================== //
	// vote
	// ========================================================== //
	void HoughTransform::vote(Vec2 pos) {
		// Alle Winkel durchgehen
		for(int t=0; t<this->maxTheta; t++) {
			// r berechnen (Abstand zum Ursprung)
			int r = (int) (((pos.x - this->imgCenter.x) * this->cosCache[t]) + ((pos.y - this->imgCenter.y) * this->sinCache[t])); 

			// Um negative Werte zu vermeiden:
			r += this->houghSize.y;

			if(r<0 || r>=this->doubleHeight) continue;
			(*this->houghSpace)[t][r]++;
			//this->houghSpace[t+r*houghSize.x]++;
		}

		this->numPoints++;
	}

	// ========================================================== //
	// unvote
	// ========================================================== //
	void HoughTransform::unvote(Vec2 pos) {
		// Alle Winkel durchgehen
		for(int t=0; t<this->maxTheta; t++) {
			// r berechnen (Abstand zum Ursprung)
			int r = (int) (((pos.x - this->imgCenter.x) * this->cosCache[t]) + ((pos.y - this->imgCenter.y) * this->sinCache[t])); 

			// Um negative Werte zu vermeiden:
			r += this->houghSize.y;

			if(r<0 || r>=this->doubleHeight) continue;
			(*this->houghSpace)[t][r]--;
			//this->houghSpace[t+r*houghSize.x]--;
		}

		//this->numPoints--;
	}

	// ========================================================== //
	// houghToLine
	// ========================================================== //
	Line HoughTransform::houghToLine(Vec2 houghLine) {
		// Resultat
		Line l;
		// Berechnung des echten Theta
		double theta = houghLine.x * thetaStep;

		// Anfang- und Endpunkte der Geraden auf dem Bild berechnen
		// Unterscheidung, weil vertikale Geraden einen unendlich hohen Koeffizienten haben
		int x1,x2,y1,y2;
		Vec2 p1, p2;
		if (theta < Math::pi * 0.25 || theta > Math::pi * 0.75) {
			// "vertikale" Geraden
			// ersten Punkt (im Bild) finden (unten)
			for(y1=0; y1<this->imgSize.y; y1++) {
				x1 = (int) ((((houghLine.y - this->houghSize.y) - ((y1-this->imgCenter.y) * this->sinCache[houghLine.x])) / this->cosCache[houghLine.x]) + this->imgCenter.x);
				if(x1 >= 0 && x1 < this->imgSize.x)
					break;
			}
			l.a = Vec2(x1, y1);

			// ersten Punkt (im Bild) finden (oben)
			for(y2=this->imgSize.y-1; y2>0; y2--) {
				x2 = (int) ((((houghLine.y - this->houghSize.y) - ((y2-this->imgCenter.y) * this->sinCache[houghLine.x])) / this->cosCache[houghLine.x]) + this->imgCenter.x);
				if(x2 >= 0 && x2 < this->imgSize.x)
					break;
			}
			l.b = Vec2(x2,y2);
		} else {
			// "horizontale" Geraden
			// ersten Punkt (im Bild) finden (links)
			for(x1=0; x1<this->imgSize.x; x1++) {
				y1 = (int) ((((houghLine.y - this->houghSize.y) - ((x1-this->imgCenter.x) * this->cosCache[houghLine.x])) / this->sinCache[houghLine.x]) + this->imgCenter.y);
				if(y1 >= 0 && y1 < this->imgSize.y)
					break;
			}
			l.a = Vec2(x1, y1);

			// zweiten Punkt (im Bild) finden (rechts)
			for(x2 = this->imgSize.x-1; x2>0; x2--) {
				y2 = (int) ((((houghLine.y - this->houghSize.y) - ((x2 - this->imgCenter.x) * this->cosCache[houghLine.x])) / this->sinCache[houghLine.x]) + this->imgCenter.y);
				if(y2 >= 0 && y2 < this->imgSize.y)
					break;
			}
			l.b = Vec2(x2, y2);
		}

		return l;
	}

	// ========================================================== //
	// getines
	// ========================================================== //
	vector<Vec2> HoughTransform::getHoughLines(int threshold) {
		if(this->numPoints < 1)
			throw exception("HoughTransform::getLines -- The hough space is emtpy. Calculate hough space first.");
		
		// Anzahl der Nachbar-Pixel die berücksichtigt werden sollen
		const int neighbourhoodSize = 4;
		vector<Vec2> lines;

		// Alle Winkel durchgehen
		for(int t=0; t<this->maxTheta; t++) {
			for (int r = neighbourhoodSize; r < this->doubleHeight - neighbourhoodSize; r++) {
                
				// Nur Werte über dem Schwellwert berücksichtigen
				//int peak = this->houghSpace->getPixel(Vec2(t,r)).b;
				int peak = (*this->houghSpace)[t][r];
				if(peak > threshold) {
					bool localMaximum = true;

                    // Umgebung des lokalen Maximums überprüfen
                    for (int dx = -neighbourhoodSize; dx <= neighbourhoodSize; dx++) {
                        for (int dy = -neighbourhoodSize; dy <= neighbourhoodSize; dy++) {
                            int dt = t + dx;
                            int dr = r + dy;
                            if (dt < 0) dt = dt + maxTheta;
                            else if (dt >= maxTheta) dt = dt - maxTheta;
                            
							// In der Umgebung wurde ein höheres lokales Maximum gefunden
							if((*this->houghSpace)[dt][dr] > peak) {
								localMaximum = false;
								continue;
                            }
                        }
                    }

					if(localMaximum) {
						// Berechnung des echten Theta
						//double theta = t * thetaStep;
						//cout<<"Line: t="<<t<<", r="<<r<<endl;

						///lines.push_back(this->houghToLine(Vec2(t,r)));
						lines.push_back(Vec2(t,r));
						//cout<<"Line: A("<<x1<<"|"<<y1<<"), B("<<x2<<"|"<<y2<<")"<<endl;
					}
				} // if peak
			} // for r
		} // for t

		return lines;
	}

	// ========================================================== //
	// ppht
	// ========================================================== //
	vector<Line> HoughTransform::ppht() {
		//Bitmap* img1 = this->img;
		Bitmap img2;
		img2 = this->img->copy();
		//this->img = &img2;

		const int threshold = 100;
		const int minLineLength = 20;

		unsigned char* img2Buffer = img2.getBuffer();
		unsigned long img2Len = img2.getSize();

		vector<Line> lines;

	step1:
		// Ist img2 leer (=komplett schwarz) ?
		bool img2Empty = true;
		for(unsigned long n=0; n<img2Len; n++) {
			if(img2Buffer[n] > 127) {
				img2Empty = false;
				break;
			}
		}

		if(img2Empty) {
			cout<<"finish"<<endl;
			return lines;
		}

		// Zufälligen Punkt aussuchen, der weiß ist
		unsigned char w = 0;
		Vec2 randPos;
		while(w<127) {
			randPos = Vec2(Math::randomInt(0, img2.getWidth()-1), Math::randomInt(0, img2.getHeight()-1));
			w = img2.getPixel(randPos).b;
		}

		// Voten
		this->vote(randPos);
		
		// Pixel aus img2 entfernen
		img2.setPixel(randPos, Color(0,0,0));

		// Höchsten Wert im Hough-Raum suchen und mit Threshold vergleichen
		//cout<<"points: "<<this->numPoints<<endl;
		Vec3 highest = this->getHighestValue();
		if(highest.z < threshold) {
			goto step1;
		}
		
		vector<Line> segs = this->getLineSegments(Vec2(highest.x, highest.y));
		for(int n=0; n<segs.size(); n++)
			lines.push_back(segs[n]);

		//this->houghSpace[highest.x+highest.y*this->houghSize.x] = 0;
		/*highest.x = thetaStep * highest.x;
		for(int x1=0; x1<this->imgSize.x; x1++) {
			int y1 = (int) ((((highest.y - this->houghSize.y) - ((x1-this->imgCenter.x) * this->cosCache[highest.x])) / this->sinCache[highest.x]) + this->imgCenter.y);
			if(y1 >= 0 && y1 < this->imgSize.y)
				break;
			this->unvote(Vec2(x1,y1));
		}*/

		goto step1;

		// Das längste Segment von Pixeln finden, die auf der Geraden sind

		//img2.save("test.bmp");
		return lines;
	}

	// ========================================================== //
	// getLineSegments
	// ========================================================== //
	vector<Line> HoughTransform::getLineSegments(Vec2 houghLine, bool removeHits/*=true*/) {
		Line line = this->houghToLine(houghLine);
		
		int t = houghLine.x;
		double r = houghLine.y;
		double ry = r - this->houghSize.y;
		double theta = t * this->thetaStep;
		
		// first: Gibt an ob schon der Startpunkt des Liniensegments gefunden wurde
		bool first = true;
		Vec2 pos1, pos2;
		
		// Maximale Anzahl erlaubter Lücken
		const int maxGaps = 6;
		// Anzahl der Lücken
		int gaps = 0;
		// Anzahl der Treffer (=weiße Pixel)
		int numHits = 0;

		// Start- und Endpunkte des Liniensegments
		Vec2 a,b;

		// Menge der gefundenen Liniensegmente
		vector<Line> lineSegments;

		// Schwellwert für die Mindestlänge der Liniensegmente
		const int minLength = 10;

		// Gerade verläuft (eher) horizontal
		bool horizontal = !(theta < Math::pi * 0.25 || theta > Math::pi * 0.75);
		int x, y, tmpX, tmpY, xMax;
		if(horizontal) {
			xMax = this->imgSize.x - 1;
		}else{
			xMax = this->imgSize.y - 1;
		}
		//cout<<"horizontal: "<<horizontal<<endl;
		// (1) Alle Punkte der Geraden auf dem Bild durchgehen
		for(int x=1; x < xMax; x++) {
			numHits = 0;

			// horizontale Gerade
			if(horizontal) {
				y = (int) (((ry - ((x-this->imgCenter.x) * this->cosCache[t])) / this->sinCache[t]) + this->imgCenter.y);
				// Punkte, die nicht im Bild liegen, überspringen
				if(y < 1 || y >= this->imgSize.y-1)
					continue;
			} else {
				tmpX = x;
				y = x;
				x = (int) (((ry - ((x-this->imgCenter.y) * this->sinCache[t])) / this->cosCache[t]) + this->imgCenter.x);
				// Punkte, die nicht im Bild liegen, überspringen
				if(x < 1 || x >= this->imgSize.x-1) {
					x = tmpX;
					continue;
				}
			}

			pos1 = Vec2(x,y);
			
			bool hit1 = false;

			// Treffer
			hit1 = this->img->getPixel(pos1).b > 50;
				
			// (2) Startpunkt des Liniensegments finden
			if(hit1 && first) {
				a.x = x;
				a.y = y;
				//cout<<"Line start: "<<x<<", "<<y<<endl;
			}

			// (3) Nachbarpixel überprüfen
			if(hit1 || !first) {
				first = false;

				if(hit1) {
					numHits++;
					if(removeHits) {
						this->img->setPixel(pos1, Color(60));
						this->unvote(pos1);
					}
				}

				// (3) Nachbarpixel durchgehen
				for(int i=-1; i<2; i++) {
					for(int j=-1; j<2; j++) {
						// Den aktuellen Pixel (Mitte der Nachbaren) auslassen
						if(i == 0 && j == 0)
							continue;

						// Position des Nachbarpixels
						pos2 = pos1+Vec2(i,j);
						
						// Treffer beim Nachbarpixel
						if(this->img->getPixel(pos2).b > 50) {
							gaps = 0;
							numHits++;
							b = pos2;

							if(removeHits) {
								this->img->setPixel(pos2, Color(60));
								this->unvote(pos2);
							}
						}
					} // for j
				} // for i
			} // Ende der Nachbarpixelüberprüfung

			// (4) Lücken Zählen
			if(numHits == 0 && !first) {
				gaps++;
			}

			// (5) Liniensegment beenden
			// Liniensegment ist zu Ende, wenn der Schwellwert der Lücken überschritten wurde
			if(gaps >= maxGaps) {
				if(!first && a.distance(b) > minLength) {
					lineSegments.push_back(Line(a,b));
					//cout<<"Line end: "<<b.x<<", "<<b.y<<endl;
				}
				first = true;
			}

			if(!horizontal) {
				x = tmpX;
			}
		} // for x

		// (5) Liniensegment beenden
		// Letztes Liniensegment ist zu Ende, wenn die Schleife fertig ist
		if(!first && a.distance(b) > minLength) {
			lineSegments.push_back(Line(a,b));
			//cout<<"Line end: "<<b.x<<", "<<b.y<<endl;
		}

		//cout<<"Result: "<<lineSegments.size()<<" segments"<<endl;
		//cout<<"gaps: "<<gaps<<endl;
		// (6) Rückgabe der Resultate
		return lineSegments;
	}

	// ========================================================== //
	// saveHoughImage
	// ========================================================== //
	void HoughTransform::saveHoughImage(string filename) {
		Bitmap houghImg(Vec2(this->houghSize.x, this->doubleHeight), BITMAP_8);
		
		Vec2 pos;
		int v;
		unsigned char w;
		
		for(pos.y=0; pos.y<houghImg.getHeight(); pos.y++) {
			for(pos.x=0; pos.x<houghImg.getWidth(); pos.x++) {
				v = (*this->houghSpace)[pos.x][pos.y];
				if(v > 255)
					w = 255;
				else if(v < 0)
					w = 0;
				else
					w = v;
				houghImg.setPixel(pos, Color(w));
			}
		}

		houghImg.save(filename);
	}

	// ========================================================== //
	// getHighestValue
	// ========================================================== //
	Vec3 HoughTransform::getHighestValue() {
		if(this->numPoints < 1)
			throw exception("HoughTransform::getHighestValue -- The hough space is emtpy. Calculate hough space first.");

		int highestValue = 0;
		int currentValue = 0;
		Vec3 result;

		// Den gesamten Hough-Raum durchsuchen
		for(int r=0; r<this->doubleHeight; r++) {
			for(int t=0; t<this->houghSize.x; t++) {
				currentValue = (*this->houghSpace)[t][r];
				if(currentValue>highestValue) {
					highestValue = currentValue;
					result = Vec3(t,r,highestValue);
				}
			}
		}

		return result;
	}
}