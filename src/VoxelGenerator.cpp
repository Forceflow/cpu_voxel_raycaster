///*
// * VoxelGenerator.cpp
// *
// *  Created on: Feb 21, 2012
// *      Author: jeroenb
// */
//
//#include "VoxelGenerator.h"
//
//using namespace std;
//
//VoxelData generateColorField(int x, int y, int z, double percent_sparse){
//	VoxelData a = VoxelData(x,y,z);
//	for(int i = 0; i < x; i++){
//		for(int j = 0; j < y; j++){
//			for(int k = 0; k < z; k++){
//					double opacity = 1.0;
//					double color_x = ((double)i/x)*255;
//					double color_y = ((double)j/y)*255;
//					double color_z = ((double)k/z)*255;
//					(*a)[i][j][k] = DataPoint(opacity, RGBColor((float)color_x,(float)color_y,(float)color_z));
//					//bool isempty = !(x % 20 == 0);
//					bool isempty = (rand() % 1000) < percent_sparse*10;
//					if(isempty) (*a)[i][j][k] = DataPoint();
//				}
//			}
//		}
//	return a;
//}
//
//
//GridData* generateRandomColorField(int x, int y, int z, double percent_sparse){
//	GridData* a = new GridData(boost::extents[x][y][z]);
//	//initialize random seed
//	srand ((unsigned int) time(NULL));
//	for(int i = 0; i < x; i++){
//		for(int j = 0; j < y; j++){
//			for(int k = 0; k < z; k++){
//				bool isempty = (rand() % 1000) < percent_sparse*10;
//				if(isempty){
//					(*a)[i][j][k] = DataPoint(); // generate empty DataPoint
//				} else {
//					float opacity = ( rand() % 50 + 1.0 ) / 100.0;
//					float r = (float) (rand() % 255) + 1;
//					float g = (float) (rand() % 255) + 1;
//					float b = (float) (rand() % 255) + 1;
//					(*a)[i][j][k] = DataPoint(opacity, RGBColor(r,g,b));
//				}
//			}
//		}
//	}
//	return a;
//}
//
//GridData* generateSphere(int x, int y, int z, double percent_sparse){
//	GridData* a = new GridData(boost::extents[x][y][z]);
//	//initialize random seed
//	srand ((unsigned int) time(NULL));
//	for(int i = 0; i < x; i++){
//		for(int j = 0; j < y; j++){
//			for(int k = 0; k < z; k++){
//				float xd = x / 2.0f;
//				float yd = y / 2.0f;
//				float zd = z / 2.0f;
//				float distance = sqrt( ((i-xd)*(i-xd)) + ((j-yd)*(j-yd)) + ((k-zd)*(k-zd)) );
//				if(distance > (xd)){
//					(*a)[i][j][k] = DataPoint(); // generate empty DataPoint
//				} else {
//					if((rand() % 1000) < percent_sparse*10){
//						(*a)[i][j][k] = DataPoint();
//					}
//					else{
//						float opacity = ( rand() % 50 + 1.0 ) / 100.0;
//						float intensecolor = 1 - ((xd-distance)/xd);
//						float intensity_r = (float) intensecolor * 255;
//						float intensity_g = (float) intensecolor * 255;
//						float intensity_b = (float) intensecolor * 255;
//						float r = (intensity_r);
//						float g = (intensity_g);
//						float b = (intensity_b);
//						(*a)[i][j][k] = DataPoint(opacity, RGBColor(r,g,b));
//					}
//				}
//			}
//		}
//	}
//	return a;
//}
//
//GridData generateSphere2(int x, int y, int z){
//	GridData a = GridData(boost::extents[x][y][z]);
//	//initialize random seed
//	srand ((unsigned int) time(NULL));
//	for(int i = 0; i < x; i++){
//		for(int j = 0; j < y; j++){
//			for(int k = 0; k < z; k++){
//				float xd = x / 2.0;
//				float yd = y / 2.0;
//				float zd = z / 2.0;
//				float distance = sqrt( ((i-xd)*(i-xd)) + ((j-yd)*(j-yd)) + ((k-zd)*(k-zd)) );
//				if(distance > (xd)){
//					a[i][j][k] = DataPoint(); // generate empty DataPoint
//				} else {
//					//use distance for probability of DataPoint
//					float scale = (xd-distance)/xd;
//					if((rand() % 100) > scale*scale*scale*scale*scale*100){
//						a[i][j][k] = DataPoint();
//					}
//					else{
//						float opacity = ( rand() % 50 + 1.0 ) / 100.0;
//						float intensecolor = ((xd-distance)/xd);
//						float intensity_r = (float) intensecolor * 255.0f;
//						float intensity_g = (float) intensecolor * 255.0f;
//						float intensity_b = (float) intensecolor * 255.0f;
//						float r = (float) clampd((intensity_r + 100),0,255);
//						float g = (intensity_g +10 );
//						float b = (intensity_b +10);
//						a[i][j][k] = DataPoint(opacity, RGBColor(r,g,b));
//					}
//				}
//			}
//		}
//	}
//	return a;
//}
//
//GridData generateTorus(int x, int y, int z, int radius, int diameter){
//	GridData a = GridData(boost::extents[x][y][z]);
//	//initialize random seed
//	srand ((unsigned int) time(NULL));
//	for(int i = 0; i < x; i++){
//		for(int j = 0; j < y; j++){
//			for(int k = 0; k < z; k++){
//				//centerblock
//				//for tomorrow
//				// todo
//				if(abs(radius*radius - diameter) > 40){
//					a[i][j][k] = DataPoint(); // generate empty DataPoint
//				} else {
//						float opacity = ( rand() % 50 + 1.0 ) / 100.0;
//						float r = (float) 150 + (rand() & 5);
//						float g = (float) 10+ (rand() & 15);
//						float b = (float) 10+ (rand() & 15);
//						a[i][j][k] = DataPoint(opacity, RGBColor(r,g,b));
//					}
//				}
//			}
//	}
//	return a;
//}
//
////not working yet ...
//GridData generateRandomSinusField(int x, int y, int z, double percent_sparse){
//	GridData a = GridData(boost::extents[x][y][z]);
//	//initialize random seed
//	srand ((unsigned int) time(NULL));
//	for(int i = 0; i < x; i++){
//		for(int j = 0; j < y; j++){
//			for(int k = 0; k < z; k++){
//				bool isempty = (rand() % 100) < percent_sparse;
//				if(isempty){
//					a[i][j][k] = DataPoint(); // generate empty DataPoint
//				} else {
//					double opacity = ( rand() % 50 + 1.0 ) / 100.0;
//					const double degree2radian = 3.14159265358979323846 / 180.0;
//					int r = (int) sin(float(i/x)*degree2radian);
//					int g = (int) sin(float(j/y)*degree2radian);
//					int b = (int) sin(float(k/z)*degree2radian);
//					r = r % 255;
//					g = g % 255;
//					b = b % 255;
//					a[i][j][k] = DataPoint(opacity, RGBColor(float(r),float(g),float(b)));
//				}
//			}
//		}
//	}
//	return a;
//}
//
//
