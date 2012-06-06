//============================================================================
// Name        : drawWidget.cpp
// Author      : Phillip Wilt
// Version     :
// Copyright   : 
// Description : A widget to set up the drawing scene and draw objects to
//				 the voxiebox.
//				TODO: Remove dynamic memory allocation
//					-Split levels in 3 color planes that are 8 steps in time
//					-Correct for travel from bottom to top and reversing colors
//					-Change to least significant bit first
//============================================================================

#include "drawWidget.h"
#include <cmath>
#include <cstddef>
using namespace std;
//dimensions of the projector
const int xMax = 684;
const int yMax = 608;
const int zMax = 240; //100 pix per layer, used for interpolation later
const int stepCount = 3;
const int colorBits = 8;

DrawWidget::DrawWidget(int xWidth,int yDepth,int zHeight){


	xSize = xMax;
	ySize = yMax;
	zSize = zMax;

	//take in dimensions and rescale
	scaleX = (double)xMax / (double)xWidth;
	scaleY = (double)yMax / (double)yDepth;
	scaleZ = (double)zMax / (double)zHeight;

	int numLevels = LEVELS;
	layers = new rgbPixel*[numLevels];
	images = new BMP[numLevels / stepCount];
	int count = 0;
	//initialize our layers to be 2D representation of the XY plane
	//using only RGBDrawWidget(xSize = 684, ySize  = 608, zSize = 700);
	for(int i = 0; i < numLevels; i++)
	{
		if(i < numLevels - 1 && i % stepCount == 0){

			images[count].SetSize(xMax,yMax);
			images[count++].SetBitDepth(24);
		}

		layers[i] = new rgbPixel[xMax * yMax];
	}

}

DrawWidget::~DrawWidget(){

	for(int i = 0; i < LEVELS; i++)
	{
		delete[] layers[i];
	}

	delete[] layers;
	delete[] images;

}

//sets the RGB value of the pixel specified at level
bool DrawWidget::setPixel(int level, int x, int y, rgbPixel rgb)
{/*
	if( (level < 0 || level >= numLevels) || (x < 0) || (y < 0) )
		return false;
*/
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


void DrawWidget::convertToBMP()
{
	int steps = LEVELS / stepCount;
	RGBApixel p;
	int r,g,b;
	int count = 0;
	for(int z = 0; z < LEVELS; z++){

		for(int y = 0; y < yMax; y++){
			for(int x = 0; x < xMax; x++){ //this is some nasty loop
				r = 0;
				g = 0;
				b = 0;
				for(int i = 0; i < steps; i++){ //gather up the color at every level

					if(layers[z+i][y*xMax + x].r > 0){
						r += pow(2,colorBits-1-i);
						//cout << r << " ";
					}
					if(layers[z+i][y*xMax + x].g > 0)
						g += pow(2,colorBits-1-i);
					if(layers[z+i][y*xMax + x].b > 0)
						b += pow(2,colorBits-1-i);
				}
				p.Red = r;
				p.Green = g;
				p.Blue = b;
				images[z / stepCount].SetPixel(x,y,p); //set the pixel as the sum of the colors
			}
			//cout << endl;
		}
		count++;
	
	}
}

bool DrawWidget::saveBMP(const char* file){


	convertToBMP();

	char buffer[32];
	
	for(int i = 0; i < LEVELS / stepCount; i++){
		sprintf(buffer, "%s%04d.bmp", file, i);
		if(!images[i].WriteToFile(buffer))
			return false;
	}
	
	return true;		

}

//fill planes with a sphere
void DrawWidget::drawSphere(int r) {

	float step = PI / LEVELS; //divide into steps
	float minAngle = asin(15 / r);
	int modRadius = 0;
	

	//locate our starting positions
	int startX = xSize / 2;
	int startY = ySize / 2;
	double height = r / 10;
	int x,y,z;
	
	rgbPixel rgb;
	rgb.r = 255;
	rgb.g = 0;
	rgb.b = 0;
	
	for(int z= 0; z < LEVELS; z++) {
		modRadius = int(r * sin(step*z)); //radius of current circle
		for(int y = startY - modRadius; y < startY + modRadius; y++){
			for(int x = startX - modRadius; x < startX + modRadius; x++){
				this->setPixel(z,x,y,rgb);
			}
		}
	}
}
