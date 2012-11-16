/*
 * DataPoint.h
 *
 *  Created on: Feb 20, 2012
 *      Author: jeroenb
 */

#ifndef DATAPOINT_H_
#define DATAPOINT_H_

#include "array_3d.h"
#include "util.h"



class DataPoint {
public:
	float opacity;
	RGBColor color;
	vec3 normal;

	DataPoint();
	DataPoint(float opacity, RGBColor color);
	DataPoint(float opacity, RGBColor color,vec3 normal);
	bool isEmpty();
};

#endif /* DATAPOINT_H_ */

typedef al::array::array_3d<DataPoint> VoxelData;
