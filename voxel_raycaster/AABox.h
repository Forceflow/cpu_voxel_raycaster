/*
 * Box.h
 *
 *  Created on: Feb 15, 2012
 *      Author: jeroenb
 */

#ifndef BOX_H_
#define BOX_H_

#include <iostream>
#include <algorithm>
#include <TriMesh.h>
#include "util.h"
#include "string.h"

using namespace trimesh;

class AABox {
public:
	//variables
	vec3 min;
	vec3 max;

	// functions
	AABox();
	AABox(vec3 min, vec3 max);
	virtual ~AABox();
};

inline AABox::AABox() : min(vec3(0,0,0)), max(vec3(0.0,0.0,0.0)) {
	// TODO Auto-generated constructor stub
}

inline AABox::AABox(vec3 min, vec3 max): min(min), max(max) {
	if(len(min) > len(max)){
		this->max = min;
		this->min = max;
	}
}

inline AABox::~AABox() {
	// TODO Auto-generated destructor stub
}

#endif /* BOX_H_ */
