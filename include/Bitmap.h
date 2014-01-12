// ========================================================== //
// Bitmap
// - Laden, Speichern, Bearbeiten von Bilddateien
// - WICHTIG: Unterstützt nur 8 und 24 bpp Bitmaps (sollte reichen)
//
// Shivan Taher <shi-89@gmx.net>
// Created: 05.02.2011
// ========================================================== //

#ifndef _BITMAP_H
#define _BITMAP_H

#include "ComputerVision.h"

namespace ComputerVision {

	using namespace std;

	// Unterstützt nur 8 und 24 Bit - andere Werte sind für dieses Projekt irrelevant
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

			// Lädt eine Bitmap-Datei
			void load(string filename);

			// Erstellt das Bild (8,16,24 Bit Farbtiefe)
			void create(Vec2 dimensions, ColorDepth bpp = BITMAP_24);

			// Speichert die Daten als Bitmap-Datei
			void save(string filename);

			// Konvertiert das Bild in ein anderes Farbformat (ändert die Farbtiefe)
			// Verwendet Grauwerte für die Farbpalette, falls von 24bit in 8bit konvertiert wird.
			// Die Kanalgewichtung ist ausgeglichen (1,1,1)
			// - Bei Bedarf erst das Bild mit beliebiger Kanalgewichtung in ein Graustufenbild umwandeln
			void convert(ColorDepth bpp);

			// Skaliert das Bild mit billinearer Interpolation
			void scale(Vec2 size, InterpolationMethod im = IM_BILINEAR);

			// Kopiert einen Bildauschnitt und gibt das neue Bitmap-Objekt zurück
			Bitmap copy(Vec2 from, Vec2 to);
			// Kopiert das ganze Objekt und gibt es zurück
			Bitmap copy();

			// Fügt ein Bitmap in dieses (an der Stelle pos) ein
			void paste(Bitmap* bmp, Vec2 pos);

			void operator =(Bitmap& bmp);

			// Gibt Breite und Höhe in einem Vektor zurück
			inline Vec2 getDimensions() { return Vec2((int)this->bitmap.bmWidth, (int)this->bitmap.bmHeight); }

			// Gibt die Breite in Pixel zurück
			inline int getWidth() { return (int)this->bitmap.bmWidth; }

			// Gibt die Höhe in Pixel zurück
			inline int getHeight() { return (int)this->bitmap.bmHeight; }

			// Gibt Breite*Höhe*BitsPerPixel/8 zurück (also die Größe der Daten ohne Header in Bytes)
			inline unsigned long getSize() { return this->bitmapInfo->bmiHeader.biSizeImage; }

			// Gibt die Farbtiefe BPP (BitsPerPixel) zurück
			inline ColorDepth getBPP() { return (ColorDepth)this->bitmap.bmBitsPixel; };

			// Überprüft, ob der Punkt p im Bild liegt
			inline bool isPixelInRange(Vec2 p) {return !(p.x >= this->getWidth() || p.x < 0 || p.y >= this->getHeight() || p.y < 0);}

			// Gibt die Länge einer Zeile in Bytes zurück
			inline unsigned int getLineBytes() {return this->lineWidthBytes;}

			// Setzt Pixel v auf Farbe pix
			void setPixel(Vec2 v, Color pix);

			// Setzt das letzte Pixel mit getPixel auf pix
			void setPixel(Color col);

			// Holt Pixelfarbe an der Stelle v
			Color getPixel(Vec2 v);

			// Gibt die Position auf dem Buffer vom letzten Pixelzugriff zurück
			inline unsigned int getPosOnBuffer() {return this->nPosOnBuffer;}

			// Gibt die Pixeldaten als Array zurück, Zeilen sind verkehrt herum gespeichert
			inline unsigned char* getBuffer() {return this->data;}
			
			// Ändert die Speicheradresse vom Buffer auf <buffer> und löscht den alten Buffer
			// - WARNUNG: <buffer> muss genau die Größe getSize() haben
			//			  - Ändert nur den Pointer, kopiert den Inhalt nicht!
			inline void setBuffer(unsigned char* buffer) {delete[] this->data; this->data = buffer;}

			// Füllt das gesamte Bild mit der gegebenen Farbe
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

			// Setzt Werte von privaten Attributen zurück
			//void reset();

			// Initialisiert Objekte
			void init();

			HBITMAP bitmapHandle;
			BITMAP bitmap;
			BITMAPINFO* bitmapInfo;

			unsigned char bytesPerPixel;
			unsigned char* data;
			int lineWidthBytes;		// Zeilenlänge in Bytes
			unsigned long nPosOnBuffer; // Pixelposition im Buffer
			Vec2 bufferDim; // Buffer-Dimensionen = Bilddimensionen - 1 (Performance)
	};
}

#endif