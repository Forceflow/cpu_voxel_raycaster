#include "octree_generation.h"

using namespace std;

bool isBufferEmpty(const vector<Node> &buffer){
	for(int k = 0; k<buffer.size(); k++){
		if(! buffer[k].isNull()){ return false;}
	}
	return true;
}

Node groupNodes(Octree** _octree, const vector<Node> &buffer){
	Node parent = Node();
	for(int k = 0; k<8; k++){
		//parent.children[k] = (*_octree)->storeNode(buffer[k]);
	}
	return parent;
}

void generateOctree(size_t gridlength,VoxelData const *data,bool const *morton_array, Octree** _result){
	cout << "Generating octree ..." << endl;
	Timer timer = Timer();

	// Statistics stuff
	int nodecount = 0;

	*_result = new Octree(data,vec3(0,0,0),vec3(1,1,1),vec3(1,1,1),gridlength);

	// create buffers for every level in the octree
	int depth = log2(gridlength);
	vector<vector<Node>> buffers;
	buffers.resize(depth+1);

	// iterate over morton array
	size_t max_index = gridlength*gridlength*gridlength;
	for(size_t i = 0; i<max_index;i++){
		Node n = Node();
		// read the next voxel and put it in the lowest buffer
		if(morton_array[i]) {
			nodecount++;
			vector<uint64_t> s = mortonDecode(i);
			//n.data = &(*data)(s[2],s[1],s[0]); // because morton encoding is z,y,x
		} 
		buffers[depth].push_back(n);

		// check all levels (bottom up) and group 8 nodes on a higher level
		for(int d = depth; d >= 0; d--){
			if(buffers[d].size() == 8){ // if we have 8 nodes
				if(isBufferEmpty(buffers[d])){ 
					buffers[d-1].push_back(Node()); // push back NULL to represent 8 empty nodes
				} else { 
					buffers[d-1].push_back(groupNodes(_result, buffers[d])); // push back parent node
					nodecount++;
				}
				buffers[d].clear(); // clear the 8 nodes on this level
			} else {
				break; // break the for loop: no upper levels will need changing
			}
		}
	}

	(*_result)->storeNode(buffers[0][0]);
	cout << "  generated in " << timer.getTimeMilliseconds() << " ms, contains " << nodecount << " nodes." << endl;
	cout << "  accounts for " << nodecount*sizeof(Node)/1024/1024 << " MB of octree data" << endl;
}

