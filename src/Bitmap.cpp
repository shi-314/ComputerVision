// ========================================================== //
// Bitmap
// - Laden, Speichern, Bearbeiten von Bitmap-Dateien
// - Später evtl. noch andere Formate
// > Hinweis: Code ist teilweise aus performancegründen so lang
// 
// > ACHTUNG: Viele Methode funktionieren aus performancegründen 
//			  nur mit Bitmaps mit gerader Breite!
//
// Shivan Taher <shi-89@gmx.net>
// Created: 05.02.2011
// ========================================================== //

#include "Bitmap.h"

using namespace std;

namespace ComputerVision {
	
	// ========================================================== //
	// Bitmap
	// ========================================================== //
	Bitmap::Bitmap() {
		this->init();
	}

	Bitmap::Bitmap(string filename) {
		this->init();
		this->load(filename);
	}

	Bitmap::Bitmap(Vec2 dim, ColorDepth bpp /*= BITMAP_24*/, Color bgcolor /*= Color(255,255,255)*/) {
		this->init();
		this->create(dim, bpp);
		this->fill(bgcolor);
	}

	// ========================================================== //
	// init
	// ========================================================== //
	void Bitmap::init() {
		this->data =  0;
		this->bitmapHandle = 0;
		this->lineWidthBytes = 0;
		memset(&this->bitmap, 0, sizeof(BITMAP));

		// Farbpalette mit 256 Farben instanzieren
		this->colorPalette = new ColorPalette();
		
		// BITMAPINFO erstellen mit Speicherplatz für 256 RGBQUADs
		this->bitmapInfo = (BITMAPINFO*)new unsigned char[sizeof(BITMAPINFOHEADER)+256*sizeof(RGBQUAD)];
		memset(this->bitmapInfo, 0 , sizeof(BITMAPINFOHEADER)+256*sizeof(RGBQUAD));

		this->bitmapInfo->bmiHeader.biSize = sizeof(BITMAPINFOHEADER);
	}

	// ========================================================== //
	// ~Bitmap
	// ========================================================== //
	Bitmap::~Bitmap() {
		this->releaseBitmap();
		delete this->bitmapInfo;
		delete this->colorPalette;
	}

	// ========================================================== //
	// safeRelease
	// ========================================================== //
	void Bitmap::releaseBitmap() {
		// Bitmap-Handle freigeben, falls geladen oder erstellt
		if(this->bitmapHandle != 0)
			DeleteObject(this->bitmapHandle);
		if(this->data != 0)
			delete this->data;
	}

	// ========================================================== //
	// loadBitmap
	// ========================================================== //
	void Bitmap::load(string filename) {
		// Daten resetten
		this->releaseBitmap();
		this->data =  0;
		this->bitmapHandle = 0;

		HDC hdc = GetDC(0);
		int res;

		// Datei laden
		this->bitmapHandle = (HBITMAP)LoadImageA(	0, 
													filename.c_str(),
													IMAGE_BITMAP,
													0,
													0,
													LR_DEFAULTSIZE|LR_LOADFROMFILE|LR_CREATEDIBSECTION
													);
		if(bitmapHandle == 0) {
			DWORD lastError = GetLastError();
			if(lastError == ERROR_FILE_NOT_FOUND)
				throw exception("Bitmap::loadBitmap -- File not found");
			else
				throw exception("Bitmap::loadBitmap -- (HBITMAP)LoadBitmap failed");
		}
		
		res = GetObject(this->bitmapHandle, sizeof(BITMAP), &this->bitmap);
		if(res == 0) {
			throw exception("Bitmap::loadBitmap -- GetObject failed");
		}
		
		// BITMAPINFOHEADER holen
		res = GetDIBits(hdc, this->bitmapHandle, 0, this->getHeight(), 0, this->bitmapInfo, DIB_RGB_COLORS);
		if(res == 0) {
			if(res == ERROR_INVALID_PARAMETER)
				throw exception("Bitmap::loadBitmap -- GetDIBits (reading BITMAPINFOHEADER) ERROR_INVALID_PARAMETER");
			else
				throw exception("Bitmap::loadBitmap -- GetDIBits (reading BITMAPINFOHEADER) failed");
		}

		// BITMAPINFOHEADER zwischenspeichern
		BITMAPINFOHEADER bitmapIh = this->bitmapInfo->bmiHeader;

		// Byte-Array für Pixeldaten erstellen
		this->data = new unsigned char[this->bitmapInfo->bmiHeader.biSizeImage];

		// Farbpalette und Pixeldaten holen
		res = GetDIBits(hdc, this->bitmapHandle, 0, this->getHeight(), this->data, this->bitmapInfo, DIB_RGB_COLORS);
		if(res == 0) {
			if(res == ERROR_INVALID_PARAMETER)
				throw exception("Bitmap::loadBitmap -- GetDIBits (reading RGBQUAD and data) ERROR_INVALID_PARAMETER");
			else
				throw exception("Bitmap::loadBitmap -- GetDIBits (reading RGBQUAD and data) failed");
		}

		this->bitmapInfo->bmiHeader = bitmapIh;
		
		// Größe der internen Farbpalette mit der neuen Farbpalette vergleichen
		if(this->bitmapInfo->bmiHeader.biClrUsed != 0) {
			delete this->colorPalette;
			this->colorPalette = new ColorPalette(this->bitmapInfo->bmiHeader.biClrUsed);
		}

		// Farbpalette in die Interne kopieren
		for(unsigned int i=0; i<this->bitmapInfo->bmiHeader.biClrUsed; i++) {
			Color pix;			
			pix.r = this->bitmapInfo->bmiColors[i].rgbRed;
			pix.g = this->bitmapInfo->bmiColors[i].rgbGreen;
			pix.b = this->bitmapInfo->bmiColors[i].rgbBlue;
			this->colorPalette->colors[i] = pix;
		}
	
		// DC freigeben
		DeleteDC(hdc);

		// Wegen Performance
		this->lineWidthBytes = this->getBPP() / 8 * this->getWidth();
		if(this->lineWidthBytes % 4 != 0)
			this->lineWidthBytes += (4 - this->lineWidthBytes % 4);

		this->bytesPerPixel = this->getBPP()/8;
		this->bufferDim = this->getDimensions() - Vec2(1,1);

		// Falls Breite des Bildes ungerade ist
		unsigned int nNullBytes1 = this->getWidth() % 4;
		unsigned int nNullBytes2 = this->getHeight() % 4;
		if(nNullBytes1!=0 || nNullBytes2!=0) {
			cerr<<"Bitmap::loadBitmap -- Warning: Try to use Bitmaps with even width numbers."<<endl;
		}
	}

	// ========================================================== //
	// create
	// ========================================================== //
	void Bitmap::create(Vec2 dim, ColorDepth bpp) {
		// Daten resetten
		this->releaseBitmap();
		this->data =  0;
		this->bitmapHandle = 0;
		memset(this->bitmapInfo, 0 , sizeof(BITMAPINFOHEADER)+256*sizeof(RGBQUAD));
		this->bitmapInfo->bmiHeader.biSize = sizeof(BITMAPINFOHEADER);
		
		HDC hdc = GetDC(0);
		HDC hdcMem = CreateCompatibleDC(hdc);
		int res;

		// Falls die Breite des Bildes ungerade ist
		unsigned int nNullBytes1 = dim.x % 4;
		unsigned int nNullBytes2 = dim.y % 4;
		if(nNullBytes1!=0 || nNullBytes2!=0) {
			#ifndef _HIDE_WARNINGS
				cerr<<"Bitmap::create -- Warning: Try to use Bitmaps with even width numbers."<<endl;
			#endif
			
			if(nNullBytes1!=0) dim.x += 4-nNullBytes1;
			if(nNullBytes2!=0) dim.y += 4-nNullBytes2;
			
			#ifndef _HIDE_WARNINGS
				cerr<<"Bitmap::create -- Size set to "<<dim.x<<" x "<<dim.y<<endl;
			#endif
		}

		// Zeilen mit ungerader Anzahl an Bytes müssen durch angehänge Nullen ergänzt werden
		this->data = new unsigned char[dim.x*dim.y*bpp/8];
		
		// Bitmap-Handle erstellen
		this->bitmapHandle = CreateBitmap(dim.x, dim.y, 1, bpp, this->data);
		if(bitmapHandle == 0) {
			throw exception("Bitmap::create -- CreateBitmap failed");
		}

		SelectObject(hdcMem, this->bitmapHandle);

		res = GetObject(this->bitmapHandle, sizeof(BITMAP), &this->bitmap);
		if(res == 0) {
			throw exception("Bitmap::create -- GetObject failed");
		}

		// BITMAPINFOHEADER holen
		res = GetDIBits(hdc, this->bitmapHandle, 0, 0, this->data, this->bitmapInfo, DIB_RGB_COLORS);
		if(res == 0) {
			if(res == ERROR_INVALID_PARAMETER)
				throw exception("Bitmap::create -- GetDIBits ERROR_INVALID_PARAMETER");
			else
				throw exception("Bitmap::create -- GetDIBits failed");
		}

		// Bei einem 8bpp Bitmap eine Farbpalette erstellen
		if(bpp == BITMAP_8) {
			delete this->colorPalette;
			this->colorPalette = new ColorPalette(256);
			// Die Farbpalette mit Grauwerten füllen
			for(int n=0; n<256; n++) {
				this->colorPalette->colors[n] = Color(n,n,n);
			}
		}

		// DC freigeben
		DeleteDC(hdc);
		DeleteDC(hdcMem);

		// Wegen Performance
		this->lineWidthBytes = this->getBPP() / 8 * this->getWidth();
		if(this->lineWidthBytes % 4 != 0)
			this->lineWidthBytes += (4 - this->lineWidthBytes % 4);

		this->bytesPerPixel = this->getBPP()/8;
		this->bufferDim = this->getDimensions() - Vec2(1,1);
	}

	// ========================================================== //
	// save
	// ========================================================== //
	void Bitmap::save(string filename) {
		// Dateikopf
		BITMAPFILEHEADER fileHeader;
		fileHeader.bfType = 0x4D42; // 'BM' Indikator für das Bitmap-Format
		fileHeader.bfSize = sizeof(BITMAPFILEHEADER) + sizeof(BITMAPINFOHEADER) + this->getSize();	// Gesamtgröße der Datei
		fileHeader.bfOffBits = sizeof(BITMAPFILEHEADER) + sizeof(BITMAPINFOHEADER)
							   + this->bitmapInfo->bmiHeader.biClrUsed * sizeof(RGBQUAD);			// ab hier kommen die Pixeldaten
		fileHeader.bfReserved1 = 0;
		fileHeader.bfReserved2 = 0;

		HANDLE file = CreateFileA(filename.c_str(), GENERIC_READ | GENERIC_WRITE, 0, NULL, CREATE_ALWAYS, FILE_ATTRIBUTE_NORMAL, 0);

		if(file == INVALID_HANDLE_VALUE)
			throw exception("Bitmap::saveBitmap -- Cannot create file (INVALID_HANDLE_VALUE)");
		
		// Die Daten in die Datei schreiben
		unsigned long bytesWritten;
		int res = 0;

		// Datenkopf schreiben
		res = WriteFile(file, &fileHeader, sizeof(BITMAPFILEHEADER), &bytesWritten, NULL);
		if(res == 0)
			throw exception("Bitmap::saveBitmap -- Could not write fileHeader");

		// Bitmap Info Header schreiben
		if(this->bitmapInfo->bmiHeader.biClrUsed != 0) {
			// Es existiert eine Farbpalette...
			// Interne Farbpalette in die Externe kopieren
			Color pix;
			for(unsigned int i=0; i<this->bitmapInfo->bmiHeader.biClrUsed; i++) {
				pix = this->colorPalette->colors[i];
				this->bitmapInfo->bmiColors[i].rgbRed = pix.r;
				this->bitmapInfo->bmiColors[i].rgbGreen = pix.g;
				this->bitmapInfo->bmiColors[i].rgbBlue = pix.b;
			}

			unsigned long biSize = sizeof(BITMAPINFOHEADER) + this->bitmapInfo->bmiHeader.biClrUsed * sizeof(RGBQUAD);
			res = WriteFile(file, this->bitmapInfo, biSize, &bytesWritten, NULL);
		}else{
			// Es existiert keine Farbpalette...
			res = WriteFile(file, &this->bitmapInfo->bmiHeader, sizeof(BITMAPINFOHEADER), &bytesWritten, NULL);
		}

		if(res == 0)
			throw exception("Bitmap::saveBitmap -- Could not write bmiHeader");

		// Pixeldaten schreiben
		res = WriteFile(file, this->data, this->bitmapInfo->bmiHeader.biSizeImage, &bytesWritten, NULL);
		if(res == 0)
			throw exception("Bitmap::saveBitmap -- Could not write bmBits");

		CloseHandle(file);
	}

	// ========================================================== //
	// convert
	// ========================================================== //
	void Bitmap::convert(ColorDepth bpp) {
		if(this->getBPP() == bpp)
			return;
		
		unsigned int newSize;
		if(bpp == BITMAP_24)
			newSize = this->getWidth()*this->getHeight()*3;
		else
			newSize = this->getWidth()*this->getHeight();

		unsigned char* newData = new unsigned char[newSize];
		int pixelBrightness;
		unsigned int newPixelPos = 0;
		
		// Hole alle alten Pixel...
		Color pix, palettePix;
		if(bpp == BITMAP_24) {
			// 8 Bit -> 24 Bit
			for(this->nPosOnBuffer=0; this->nPosOnBuffer<this->getSize(); this->nPosOnBuffer++) {
				// Neue Farbwerte berechnen
				palettePix = this->colorPalette->getColor(this->data[this->nPosOnBuffer]);
				
				newData[newPixelPos]   = palettePix.b;
				newData[newPixelPos+1] = palettePix.g;
				newData[newPixelPos+2] = palettePix.r;

				newPixelPos+=3;
			}
		} else {
			// 24 Bit -> 8 Bit
			for(this->nPosOnBuffer=0; this->nPosOnBuffer<this->getSize(); this->nPosOnBuffer+=3) {
				// Neue Farbwerte berechnen
				pix.b = this->data[this->nPosOnBuffer];
				pix.g = this->data[this->nPosOnBuffer+1];
				pix.r = this->data[this->nPosOnBuffer+2];

				pixelBrightness = (pix.r+pix.g+pix.b)/3;
				newData[newPixelPos] = (unsigned char)pixelBrightness;
				
				newPixelPos++;
			}
		}

		// Erstelle neues Bitmap mit der neuen Farbtiefe
		this->create(this->getDimensions(), bpp);
		
		// Setze die Pixel des neuen Bitmaps
		this->setBuffer(newData);
	}

	// ========================================================== //
	// scale
	// ========================================================== //
	void Bitmap::scale(Vec2 size, InterpolationMethod im /*= IM_BILINEAR*/) {
		if(im != IM_NEAREST_NEIGHBOR && im != IM_BILINEAR)	
			throw exception("Bitmap::scale -- Unknown interpolation method");
		
		if(size.x*size.y == 0)
			throw exception("Bitmap::scale -- Invalid size");

		// Falls die Breite des Bildes ungerade ist
		unsigned int nNullBytes1 = size.x % 4;
		unsigned int nNullBytes2 = size.y % 4;
		if(nNullBytes1!=0 || nNullBytes2!=0) {
			#ifndef _HIDE_WARNINGS
				cerr<<"Bitmap::scale -- Warning: Try to use Bitmaps with even width numbers."<<endl;
			#endif
			size += Vec2(4-nNullBytes1,4-nNullBytes2);
			#ifndef _HIDE_WARNINGS
				cerr<<"Bitmap::scale -- Size set to "<<size.x<<" x "<<size.y<<endl;
			#endif
		}

		unsigned int newSize;
		if(this->getBPP() == BITMAP_24)
			newSize = size.x*size.y*3;
		else
			newSize = size.x*size.y;

		unsigned char* newData = new unsigned char[newSize];
		unsigned int newPixelPos = 0;

		//Color* newPixel = new Color[size.x*size.y];

		const float xRatio = (float)(this->getWidth()-1)/(float)size.x;
		const float yRatio = (float)(this->getHeight()-1)/(float)size.y;
		Color pix; // Neues Pixel
		Vec2 src, dst;
		Vec2f diff;
		
		if(im == IM_NEAREST_NEIGHBOR) {
			// ------ nearest neighbor interpolation ------
			if(this->getBPP() == BITMAP_24) {
				// 24 Bit Format
				for(dst.y=0; dst.y<size.y; dst.y++) {
					for(dst.x=0; dst.x<size.x; dst.x++) {
						src.x = (int)(xRatio * dst.x);
						src.y = (int)(yRatio * dst.y);

						this->nPosOnBuffer = this->lineWidthBytes*src.y + src.x*3;
						newData[newPixelPos++] = this->data[this->nPosOnBuffer];
						newData[newPixelPos++] = this->data[this->nPosOnBuffer+1];
						newData[newPixelPos++] = this->data[this->nPosOnBuffer+2];
					}
				}
			} else {
				// 8 Bit Format
				for(dst.y=0; dst.y<size.y; dst.y++) {
					for(dst.x=0; dst.x<size.x; dst.x++) {
						src.x = (int)(xRatio * dst.x);
						src.y = (int)(yRatio * dst.y);

						this->nPosOnBuffer = this->lineWidthBytes*src.y + src.x;
						newData[newPixelPos++] = this->data[this->nPosOnBuffer];
					}
				}
			}
		} else if(im == IM_BILINEAR) {
			// ---------- bilinear interpolation ---------
			Color a,b,c,d;	// Nachbarpixel
			float factorA, factorB, factorC, factorD;

			if(this->getBPP() == BITMAP_24) {
				// 24 Bit Format
				// Für alle neuen Pixel
				for(dst.y=0; dst.y<size.y; dst.y++) {
					for(dst.x=0; dst.x<size.x; dst.x++) {
						src.x = (int)(xRatio * dst.x);
						src.y = (int)(yRatio * dst.y);
									
						diff.x = (xRatio * dst.x) - src.x;
						diff.y = (yRatio * dst.y) - src.y;

						this->nPosOnBuffer = this->lineWidthBytes*src.y + src.x*3;
						a.b = this->data[this->nPosOnBuffer];
						a.g = this->data[this->nPosOnBuffer+1];
						a.r = this->data[this->nPosOnBuffer+2];
						
						b.b = this->data[this->nPosOnBuffer+3];
						b.g = this->data[this->nPosOnBuffer+4];
						b.r = this->data[this->nPosOnBuffer+5];
						
						this->nPosOnBuffer+=this->lineWidthBytes;
						c.b = this->data[this->nPosOnBuffer];
						c.g = this->data[this->nPosOnBuffer+1];
						c.r = this->data[this->nPosOnBuffer+2];

						d.b = this->data[this->nPosOnBuffer+3];
						d.g = this->data[this->nPosOnBuffer+4];
						d.r = this->data[this->nPosOnBuffer+5];
						
						// Interpolation durch: Y = A(1-w)(1-h) + B(w)(1-h) + C(h)(1-w) + Dwh
						//						  = A*factorA + B*factorB + C*factorC + D*factorD
						factorA = (1-diff.x)*(1-diff.y);
						factorB = (diff.x)*(1-diff.y);
						factorC = (diff.y)*(1-diff.x);
						factorD = (diff.x*diff.y);

						// blue
						newData[newPixelPos++] = (unsigned char)((a.b)*factorA + (b.b)*factorB +
																 (c.b)*factorC + (d.b)*factorD);
						// green
						newData[newPixelPos++] = (unsigned char)((a.g)*factorA + (b.g)*factorB +
																 (c.g)*factorC + (d.g)*factorD);
						// red
						newData[newPixelPos++] = (unsigned char)((a.r)*factorA + (b.r)*factorB +
																 (c.r)*factorC + (d.r)*factorD);
					}
				}
			} else {
				// 8 Bit Format
				// Für alle neuen Pixel
				for(dst.y=0; dst.y<size.y; dst.y++) {
					for(dst.x=0; dst.x<size.x; dst.x++) {
						src.x = (int)(xRatio * dst.x);
						src.y = (int)(yRatio * dst.y);
										
						diff.x = (xRatio * dst.x) - src.x;
						diff.y = (yRatio * dst.y) - src.y;

						this->nPosOnBuffer = this->lineWidthBytes*src.y + src.x;
						a.b = this->data[this->nPosOnBuffer];
						b.b = this->data[this->nPosOnBuffer+1];
						
						this->nPosOnBuffer+=this->lineWidthBytes;
						c.b = this->data[this->nPosOnBuffer];
						d.b = this->data[this->nPosOnBuffer+1];

						// Interpolation durch: Y = A(1-w)(1-h) + B(w)(1-h) + C(h)(1-w) + Dwh
						newData[newPixelPos++] = (unsigned char)(
																(a.b)*(1-diff.x)*(1-diff.y) + (b.b)*(diff.x)*(1-diff.y) +
																(c.b)*(diff.y)*(1-diff.x)   + (d.b)*(diff.x*diff.y)
																);
					}
				}
			}
		}

		// Erstelle neues Bitmap mit der neuen Farbtiefe
		this->create(size, this->getBPP());
		
		// Setze die Pixel des neuen Bitmaps
		this->setBuffer(newData);
	}

	// ========================================================== //
	// copy
	// ========================================================== //
	Bitmap Bitmap::copy(Vec2 from, Vec2 to) {
		if(from.x < 0 || from.y < 0 || to.x <= from.x || to.y <= from.y || to.x >= this->getWidth() || to.y >= this->getHeight())
			throw exception("Bitmap::copy -- Rectangle out of range or invalid parameters");
		
		Vec2 src, dst, size;
		size = to-from+Vec2(1,1);
		Bitmap* bitmapPart = new Bitmap(size, this->getBPP());

		// Pixel von hier nach bitmapPart kopieren
		for(src.y=from.y; src.y<=to.y; src.y++) {
			for(src.x=from.x; src.x<=to.x; src.x++) {
				dst = src-from;
				bitmapPart->setPixel(dst, this->getPixel(src));
			}
		}

		return *bitmapPart;
	}

	Bitmap Bitmap::copy() {
		return this->copy(Vec2(0,0), this->getDimensions()-Vec2(1,1));
	}

	// ========================================================== //
	// paste
	// ========================================================== //
	void Bitmap::paste(Bitmap* bmp, Vec2 pos) {
		if(pos.x < 0 || pos.y < 0)
			throw exception("Bitmap::paste -- Negative parameters for pos");
		if(pos.x+bmp->getWidth() > this->getWidth() || pos.y+bmp->getHeight() > this->getHeight())
			throw exception("Bitmap::paste -- Bitmap is too large to be pasted on that position");

		Vec2 src, dst;
		Color pix;

		for(dst.x=pos.x; dst.x<pos.x+bmp->getWidth(); dst.x++) {
			for(dst.y=pos.y; dst.y<pos.y+bmp->getHeight(); dst.y++) {
				src = dst - pos;
				pix = bmp->getPixel(src);

				// Falls die Bitmaps unterschiedliche Farbtiefen haben, Farbe korrigieren...
				if(this->getBPP() != bmp->getBPP()) {
					if(this->getBPP() == BITMAP_24 && bmp->getBPP() == BITMAP_8) {
						pix = bmp->colorPalette->getColor(pix.b);
					} else {
						int pixelBrightness = (pix.r+pix.g+pix.b)/3;
						pix.b = pixelBrightness;
					}
				}

				this->setPixel(dst, pix);
			}
		}

	}

	void Bitmap::operator =(Bitmap& bmp) {
		this->create(bmp.getDimensions(), bmp.getBPP());
		Vec2 pos;

		// Pixeldaten kopieren
		for(pos.y=0; pos.y < this->getHeight(); pos.y++) {
			for(pos.x=0; pos.x < this->getWidth(); pos.x++) {
				this->setPixel(pos, bmp.getPixel(pos));
			}
		}

		// Farbtabelle kopieren
		for(unsigned int n=0; n<bmp.colorPalette->getSize(); n++) {
			this->colorPalette->setColor(n, bmp.colorPalette->getColor(n));
		}
	}

	// ========================================================== //
	// setPixel
	// ========================================================== //
	void Bitmap::setPixel(Vec2 v, Color pix) {
		// Koordinaten fangen unten links an (komischerweise)
		// = Zeilen sind umgekehrt
		if(!this->isPixelInRange(v)) {
			char posStr[16] = "";
			sprintf_s(posStr, "(%i|%i)", v.x, v.y);
			string emsg = "Bitmap::setPixel -- Pixel out of range ";
			emsg += posStr;
			throw exception(emsg.c_str());
		}

		v.y = this->bufferDim.y-v.y;

		if(this->getBPP() == BITMAP_8) {		// 8 Bit
			this->nPosOnBuffer = this->lineWidthBytes*v.y + v.x;
			this->data[this->nPosOnBuffer] = pix.b;
		} else {								// 24 Bit
			this->nPosOnBuffer = this->lineWidthBytes*v.y + v.x*3;
			this->data[this->nPosOnBuffer+2] = pix.r;
			this->data[this->nPosOnBuffer+1] = pix.g;
			this->data[this->nPosOnBuffer] = pix.b;
		}
	}

	void Bitmap::setPixel(Color pix) {
		if(this->getBPP() == BITMAP_8) {		// 8 Bit
			this->data[this->nPosOnBuffer] = pix.b;
		} else {								// 24 Bit
			this->data[this->nPosOnBuffer+2] = pix.r;
			this->data[this->nPosOnBuffer+1] = pix.g;
			this->data[this->nPosOnBuffer] = pix.b;
		}
	}
	// ========================================================== //
	// getPixel
	// ========================================================== //
	Color Bitmap::getPixel(Vec2 v) {
		// Koordinaten fangen unten links an (komischerweise)
		// = Zeilen sind umgekehrt

		if(!this->isPixelInRange(v)) {
			char posStr[16] = "";
			sprintf_s(posStr, "(%i|%i)", v.x, v.y);
			string emsg = "Bitmap::getPixel -- Pixel out of range ";
			emsg += posStr;
			throw exception(emsg.c_str());
		}

		v.y = this->bufferDim.y-v.y;

		Color pixel;
		if(this->getBPP() == BITMAP_8) {		// 8 Bit
			this->nPosOnBuffer = this->lineWidthBytes*v.y + v.x;
			pixel.b	= (this->data[nPosOnBuffer]);
		} else {								// 24 Bit
			this->nPosOnBuffer = this->lineWidthBytes*v.y + v.x*3;
			pixel.r	= (this->data[nPosOnBuffer+2]);
			pixel.g	= (this->data[nPosOnBuffer+1]);
			pixel.b	= (this->data[nPosOnBuffer]);
		}
		
		return pixel;
	}

	// ========================================================== //
	// fill
	// ========================================================== //
	void Bitmap::fill(Color color) {
		if(this->getBPP() == BITMAP_8) {
			for(unsigned int n=0; n<this->getSize(); n++) {
				this->data[n] = color.b;
			}
		}else if(this->getBPP() == BITMAP_24) {
			for(unsigned int n=0; n<this->getSize(); n+=3) {
				this->data[n+2] = color.r;
				this->data[n+1] = color.g;
				this->data[n] = color.b;
			}
		}
	}

	// ========================================================== //
	// drawLine
	// ========================================================== //
	void Bitmap::drawLine(Vec2 from, Vec2 to, Color color) {
		// Bresenhams Linienalgorithmus.	
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
				this->setPixel(from, color);
				cycle += sh_delta;
				if (cycle > lg_delta) {
					cycle -= lg_delta;
					from.y += sh_step;
				}
				from.x += lg_step;
			}
			this->setPixel(from, color);
		}
		cycle = sh_delta >> 1;
		while (from.y != to.y) {
			this->setPixel(from, color);
			cycle += lg_delta;
			if (cycle > sh_delta) {
				cycle -= sh_delta;
				from.x += lg_step;
			}
			from.y += sh_step;
		}
		this->setPixel(from, color);
	}

	// ========================================================== //
	// drawRect
	// ========================================================== //
	void Bitmap::drawRect(Vec2 v1, Vec2 v2, Color color) {
		this->drawLine(v1, Vec2(v2.x,v1.y), color);
		this->drawLine(v1, Vec2(v1.x,v2.y), color);
		this->drawLine(Vec2(v2.x,v1.y), v2, color);
		this->drawLine(Vec2(v1.x,v2.y), v2, color);
	}

	// ========================================================== //
	// drawCircle
	// ========================================================== //
	void Bitmap::drawCircle(Vec2 m, int r, Color color) {
		// Bresenhams Kreisalgorithmus.
		int    x;
		int    y;
		int    p2;

		x = 0;
		y = r;
		p2 = 3 - (r << 2);

		while(x < y) {
			this->setPixel(Vec2(m.x - x, m.y - y), color);
			this->setPixel(Vec2(m.x + x, m.y - y), color);
			this->setPixel(Vec2(m.x - x, m.y + y), color);
			this->setPixel(Vec2(m.x + x, m.y + y), color);
			this->setPixel(Vec2(m.x - y, m.y - x), color);
			this->setPixel(Vec2(m.x + y, m.y - x), color);
			this->setPixel(Vec2(m.x - y, m.y + x), color);
			this->setPixel(Vec2(m.x + y, m.y + x), color);

			if(p2 < 0) p2 = p2 + (x << 2) + 6;
			else {
				p2 = p2 + ((x - y) << 2) + 10;
				y--;
			}

			x++;
		}

		if(x == y) {
			this->setPixel(Vec2(m.x - x, m.y - x), color);
			this->setPixel(Vec2(m.x + x, m.y - x), color);
			this->setPixel(Vec2(m.x - x, m.y + x), color);
			this->setPixel(Vec2(m.x + x, m.y + x), color);
		}
	}
}