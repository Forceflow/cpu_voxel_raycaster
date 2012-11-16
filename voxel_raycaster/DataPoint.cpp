/*
 * DataPoint.cpp
 *
 *  Created on: Feb 20, 2012
 *      Author: jeroenb
 */

#include "DataPoint.h"


DataPoint::DataPoint() : opacity(0.0f), color(RGBColor(0,0,0)), normal(vec3(0.0f,0.0f,0.0f)){
}

DataPoint::DataPoint(float opacity, RGBColor color) : opacity(opacity), color(color){
}

DataPoint::DataPoint(float opacity, RGBColor color, vec3 normal) : opacity(opacity), color(color), normal(normal){
}

bool DataPoint::isEmpty(){
	return (opacity == 0.0f);
}
