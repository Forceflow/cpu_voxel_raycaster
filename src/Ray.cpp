/*
 * Ray.cpp
 *
 *  Created on: Feb 14, 2012
 *      Author: jeroenb
 */

#include "Ray.h"


Ray::Ray(): origin(vec3(0,0,0)), direction(vec3(0,0,-1)){
}

Ray::Ray(vec3 origin, vec3 direction) : origin(origin), direction(direction){
}

//std::ostream& operator<< (std::ostream& o, Ray const& ray){
//	return o << "#RAY : " << std::endl
//			<< "Origin : " << Vector2String(ray.origin) << std::endl
//			<< "Direction    : " << Vector2String(ray.direction) << std:: endl;
//}

Ray::~Ray() {
	// TODO Auto-generated destructor stub
}

