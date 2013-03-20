#include "octree_io.h"

using namespace std;

bool checkForOctreeCache(std::string basefilename){

	size_t splitpoint = basefilename.find_last_of(".");
	stringstream octreecachefile;
	octreecachefile << basefilename.substr(0,splitpoint) << ".octreecache";

	cout << "Checking for octree cache ... "; 

	ifstream checkfile(octreecachefile.str().c_str());
	if(checkfile.good()){
		checkfile.close();
		cout << "Exists!" << endl;
		return true;
	}
	checkfile.close();
	cout << "Not found." << endl;
	return false;
}

int readOctreeData(Octree* octree, std::string filename){
	ifstream datafile;
	datafile.open(filename.c_str(), ios::in | ios::binary);

	octree->data = new DataPoint[octree->n_data];

	// read data
	for(size_t i = 0; i< octree->n_data; i++){
		DataPoint d = DataPoint();
		datafile.read(reinterpret_cast<char*> (&d.opacity), sizeof(float));
		datafile.read(reinterpret_cast<char*> (&d.color[0]), sizeof(float));
		datafile.read(reinterpret_cast<char*> (&d.color[1]), sizeof(float));
		datafile.read(reinterpret_cast<char*> (&d.color[2]), sizeof(float));
		datafile.read(reinterpret_cast<char*> (&d.normal[0]), sizeof(float));
		datafile.read(reinterpret_cast<char*> (&d.normal[1]), sizeof(float));
		datafile.read(reinterpret_cast<char*> (&d.normal[2]), sizeof(float));
		// Store datapoint
		octree->data[i] = d;
	}
	datafile.close();
	return 1;
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

	// start reading octree
	octree = new Octree(); // create empty octree
	
	// read header and print statistics
	readOctreeHeader(octree,octreecachefile.str());
	readOctreeNodes(octree,nodefile.str());
	readOctreeData(octree, datafile.str());

	return 1;
}

