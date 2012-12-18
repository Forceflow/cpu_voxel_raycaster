#ifndef OCTREEBUILDER_H_
#define OCTREEBUILDER_H_
#pragma once

#include "Node.h"
#include <vector>
#include "morton.h"
#include "Octree.h"

using namespace std;

class OctreeBuilder
{
public:
	OctreeBuilder(size_t gridlength, size_t nfilled);
	void addDataPoint(uint64_t morton_number, DataPoint x);
	void buildLevels();
	void finalizeOctree();
	Octree* getOctree();

private:
	// octree
	Octree* octree;
	bool finalized;
	
	// buffer info
	int maxdepth;
	vector< vector< Node > > buffers;
	
	// morton numbers
	uint64_t current_morton;
	uint64_t max_morton;

	// voxel data position
	size_t currentdatapos;
	 
	// helper functions
	void refineNode(Node* n);
	bool isBufferEmpty(const vector<Node> &buffer);
	Node groupNodes(Octree* _octree, const vector<Node> &buffer);

};

inline bool OctreeBuilder::isBufferEmpty(const vector<Node> &buffer){
	for(int k = 0; k<buffer.size(); k++){
		if(! buffer[k].isNull()){ return false;}
	}
	return true;
}

inline Node OctreeBuilder::groupNodes(Octree* _octree, const vector<Node> &buffer){
	Node parent = Node();
	for(int k = 0; k<8; k++){
		if(!buffer[k].isNull()){
			parent.children[k] = _octree->storeNode(buffer[k]);
		}
		else{
			parent.children[k] = NULL;
		}
	}
	return parent;
}

#endif