/*
 * misc_math.h
 *
 *  Created on: Feb 14, 2012
 *      Author: jeroenb
 */
#ifndef MISC_MATH_H_
#define MISC_MATH_H_

#include "float.h"
#include <math.h>

inline int log2(float x){
	int log = log10((float) x)/log10(2.0f);
	return log;
}

inline int isPowerOf2(unsigned int x){
  return ((x != 0) && !(x & (x - 1)));
}

template <typename T> int sgn(T val) {
	return (val > T(0)) - (val < T(0));
}

inline double sym_round(double r) {
	return (r > 0.0) ? floor(r + 0.5) : ceil(r - 0.5);
}

inline double clampf(float d,float min, float max) {
	if (d>max) d = max;
	if (d<min) d = min;
	return d;
}

inline int clamp(int d,int min, int max) {
	if (d>max) d = max;
	if (d<min) d = min;
	return d;
}

inline bool IsNumber(double x) {
	// This looks like it should always be true, 
	// but it's false if x is a NaN.
	return (x == x); 
}

inline bool isFiniteNumber(double x) 
{
	return (x <= DBL_MAX && x >= -DBL_MAX); 
}

#endif /* MISC_MATH_H_ */
