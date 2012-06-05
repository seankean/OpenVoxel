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

	DrawWidget widg(684,608,640);

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

	//replace the next piece with initalization to 0
	for(int z = 0; z < 64; z++){
		for(int y = 0; y < widg.getYSize(); y++){
			for(int x = 0; x < widg.getXSize(); x++){
				widg.setPixel(0, x, y, black);
			}
		}
	}

	widg.drawSphere(200);
	const char *file = "images";

	if(widg.saveBMP(file))
		cout << "Saved!"<< endl;
	else
		cout << "Didn't save!"<< endl;

	return 0;

}

