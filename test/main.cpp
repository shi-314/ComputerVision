#include <cv/ComputerVision.h>
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

		Bitmap img1, img2;
		img1.load("img/lines.bmp");

		Filter::grayscale(&img1);
		img1.save("out/grayscale.bmp");

		img1.convert(ColorDepth::BITMAP_8);

		Histogram hist1(&img1);
		Bitmap hist1Graph = hist1.getGraph();
		hist1Graph.save("out/hist1.bmp");

		Filter::histogramEqualization(&img1);
		img1.save("out/histeq.bmp");

		Histogram hist2(&img1);
		Bitmap hist2Graph = hist2.getGraph();
		hist2Graph.save("out/hist2.bmp");

		img2 = img1.copy();
		cout<<img1.getWidth()<<" x "<<img1.getHeight()<<" @ "<<img1.getBPP()<<" = "<<img1.getSize();
		
		cout<<"-----------------------\n";
		cout<<"Timer started...\n\n";

		Filter::canny(&img1, 3, 12);
		img1.save("out/canny.bmp");

		cout<<"hough transform ..."<<endl;
		HoughTransform hough(&img1);

		img1.convert(ColorDepth::BITMAP_24);
		img2.convert(ColorDepth::BITMAP_24);

		hough.calculate();
		hough.saveHoughImage("out/hough_original.bmp");
		
		cout<<"get hough lines..."<<endl;

		vector<Vec2> houghLines = hough.getHoughLines(50);
		vector<Line> lineSegs;
		
		for(int n=0; n<houghLines.size(); n++) {
			Vec2 hl = houghLines[n];
			vector<Line> segs = hough.getLineSegments(hl);

			for(int m=0; m<segs.size(); m++) {
				segs[m].draw(&img2, Color(0,0,255));
			}
		}
		
		img2.save("out/detected_lines.bmp");
		hough.saveHoughImage("out/houghspace.bmp");
		

		performanceCounter.StopTimer();
		long long microSecs = performanceCounter.GetTime();
		double milliSecs = (double)microSecs/1000;
		cout<<"\nTime elapsed: "<<milliSecs<<" ms"<<endl;
		cout<<"-----------------------\n";

		cout<<"save img1..."<<endl;
		img1.save("out/bili.bmp");
	}catch(const exception &e) {
		cerr<<"Exception Error: "<<e.what()<<endl;
		cerr<<"Last Error: "<<GetLastError()<<endl;
	}catch(...) {
		cerr<<"Exception Error: unknown exception"<<endl;
	}
	cout<<"\ndone";
	_getch();
}