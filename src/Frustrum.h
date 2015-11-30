/*
 * Frustrum.h
 *
 *  Created on: Feb 8, 2012
 *      Author: jeroenb
 */

#ifndef FRUSTRUM_H_
#define FRUSTRUM_H_

#include <iostream>
#include <cmath>

class Frustrum {
public:
	float l,r,b,t,n,f;
	Frustrum();
	Frustrum(float l, float r, float b, float t, float n, float f);
	Frustrum(float fov, float aspectratio, float front, float back);
	virtual ~Frustrum();
};

inline Frustrum::Frustrum()
	: l(-1), r(1), b(-1), t(1), n(1), f(100){
}

inline Frustrum::Frustrum(float l, float r, float b, float t, float n, float f)
	: l(l), r(r), b(b), t(t), n(n), f(f){
}

inline Frustrum::Frustrum(float fov, float aspectratio, float front, float back){
	// using fov, determine height and width
	const float degree2radian = 3.14159265358979323846 / 180.0;
	float tang = tan(fov*degree2radian);
	float height = front * tang;
	float width = height*aspectratio;
	// put it in general frustrum coordinates
	l = -width; r = width;
	b = -height; t = height;
	n = front; f = back;
}

inline Frustrum::~Frustrum() {
	// TODO Auto-generated destructor stub
}


#endif /* FRUSTRUM_H_ */
