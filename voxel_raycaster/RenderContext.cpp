/*
 * RenderContext.cpp
 *
 *  Created on: Feb 13, 2012
 *      Author: jeroenb
 */

#include "RenderContext.h"

RenderContext::RenderContext(): camera(NULL), frustrum(NULL), n_x(0), n_y(0){
}

RenderContext::RenderContext(Camera const* c, Frustrum const* f, int resolution_x, int resolution_y)
: camera(c), frustrum(f), n_x(resolution_x), n_y(resolution_y) {
}

// Get the screenpoint s of the pixel on the frustrum (in world coordinates)
vec3 RenderContext::getPixelCoordinate(int i, int j) const{
	// compute u,v,w
	float u_s = frustrum->l + (frustrum->r - frustrum->l)*((i+0.5f)/n_x);
	float v_s = frustrum->b + (frustrum->t - frustrum->b)*((j+0.5f)/n_y);
	float w_s = frustrum->n;
	// get screenpoint s in world coordinates
	return camera->e_ + ((u_s*camera->u_) + (v_s*camera->v_) + (w_s*camera->w_));
}

RenderContext::~RenderContext() {
	// TODO Auto-generated destructor stub
}

