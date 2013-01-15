/*
 * RenderContext.h
 *
 *  Created on: Feb 13, 2012
 *      Author: jeroenb
 */

#ifndef RENDERCONTEXT_H_
#define RENDERCONTEXT_H_

#include "Frustrum.h"
#include "Camera.h"
#include "Ray.h"
#include "Light.h"

class RenderContext {
public:
	// class attributes
	Camera const* camera;
	Frustrum const* frustrum;
	std::vector<Light> lights;
	int n_x;
	int n_y;

	// methods
	RenderContext();
	RenderContext(Camera const* c, Frustrum const* frustrum, int resolution_x, int resolution_y);
	Ray getRayForPixel(int i, int j) const;
	vec3 getPixelCoordinate(int i, int j) const;
	virtual ~RenderContext();
};

// Get the ray from the Rendercontext's camera to the given pixel
inline Ray RenderContext::getRayForPixel(int i, int j) const {
	vec3 s = this->getPixelCoordinate(i,j);
	return Ray(camera->e_,(s - camera->e_));
}

#endif /* RENDERCONTEXT_H_ */
