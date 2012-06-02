/*
 * main.cpp
 *
 *  Created on: Jun 2, 2012
 *      Author: pmw
 *
 *      A testing main for DrawWidget.
 */

#include "drawWidget.h"
#include <iostream>

using namespace std;

int main(int argc, char *argv[])
{

	DrawWidget widg(684,608,300);


	cout << widg.getXSize() << " " << widg.getYSize() << " " << widg.getZSize() << " " << endl;
	cout << widg.getXScale() << " " << widg.getYScale() << " " << widg.getZScale() << " " << endl;

	rgbPixel white;
	white.r = 255;
	white.g = 255;
	white.b = 255;

	rgbPixel black;
	black.r = 0;
	black.g = 0;
	black.b = 0;

	for(int y = 0; y < widg.getYSize(); y++){
		for(int x = 0; x < widg.getXSize(); x++){
			if(x > 250 && x < 300)
				widg.setPixel(0, 0, 1, black);
			else
				widg.setPixel(0, x, y, white);
		}
	}


/*
	for(int y = 0; y < widg.getYSize();  y++){
		for(int x = 0; x < widg.getXSize(); x++){
			cout << (int)widg.getPixel(0, x, y).r << " ";
			cout << (int)widg.getPixel(0, x, y).g << " ";
			cout << (int)widg.getPixel(0, x, y).b << " ";
		}
			cout << endl;
	}
	*/

	const char *file = "first.bmp";

	if(widg.saveBMP(file))
		cout << "Saved!"<< endl;
	else
		cout << "Didn't save!"<< endl;

	return 0;
}

