#ifndef INTERSECTION_H_
#define INTERSECTION_H_

#include "AABox.h"
#include "Ray.h"
#include <omp.h>

// Ray Box intersection
bool intersectRayAABox2(const Ray &r, const AABox &b, float& tnear, float& tfar);

#endif /* INTERSECTION_H_ */

