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

class Camera {
public:
	vec3 e_,g_,t_,u_,v_,w_;
	Camera();
	Camera(vec3 e, vec3 g, vec3 t);
	//Eigen::Matrix4d getViewTransformation();
	friend std::ostream& operator<< (std::ostream& o, Camera const& camera);
	virtual ~Camera();
	void computeUVW(vec3 e,vec3 g, vec3 t);
};

// Construct default camera at origin
inline Camera::Camera() : e_(vec3(0,0,0)) , g_(vec3(0,0,-1)), t_(vec3(0,1,0)) {
	computeUVW(e_,g_,t_);
}

// Construct a camera using, eye e, gaze g (the view direction), and view-up vector t (which way is up);
inline Camera::Camera(vec3 e, vec3 g, vec3 t) : e_(e), g_(g), t_(t) {
	this->computeUVW(e_,g_,t_);
}

// Private method to compute camera basis in UVW coordinates
// This is like constructing the view matrix
inline void Camera::computeUVW(vec3 e, vec3 g, vec3 t){
	vec3 w = normalize(g) * -1.0f;
	vec3 u = normalize((vec3) (t CROSS w));
	vec3 v = (w CROSS u);
	w_ = w; u_ = u; v_ = v;
}

inline Camera::~Camera() {
	// TODO Auto-generated destructor stub
}

#endif /* CAMERA_H_ */
