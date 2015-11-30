#ifndef TREETRAVERSER_H_
#define TREETRAVERSER_H_

#pragma once
#include "Octree.h"
#include <vector>
#include "Ray.h"
#include "util.h"
#include <math.h>
#include <iostream>

// the struct we will use to communicate current traversal info
struct TraversalNodeInfo_ {
	const Node* node;
	vec3 t0,t1,tm;
	int nextchild;
};

class TreeTraverser
{
public:
	unsigned char a;
	Octree const* octree;
	Ray ray;
	Ray original_ray;
	std::vector<TraversalNodeInfo_> stack;
	int stepcount;

	TreeTraverser(void);
	TreeTraverser(Octree const* octree, Ray ray);
	void step();
	bool isTerminated() const;
	const Node* getCurrentNode() const;
	vec3 getCurrentPosition() const;
	~TreeTraverser(void);

private:
	TraversalNodeInfo_ buildNodeInfo(float tx0, float ty0, float tz0, float tx1, float ty1, float tz1, const Node* node);
	int newNode(float txm, int x, float tym, int y, float tzm, int z);
	int firstNode(float tx0, float ty0, float tz0, float txm, float tym, float tzm);
	void initTraversal();
};

inline TreeTraverser::TreeTraverser(void){
}

inline TreeTraverser::TreeTraverser(Octree const* octree, Ray ray): octree(octree), ray(ray){
	initTraversal();
}

inline int TreeTraverser::newNode(float txm, int x, float tym, int y, float tzm, int z){
	if(txm < tym){
		if(txm < tzm){return x;}  // YZ plane
	} else {
		if(tym < tzm){return y;} // XZ plane
	}
	return z; // XY plane;
}

inline TraversalNodeInfo_ TreeTraverser::buildNodeInfo(float tx0, float ty0, float tz0, float tx1, float ty1, float tz1, const Node* node){
	TraversalNodeInfo_ info;
	info.node = node;
	info.t0 = vec3(tx0,ty0,tz0);
	info.t1 = vec3(tx1,ty1,tz1);
	info.nextchild = -1;
	return info;
}

inline bool TreeTraverser::isTerminated() const{
	return (stack.empty());
}

inline vec3 TreeTraverser::getCurrentPosition() const{
	float t = stack.back().t0.max();
	// TODO: better implementation, use a.
	vec3 answer = original_ray.getRayPoint(t);
	answer[2] = -answer[2];
	return answer;
}

inline const Node* TreeTraverser::getCurrentNode() const{
	return stack.back().node;
}

#endif
