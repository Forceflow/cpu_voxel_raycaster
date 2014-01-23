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

using namespace trimesh;

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

inline RenderContext::RenderContext() : camera(NULL), frustrum(NULL), n_x(0), n_y(0){
}

inline RenderContext::RenderContext(Camera const* c, Frustrum const* f, int resolution_x, int resolution_y)
: camera(c), frustrum(f), n_x(resolution_x), n_y(resolution_y) {
}

// Get the screenpoint s of the pixel on the frustrum (in world coordinates)
// This is what a projection matrix normally does

inline vec3 RenderContext::getPixelCoordinate(int i, int j) const{
	// compute u,v,w
	float u_s = frustrum->l + (frustrum->r - frustrum->l)*((i + 0.5f) / n_x);
	float v_s = frustrum->b + (frustrum->t - frustrum->b)*((j + 0.5f) / n_y);
	float w_s = frustrum->n;
	// get screenpoint s in world coordinates
	return camera->e_ + ((u_s*camera->u_) + (v_s*camera->v_) + (w_s*camera->w_));
}

inline RenderContext::~RenderContext() {
	// TODO Auto-generated destructor stub
}

// Get the ray from the Rendercontext's camera to the given pixel
inline Ray RenderContext::getRayForPixel(int i, int j) const {
	vec3 s = this->getPixelCoordinate(i,j);
	return Ray(camera->e_,(s - camera->e_));
}

#endif /* RENDERCONTEXT_H_ */
