// ========================================================== //
// Bitmap
// - Laden, Speichern, Bearbeiten von Bilddateien
// - WICHTIG: Unterst�tzt nur 8 und 24 bpp Bitmaps (sollte reichen)
//
// Shivan Taher <shi-89@gmx.net>
// Created: 05.02.2011
// ========================================================== //

#ifndef _BITMAP_H
#define _BITMAP_H

#include "ComputerVision.h"

namespace ComputerVision {

	using namespace std;

	// Unterst�tzt nur 8 und 24 Bit - andere Werte sind f�r dieses Projekt irrelevant
	enum ColorDepth {
		BITMAP_8 = 8,
		BITMAP_24 = 24
	};

	enum InterpolationMethod {
		IM_NEAREST_NEIGHBOR = 0,
		IM_BILINEAR = 1
	};

	class Bitmap {
		public:
			Bitmap();
			Bitmap(string filename);
			Bitmap(Vec2 dim, ColorDepth bpp = BITMAP_24, Color bgcolor = Color(255,255,255));
			~Bitmap();

			// L�dt eine Bitmap-Datei
			void load(string filename);

			// Erstellt das Bild (8,16,24 Bit Farbtiefe)
			void create(Vec2 dimensions, ColorDepth bpp = BITMAP_24);

			// Speichert die Daten als Bitmap-Datei
			void save(string filename);

			// Konvertiert das Bild in ein anderes Farbformat (�ndert die Farbtiefe)
			// Verwendet Grauwerte f�r die Farbpalette, falls von 24bit in 8bit konvertiert wird.
			// Die Kanalgewichtung ist ausgeglichen (1,1,1)
			// - Bei Bedarf erst das Bild mit beliebiger Kanalgewichtung in ein Graustufenbild umwandeln
			void convert(ColorDepth bpp);

			// Skaliert das Bild mit billinearer Interpolation
			void scale(Vec2 size, InterpolationMethod im = IM_BILINEAR);

			// Kopiert einen Bildauschnitt und gibt das neue Bitmap-Objekt zur�ck
			Bitmap copy(Vec2 from, Vec2 to);
			// Kopiert das ganze Objekt und gibt es zur�ck
			Bitmap copy();

			// F�gt ein Bitmap in dieses (an der Stelle pos) ein
			void paste(Bitmap* bmp, Vec2 pos);

			void operator =(Bitmap& bmp);

			// Gibt Breite und H�he in einem Vektor zur�ck
			inline Vec2 getDimensions() { return Vec2((int)this->bitmap.bmWidth, (int)this->bitmap.bmHeight); }

			// Gibt die Breite in Pixel zur�ck
			inline int getWidth() { return (int)this->bitmap.bmWidth; }

			// Gibt die H�he in Pixel zur�ck
			inline int getHeight() { return (int)this->bitmap.bmHeight; }

			// Gibt Breite*H�he*BitsPerPixel/8 zur�ck (also die Gr��e der Daten ohne Header in Bytes)
			inline unsigned long getSize() { return this->bitmapInfo->bmiHeader.biSizeImage; }

			// Gibt die Farbtiefe BPP (BitsPerPixel) zur�ck
			inline ColorDepth getBPP() { return (ColorDepth)this->bitmap.bmBitsPixel; };

			// �berpr�ft, ob der Punkt p im Bild liegt
			inline bool isPixelInRange(Vec2 p) {return !(p.x >= this->getWidth() || p.x < 0 || p.y >= this->getHeight() || p.y < 0);}

			// Gibt die L�nge einer Zeile in Bytes zur�ck
			inline unsigned int getLineBytes() {return this->lineWidthBytes;}

			// Setzt Pixel v auf Farbe pix
			void setPixel(Vec2 v, Color pix);

			// Setzt das letzte Pixel mit getPixel auf pix
			void setPixel(Color col);

			// Holt Pixelfarbe an der Stelle v
			Color getPixel(Vec2 v);

			// Gibt die Position auf dem Buffer vom letzten Pixelzugriff zur�ck
			inline unsigned int getPosOnBuffer() {return this->nPosOnBuffer;}

			// Gibt die Pixeldaten als Array zur�ck, Zeilen sind verkehrt herum gespeichert
			inline unsigned char* getBuffer() {return this->data;}
			
			// �ndert die Speicheradresse vom Buffer auf <buffer> und l�scht den alten Buffer
			// - WARNUNG: <buffer> muss genau die Gr��e getSize() haben
			//			  - �ndert nur den Pointer, kopiert den Inhalt nicht!
			inline void setBuffer(unsigned char* buffer) {delete[] this->data; this->data = buffer;}

			// F�llt das gesamte Bild mit der gegebenen Farbe
			void fill(Color color);

			// Zeichnet eine Linie auf das Bild von v1 nach v2
			void drawLine(Vec2 from, Vec2 to, Color color);

			// Zeichnet ein Rechteck Von v1 (links oben) bis v2 (rechts unten)
			void drawRect(Vec2 v1, Vec2 v2, Color color);

			// Zeichnet einen Kreis mit m als Mittelpunkt und r als Radius
			void drawCircle(Vec2 m, int r, Color color);
			
			// Farbpalette
			ColorPalette* colorPalette;

		private:
			// Gibt den Speicher wieder frei, der vom Bitmap eingenommen wurde (Ausnahme: BITMAPINFOHEADER wegen Performance)
			void releaseBitmap();

			// Setzt Werte von privaten Attributen zur�ck
			//void reset();

			// Initialisiert Objekte
			void init();

			HBITMAP bitmapHandle;
			BITMAP bitmap;
			BITMAPINFO* bitmapInfo;

			unsigned char bytesPerPixel;
			unsigned char* data;
			int lineWidthBytes;		// Zeilenl�nge in Bytes
			unsigned long nPosOnBuffer; // Pixelposition im Buffer
			Vec2 bufferDim; // Buffer-Dimensionen = Bilddimensionen - 1 (Performance)
	};
}

#endif