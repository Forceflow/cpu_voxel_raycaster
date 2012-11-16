#include "TreeTraverser.h"

using namespace std;

int TreeTraverser::firstNode(float tx0, float ty0, float tz0, float txm, float tym, float tzm){
	unsigned char answer = 0;	// initialize to 00000000
	// select the entry plane and set bits
	if(tx0 > ty0){
		if(tx0 > tz0){ // PLANE YZ
			if(tym < tx0) answer|=2;	// set bit at position 1
			if(tzm < tx0) answer|=1;	// set bit at position 0
			return (int) answer;
		}
	} else {
		if(ty0 > tz0){ // PLANE XZ
			if(txm < ty0) answer|=4;	// set bit at position 2
			if(tzm < ty0) answer|=1;	// set bit at position 0
			return (int) answer;
		}
	}
	// PLANE XY
	if(txm < tz0) answer|=4;	// set bit at position 2
	if(tym < tz0) answer|=2;	// set bit at position 1
	return (int) answer;
}

// perform PUSH, POP or ADVANCE
void TreeTraverser::step(){
	stepcount++;
	// if the stack is empty, we're done
	if(stack.empty()) return;

	// define some aliases to make code readable
	vec3& t0 = stack.back().t0; 
	vec3& t1 = stack.back().t1; 
	vec3& tm = stack.back().tm;

	//POP
	// if we're in a terminal node or if we visited all children of that node (next child = 8), 
	// or if the t1 values have become negative, pop the current node from the stack
	if(stack.back().nextchild == 8 || stack.back().node->isTerminal()){
		stack.pop_back();
		return;
	}

	// PUSH
	// we haven't looked at any child in this voxel yet: pick the first and push down
	if(stack.back().nextchild == -1){
		// calculate midpoint and save it in stack
		tm = 0.5f*(t0+t1);
		// calculate first node
		stack.back().nextchild = firstNode(t0[0],t0[1],t0[2],tm[0],tm[1],tm[2]);
	}

	// ADVANCE
	// let's look at the next child in this voxel
	switch (stack.back().nextchild)
	{
	case 0: { 
		stack.back().nextchild = newNode(tm[0],4,tm[1],2,tm[2],1);
		if(stack.back().node->children[a] != 0){
			TraversalNodeInfo_ info = buildNodeInfo(t0[0],t0[1],t0[2],tm[0],tm[1],tm[2],octree->getNode(stack.back().node->children[a]));
			stack.push_back(info);
		} return;}
	case 1: { 
		stack.back().nextchild = newNode(tm[0],5,tm[1],3,t1[2],8);
		if(stack.back().node->children[1^a] != 0){
			TraversalNodeInfo_ info = buildNodeInfo(t0[0],t0[1],tm[2],tm[0],tm[1],t1[2],octree->getNode(stack.back().node->children[1^a]));
			stack.push_back(info);
		} return;}
	case 2: { 
		stack.back().nextchild = newNode(tm[0],6,t1[1],8,tm[2],3);
		if(stack.back().node->children[2^a] != 0){
			TraversalNodeInfo_ info = buildNodeInfo(t0[0],tm[1],t0[2],tm[0],t1[1],tm[2],octree->getNode(stack.back().node->children[2^a]));
			stack.push_back(info);
		} return;}
	case 3: { 
		stack.back().nextchild = newNode(tm[0],7,t1[1],8,t1[2],8);
		if(stack.back().node->children[3^a] != 0){
			TraversalNodeInfo_ info = buildNodeInfo(t0[0],tm[1],tm[2],tm[0],t1[1],t1[2],octree->getNode(stack.back().node->children[3^a]));
			stack.push_back(info);
		} return;}
	case 4: { 
		stack.back().nextchild = newNode(t1[0],8,tm[1],6,tm[2],5);
		if(stack.back().node->children[4^a] != 0){
			TraversalNodeInfo_ info = buildNodeInfo(tm[0],t0[1],t0[2],t1[0],tm[1],tm[2],octree->getNode(stack.back().node->children[4^a]));
			stack.push_back(info);
		} return;}
	case 5: { 
		stack.back().nextchild = newNode(t1[0],8,tm[1],7,t1[2],8);
		if(stack.back().node->children[5^a] != 0){
			TraversalNodeInfo_ info = buildNodeInfo(tm[0],t0[1],tm[2],t1[0],tm[1],t1[2],octree->getNode(stack.back().node->children[5^a]));
			stack.push_back(info);
		} return;}
	case 6: { 
		stack.back().nextchild = newNode(t1[0],8,t1[1],8,tm[2],7);
		if(stack.back().node->children[6^a] != 0){
			TraversalNodeInfo_ info = buildNodeInfo(tm[0],tm[1],t0[2],t1[0],t1[1],tm[2],octree->getNode(stack.back().node->children[6^a]));
			stack.push_back(info);
		} return;}
	case 7: { 
		stack.back().nextchild = 8;
		if(stack.back().node->children[7^a] != 0){
			TraversalNodeInfo_ info = buildNodeInfo(tm[0],tm[1],tm[2],t1[0],t1[1],t1[2],octree->getNode(stack.back().node->children[7^a]));
			stack.push_back(info);
		} return;}
	}
}

void TreeTraverser::initTraversal(){
	stepcount = 0;
	a = 0;

	original_ray = ray;

	// fixes for rays with negative direction
	if(ray.direction[0] < 0){
		ray.origin[0] = octree->size[0] - ray.origin[0];
		ray.direction[0] = - ray.direction[0];
		a |= 4 ; //bitwise OR (latest bits are XYZ)
	}
	if(ray.direction[1] < 0){
		ray.origin[1] = octree->size[1] - ray.origin[1];
		ray.direction[1] = - ray.direction[1];
		a |= 2 ; 
	}
	if(ray.direction[2] > 0){
		ray.origin[2] = octree->size[2] - ray.origin[2];
		ray.direction[2] = - ray.direction[2];
		a |= 1 ; 
	}

	float tx0 = (octree->min[0] - ray.origin[0]) * (1.0f / ray.direction[0]);
	float tx1 = (octree->max[0] - ray.origin[0]) * (1.0f / ray.direction[0]);
	float ty0 = (octree->min[1] - ray.origin[1]) * (1.0f / ray.direction[1]);
	float ty1 = (octree->max[1] - ray.origin[1]) * (1.0f / ray.direction[1]);
	float tz0 = (octree->min[2] - ray.origin[2]) * (1.0f / ray.direction[2]);
	float tz1 = (octree->max[2] - ray.origin[2]) * (1.0f / ray.direction[2]);

	if( max(max(tx0,ty0),tz0) < min(min(tx1,ty1),tz1) ){
		// push root node on stack
		stack.push_back(buildNodeInfo(tx0,ty0,tz0,tx1,ty1,tz1,octree->getRootNode()));
		return;
	}
	// push nothing on the stack
}

TreeTraverser::~TreeTraverser(void)
{
}