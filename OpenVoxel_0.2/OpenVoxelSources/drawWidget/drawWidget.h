/*
 * drawWidget.h
 *
 *  Created on: Jun 2, 2012
 *      Author: pmw
 
	Currently takes in rgb values and a world space, scales to projector 3d space
	prints out first level bitmap in colorspace.
	
	TODO: -add real conversion from color vals to timesteps
		  -output all levels in sequence format bitmaps
		  -add geometry
		  -and more!
 */

#ifndef DRAWWIDGET_H_
#define DRAWWIDGET_H_
#define LEVELS 64
#define PI 3.149159

#include "EasyBMP.h" //so we can write bitmaps

struct rgbPixel {

	unsigned char r,b,g;
};

class DrawWidget {

	private:

		int xSize, ySize, zSize, numLevels;
		double scaleX, scaleY, scaleZ;
		rgbPixel** layers; //levels
		BMP* images;

	public:

		//takes in the size of the world we are working with
		//and rescales
		DrawWidget(int xWidth,int yWidth,int zHeight);
		~DrawWidget();

		double getXScale(){return scaleX;}
		double getYScale(){return scaleY;}
		double getZScale(){return scaleZ;}

		int getXSize(){return xSize;}
		int getYSize(){return ySize;}
		int getZSize(){return zSize;}
		int getLevels(){return int(LEVELS);}

		//returns true if able to set pixel, else not
		bool setPixel(int level, int x, int y, rgbPixel rgb);
		rgbPixel getPixel(int level, int x, int y);

		//saves all levels out to bitmaps with starting filename
		bool saveBMP(const char* file);
		//smashs all levels into individual steps and layers images
		void convertToBMP();

		//graphics methods
		void drawSphere(int r);

};

#endif /* DRAWWIDGET_H_ */
