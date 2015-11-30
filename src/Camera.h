/*
 * Camera.h
 *
 *  Created on: Feb 7, 2012
 *      Author: jeroenb
 */

#ifndef CAMERA_H_
#define CAMERA_H_

#include <TriMesh.h>
#include <iostream>
#include "string.h"
#include "util.h"

using namespace trimesh;

struct Camera {
	vec3 e_,g_,t_,u_,v_,w_;

	Camera() : e_(vec3(0, 0, 0)), g_(vec3(0, 0, -1)), t_(vec3(0, 1, 0)) {
		this->computeUVW();
	}

	Camera(vec3 e, vec3 g, vec3 t) : e_(e), g_(g), t_(t) {
		this->computeUVW();
	}

	void computeUVW(){
		vec3 w = normalize(g_) * -1.0f;
		vec3 u = normalize((vec3)(t_ CROSS w));
		vec3 v = (w CROSS u);
		w_ = w; u_ = u; v_ = v;
	}

};

#endif /* CAMERA_H_ */
