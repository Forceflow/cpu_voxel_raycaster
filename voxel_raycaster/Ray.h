/*
 * Ray.h
 *
 *  Created on: Feb 14, 2012
 *      Author: jeroenb
 */

#ifndef RAY_H_
#define RAY_H_

#include <iostream>
#include <string>
#include "util.h"

class Ray {
public:

	vec3 origin;
	vec3 direction;

	// methods
	Ray();
	Ray(vec3 origin,vec3 direction);
	vec3 getRayPoint(float t) const;
	friend std::ostream& operator<< (std::ostream& o, Ray const& ray);
	virtual ~Ray();
};

inline vec3 Ray::getRayPoint(float t) const{
	return origin + (t*direction);
}

#endif /* RAY_H_ */
