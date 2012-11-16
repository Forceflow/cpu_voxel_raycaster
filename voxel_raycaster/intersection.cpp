#include "intersection.h"

/**
* Ray - Axis Aligned box intersection, Kay/Kayjia 1986
*/
bool intersectRayAABox2(const Ray &ray, const AABox &box, float& tnear, float& tfar)
{
	vec3 T_1, T_2; // vectors to hold the T-values for every direction
	float t_near = -FLT_MAX; // maximums defined in float.h // maxenter
	float t_far = FLT_MAX; // min
	double inv;
	for (int i = 0; i < 3; i++){ //we test slabs in every direction (0=x, 1=y, 2=z)
		if (ray.direction[i] == 0){ // ray parallel to planes in this direction
			if ((ray.origin[i] < box.min[i]) || (ray.origin[i] > box.max[i])) {
				return false; // parallel AND outside box : no intersection possible
			}
		} else { // ray not parallel to planes in this direction
			inv = 1/ray.direction[i];
			T_1[i] = (box.min[i] - ray.origin[i]) * inv;
			T_2[i] = (box.max[i] - ray.origin[i]) * inv;

			if(T_1[i] > T_2[i]) std::swap(T_1,T_2);
			if (T_1[i] > t_near) t_near = T_1[i];
			if (T_2[i] < t_far) t_far = T_2[i];

			if( (t_near > t_far) || (t_far < 0) ) return false;
		}
	}
	tnear = t_near; tfar = t_far; // put return values in place
	return true; // if we made it here, there was an intersection - YAY
}