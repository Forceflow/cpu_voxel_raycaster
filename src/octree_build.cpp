#include "octree_build.h"

using namespace std;

void readOctreeData(OctreeInfo const &octree_info, VoxelData** data){
	string filename = octree_info.base_filename+string(".octreedata");
	FILE* file = fopen(filename.c_str(), "rb");

	*data = new VoxelData[octree_info.n_data];

	// read data
	for(size_t i = 0; i< octree_info.n_data; i++){
		(*data)[i] = VoxelData();
		readVoxelData(file,(*data)[i]);
	}
	fclose(file);
}

void readOctreeNodes(OctreeInfo const &octree_info, std::vector<Node> &nodes){
	string filename = octree_info.base_filename+string(".octreenodes");
	FILE* file = fopen(filename.c_str(), "rb");

	nodes.reserve(octree_info.n_nodes);

	for(size_t i = 0; i< octree_info.n_nodes; i++){
		Node n = Node();
		readNode(file,n);
		nodes.push_back(n);
	}
	fclose(file);
}


int readOctreeNodes(Octree* octree, std::string filename){
	ifstream nodefile;
	nodefile.open(filename.c_str(), ios::in | ios::binary);

	for(size_t i = 0; i< octree->n_nodes; i++){
		Node n = Node();
		// read children
		nodefile.read(reinterpret_cast<char*> (& (n.children_base)), sizeof(size_t));
		nodefile.read(reinterpret_cast<char*> (& (n.children_offset[0])), sizeof(char));
		nodefile.read(reinterpret_cast<char*> (& (n.children_offset[1])), sizeof(char));
		nodefile.read(reinterpret_cast<char*> (& (n.children_offset[2])), sizeof(char));
		nodefile.read(reinterpret_cast<char*> (& (n.children_offset[3])), sizeof(char));
		nodefile.read(reinterpret_cast<char*> (& (n.children_offset[4])), sizeof(char));
		nodefile.read(reinterpret_cast<char*> (& (n.children_offset[5])), sizeof(char));
		nodefile.read(reinterpret_cast<char*> (& (n.children_offset[6])), sizeof(char));
		nodefile.read(reinterpret_cast<char*> (& (n.children_offset[7])), sizeof(char));
		// read data pointer
		nodefile.read(reinterpret_cast<char*> (& (n.data)), sizeof(size_t));
		// add node to octree
		octree->nodes.push_back(n);
	}

	nodefile.close();
	return 1;
}

int readOctreeHeader(Octree* octree, std::string filename){
	cout << "  reading octree header from " << filename << " ... " << endl;
	ifstream headerfile;
	headerfile.open(filename.c_str(), ios::in);

	string line; int version; bool done = false;
	headerfile >> line >> version;
	if (line.compare("#octreeheader") != 0) {cout << "    Error: first line reads [" << line << "] instead of [#octreeheader]" << endl; return 0;}
	cout << "    headerfile version: " << version << endl;

	while(headerfile.good() && !done) {
		headerfile >> line;
		if (line.compare("end") == 0) done = true; // when we encounter data keyword, we're at the end of the ASCII header
		else if (line.compare("gridlength") == 0) {headerfile >> octree->gridlength;}
		else if (line.compare("n_nodes") == 0) {headerfile >> octree->n_nodes;}
		else if (line.compare("n_data") == 0) {headerfile >> octree->n_data;}
		else { cout << "  unrecognized keyword [" << line << "], skipping" << endl;
			char c; do { c = headerfile.get(); } while(headerfile.good() && (c != '\n'));
		}
	}

	headerfile.close();
	return 1;
}

int readOctree(std::string basefilename, Octree*& octree){
	cout << "Reading octree from cache..." << endl;

	// compute inputfile
	size_t splitpoint = basefilename.find_last_of(".");
	stringstream octreecachefile, nodefile, datafile;
	octreecachefile << basefilename.substr(0,splitpoint) << ".octree";
	nodefile << basefilename.substr(0,splitpoint) << ".octreenodes";
	datafile << basefilename.substr(0,splitpoint) << ".octreedata";

	OctreeInfo info;
	parseOctreeHeader(basefilename, info);

	// start reading octree
	octree = new Octree(); // create empty octree
	octree->gridlength = info.gridlength;
	octree->n_data = info.n_data;
	octree->n_nodes = info.n_nodes;
	
	// read header and print statistics
	readOctreeNodes(info, octree->nodes);
	readOctreeData(info, &(octree->data));

	return 1;
}

