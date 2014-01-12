#include "ComputerVision\ComputerVision.h"
#include <conio.h>
#include "PreciseTimer.h"

using namespace std;
using namespace ComputerVision;

int main() {
	cout<<"Computer Vision\n\n";
	srand(time(0));

	try {
		CPreciseTimer performanceCounter;

		performanceCounter.StartTimer();
		//Bitmap img1(Vec2(128,128), Color(90,90,90));
		Bitmap img1, img2;
		img1.load("lines.bmp");

		Filter::grayscale(&img1);
		img1.save("out_grayscale.bmp");

		img1.convert(ColorDepth::BITMAP_8);

		Histogram hist1(&img1);
		Bitmap hist1Graph = hist1.getGraph();
		hist1Graph.save("out_hist1.bmp");

		Filter::histogramEqualization(&img1);
		img1.save("out_grayscale_histeq.bmp");

		Histogram hist2(&img1);
		Bitmap hist2Graph = hist2.getGraph();
		hist2Graph.save("out_hist2.bmp");

		img2 = img1.copy();
		cout<<img1.getWidth()<<" x "<<img1.getHeight()<<" @ "<<img1.getBPP()<<" = "<<img1.getSize();
		
		cout<<"-----------------------\n";
		cout<<"Timer started...\n\n";
		//cout<<"scale..."<<endl;
		//Vec2 size = img1.getDimensions()*1.8;
		//img1.scale(size, InterpolationMethod::IM_BILINEAR);
		
		//cout<<"grayscale..."<<endl;
		//Filter::grayscale(&img1);

		//cout<<"convert..."<<endl;
		//img1.convert(BITMAP_8);

		//cout<<"gaussian blur..."<<endl;
		//Filter::gaussianBlur(&img1, Filter::MATRIX_3);

		//cout<<"histogramEqual..."<<endl;
		//Filter::histogramEqualization(&img1);

		//cout<<"sobel..."<<endl;
		//Filter::sobel(&img1);
		//img1.save("out_sobel.bmp");

		//cout<<"laplace..."<<endl;
		//Filter::sobel(&img1);
		//Filter::threshold(&img1, 130, 255);
		//img1.save("out_sobel_thresh.bmp");

		Filter::canny(&img1, 3, 12);
		img1.save("out_canny.bmp");

		cout<<"hough transform ..."<<endl;
		HoughTransform hough(&img1);
		
		//vector<Line> lines = hough.ppht();
		//cout<<"Lines: "<<lines.size()<<endl;
		//for(int n=0; n<lines.size(); n++) {
		//	lines[n].draw(&img2, Color(255,0,0));
		//}

		img1.convert(ColorDepth::BITMAP_24);
		img2.convert(ColorDepth::BITMAP_24);

		hough.calculate();
		hough.saveHoughImage("out_hough_original.bmp");
		cout<<"get hough lines..."<<endl;
		vector<Vec2> houghLines = hough.getHoughLines(50);
		vector<Line> lineSegs;
		
		for(int n=0; n<houghLines.size(); n++) {
			Vec2 hl = houghLines[n];
			//cout<<"get line segments..."<<endl;
			vector<Line> segs = hough.getLineSegments(hl);
			//cout<<"draw lines..."<<endl;
			for(int m=0; m<segs.size(); m++) {
				segs[m].draw(&img2, Color(0,0,255));
			}
		}
		
		img2.save("out_detected_lines.bmp");
		hough.saveHoughImage("out_houghspace.bmp");
		

		performanceCounter.StopTimer();
		long long microSecs = performanceCounter.GetTime();
		double milliSecs = (double)microSecs/1000;
		cout<<"\nTime elapsed: "<<milliSecs<<" ms"<<endl;
		cout<<"-----------------------\n";
		/*cout<<"histogram 2 + draw + save ..."<<endl;
		Histogram hist2(&img1);
		Bitmap hist2Graph = hist2.getGraph();
		hist2Graph.save("out_hist2.bmp");*/

		cout<<"save img1..."<<endl;
		img1.save("out_bili.bmp");
		// ---------------------------------------------
		/*Bitmap grayscale(Vec2(64,64), BITMAP_8, Color(0));
		grayscale.drawCircle(Vec2(32,32), 10, Color(255));
		grayscale.drawCircle(Vec2(42,42), 10, Color(150));
		grayscale.save("out_grayscale.bmp");

		// Farbpalette von grayscale speichern
		Bitmap grayscalePalette(Vec2(256,8));
		for(unsigned int i=0; i<grayscale.colorPalette->getSize(); i++) {
			Color pix = grayscale.colorPalette->getColor(i);
			grayscalePalette.setPixel(Vec2(i,5), pix);
		}
		grayscalePalette.save("out_grayscale_palette.bmp");*/
	}catch(const exception &e) {
		cerr<<"Exception Error: "<<e.what()<<endl;
		cerr<<"Last Error: "<<GetLastError()<<endl;
	}catch(...) {
		cerr<<"Exception Error: unknown exception"<<endl;
	}
	cout<<"\ndone";
	_getch();
}