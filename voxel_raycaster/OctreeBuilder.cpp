#include "OctreeBuilder.h"

using namespace std;

OctreeBuilder::OctreeBuilder(size_t gridlength, size_t nfilled) : currentdatapos(0), current_morton(0), finalized(false){
	// Allocate data for voxel data and octree
	cout << "  allocating " << (nfilled*sizeof(DataPoint))/1024.0f/1024.0f << " MB of memory for voxel data ... "; cout.flush();
	DataPoint* leafdata = new DataPoint[nfilled];
	cout << "Done." << endl;

	// Create octree
	octree = new Octree(vec3(0,0,0),vec3(1,1,1),vec3(1,1,1),gridlength);
	octree->leafdata = leafdata;
	octree->n_leafnodes = nfilled;

	// Make sure we have enough buffers
	maxdepth = log2(gridlength);
	buffers.resize(maxdepth+1);

	// calculate max morton code
	max_morton = mortonEncode(gridlength-1,gridlength-1,gridlength-1);
}

void OctreeBuilder::finalizeOctree(){
	if(finalized){return;}
	// padding to get full tree
	if(current_morton < max_morton){
		for(uint64_t i = current_morton; i <= max_morton; i++){
			addDataPoint(i, DataPoint());
		}
	}
	// store root node
	octree->storeNode(buffers[0][0]);
	buffers.clear();

	// print statistics
	cout << "  octree contains " << octree->nodes.size() << " nodes" << endl;
	cout << "  accounts for " << octree->nodes.size()*sizeof(Node)/1024.0f/1024.0f << " MB of octree data" << endl;
	finalized = true;
}

Octree* OctreeBuilder::getOctree(){
	finalizeOctree();
	return octree;
}

void OctreeBuilder::addDataPoint(uint64_t morton_number, DataPoint point){
	// PADDING FOR MISSED MORTON NUMBERS
	if(morton_number != current_morton){
		for(uint64_t i = current_morton; i < morton_number; i++){
			addDataPoint(i, DataPoint());
		}
	}

	// ADD NODE TO BUFFER, STORE DATA IF FILLED
	Node n = Node(); // create empty node
	if(!point.isEmpty()) {
		assert(currentdatapos < octree->n_leafnodes);
		octree->leafdata[currentdatapos] = point; // store data
		n.data = &(octree->leafdata[currentdatapos]); // store pointer to data in octree node
		currentdatapos++;
	} 
	buffers.at(maxdepth).push_back(n);

	// REFINE BUFFERS
	// check all levels (bottom up) and group 8 nodes on a higher level
	for(int d = maxdepth; d >= 0; d--){
		if(buffers[d].size() == 8){ // if we have 8 nodes
			assert(d-1 >= 0);
			if(isBufferEmpty(buffers[d])){
				buffers[d-1].push_back(Node()); // push back NULL to represent 8 empty nodes
			} else { 
				buffers[d-1].push_back(groupNodes(octree, buffers[d])); // push back parent node
			}
			buffers.at(d).clear(); // clear the 8 nodes on this level
		} else {
			break; // break the for loop: no upper levels will need changing
		}
	}

	// INCREASE CURRENT MORTON NUMBER
	current_morton++;
}

void OctreeBuilder::buildLevels(){
	this->finalizeOctree(); // we can't build levels if octree is not complete
	octree->n_nonleafnodes = octree->nodes.size() - octree->n_leafnodes;


}

void OctreeBuilder::refineNode(Node* n){
	if(n->hasData()){  // this node already has data assigned: no need to refine
		return;
	}
	for(int i = 0; i < 8; i++){ // this node has no data: need to refine
		if(n->children[i] != NULL){
			refineNode(octree->getNode(n->children[i]));
		}
	}
	// create new datapoint
	DataPoint d = DataPoint();
	
}