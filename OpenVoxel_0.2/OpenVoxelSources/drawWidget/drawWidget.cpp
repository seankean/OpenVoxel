//============================================================================
// Name        : drawWidget.cpp
// Author      : Phillip Wilt
// Version     :
// Copyright   : 
// Description : A widget to set up the drawing scene and draw objects to
//				 the voxiebox.
//============================================================================

#include "drawWidget.h"
#include <cstddef>
using namespace std;
//dimensions of the projector
const int xMax = 684;
const int yMax = 608;
const int zMax = 700; //100 pix per layer, used for interpolation later


DrawWidget::DrawWidget(int xWidth,int yDepth,int zHeight){


	xSize = xMax;
	ySize = yMax;
	zSize = zMax;

	//take in dimensions and rescale
	scaleX = (double)xMax / (double)xWidth;
	scaleY = (double)yMax / (double)yDepth;
	scaleZ = (double)zMax / (double)zHeight;

	numLevels = int(zMax / 100);

	layers = new rgbPixel*[numLevels];

	//initialize our layers to be 2D representation of the XY plane
	//using only RGBDrawWidget(xSize = 684, ySize  = 608, zSize = 700);
	for(int i = 0; i < numLevels; i++)
	{
		layers[i] = new rgbPixel[xMax * yMax];
	}

}

DrawWidget::~DrawWidget(){

	for(int i = 0; i < numLevels; i++)
		 delete[] layers[i];

	delete[] layers;

}

//sets the RGB value of the pixel specified at level
bool DrawWidget::setPixel(int level, int x, int y, rgbPixel rgb)
{
	if( (level < 0 || level >= numLevels) || (x < 0) || (y < 0) )
		return false;

	layers[level][y * xSize + x].r = rgb.r;
	layers[level][y * xSize + x].g = rgb.g;
	layers[level][y * xSize + x].b = rgb.b;

	return true;

}

rgbPixel DrawWidget::getPixel(int level, int x, int y)
{
	//if( (level < 0 || level >= numLevels) || (x < 0) || (y < 0) )
		//	retur;

	return layers[level][y * xSize + x];
}


void DrawWidget::convertToBMP(int level)
{

	//layers[level]

}

bool DrawWidget::saveBMP(const char* file){

	BMP image;
	image.SetSize(xMax,yMax);
	image.SetBitDepth(24);


	for(int y = 0; y < yMax; y++){
		for(int x = 0; x < xMax; x++){

			RGBApixel p;
			p.Blue = layers[0][y*xMax + x].b;
			p.Red = layers[0][y*xMax + x].r;
			p.Green = layers[0][y*xMax + x].g;
			p.Alpha = 0;

			image.SetPixel(x,y,p);
		}
	}

	if(image.WriteToFile(file))
		return true;
	else
		return false;

	//image(0,0) = layers[0][1].r;

	//image.SetPixel(x,y,layers[0][y * xSize + x]);

}
