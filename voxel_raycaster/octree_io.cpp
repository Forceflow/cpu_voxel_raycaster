#include "octree_io.h"

using namespace std;


int readOctreeData(Octree* octree, std::string filename){
	cout << "  reading octree leaf data from " << filename << " ... " << endl;
	ifstream datafile;
	datafile.open(filename.c_str(), ios::in | ios::binary);

	//allocate place for data
	cout << "    allocating " << (octree->n_leafnodes*sizeof(DataPoint))/1024.0f/1024.0f << " MB of memory for voxel data ... "; cout.flush();
	octree->leafdata = new DataPoint[octree->n_leafnodes];
	cout << "Done." << endl;

	// read data
	for(size_t i = 0; i< octree->n_leafnodes; i++){
		DataPoint d = DataPoint();
		
		datafile.read(reinterpret_cast<char*> (&d.opacity), sizeof(float));
		datafile.read(reinterpret_cast<char*> (&d.color[0]), sizeof(float));
		datafile.read(reinterpret_cast<char*> (&d.color[1]), sizeof(float));
		datafile.read(reinterpret_cast<char*> (&d.color[2]), sizeof(float));
		datafile.read(reinterpret_cast<char*> (&d.normal[0]), sizeof(float));
		datafile.read(reinterpret_cast<char*> (&d.normal[1]), sizeof(float));
		datafile.read(reinterpret_cast<char*> (&d.normal[2]), sizeof(float));

		// Store datapoint
		octree->leafdata[i] = d;
	}
	datafile.close();
	return 1;
}

int writeOctreeData(Octree* octree, std::string filename){
	cout << "  writing octree leaf data to " << filename << " ... ";
	ofstream datafile;
	datafile.open(filename.c_str(), ios::out|ios::binary);

	for(size_t i = 0; i < octree->n_leafnodes; i++){
		datafile.write(reinterpret_cast<char*> (& octree->leafdata[i].opacity),sizeof(float));
		datafile.write(reinterpret_cast<char*> (& octree->leafdata[i].color[0]),sizeof(float));
		datafile.write(reinterpret_cast<char*> (& octree->leafdata[i].color[1]),sizeof(float));
		datafile.write(reinterpret_cast<char*> (& octree->leafdata[i].color[2]),sizeof(float));
		datafile.write(reinterpret_cast<char*> (& octree->leafdata[i].normal[0]),sizeof(float));
		datafile.write(reinterpret_cast<char*> (& octree->leafdata[i].normal[1]),sizeof(float));
		datafile.write(reinterpret_cast<char*> (& octree->leafdata[i].normal[2]),sizeof(float));
	}
	cout << " Done." << endl;
	return 1;
}

int readOctreeNodes(Octree* octree, std::string filename){
	cout << "  reading octree nodes from " << filename << " ... " << endl;
	ifstream nodefile;
	nodefile.open(filename.c_str(), ios::in | ios::binary);

	for(size_t i = 0; i< octree->n_leafnodes + octree->n_nonleafnodes; i++){
		Node n = Node();

		// read children
		nodefile.read(reinterpret_cast<char*> (& (n.children[0])), sizeof(size_t));
		nodefile.read(reinterpret_cast<char*> (& (n.children[1])), sizeof(size_t));
		nodefile.read(reinterpret_cast<char*> (& (n.children[2])), sizeof(size_t));
		nodefile.read(reinterpret_cast<char*> (& (n.children[3])), sizeof(size_t));
		nodefile.read(reinterpret_cast<char*> (& (n.children[4])), sizeof(size_t));
		nodefile.read(reinterpret_cast<char*> (& (n.children[5])), sizeof(size_t));
		nodefile.read(reinterpret_cast<char*> (& (n.children[6])), sizeof(size_t));
		nodefile.read(reinterpret_cast<char*> (& (n.children[7])), sizeof(size_t));

		// read data pointer
		nodefile.read(reinterpret_cast<char*> (& (n.data)), sizeof(size_t));

		// add node to octree
		octree->nodes.push_back(n);
	}

	nodefile.close();
	return 1;
}

int writeOctreeNodes(Octree* octree, std::string filename){
	cout << "  writing octree nodes to " << filename << " ... ";
	ofstream nodefile;
	nodefile.open(filename.c_str(), ios::out|ios::binary);

	for(size_t i = 0; i< octree->nodes.size(); i++){
		// write children
		nodefile.write(reinterpret_cast<char*> (& octree->nodes[i].children[0]),sizeof(size_t));
		nodefile.write(reinterpret_cast<char*> (& octree->nodes[i].children[1]),sizeof(size_t));
		nodefile.write(reinterpret_cast<char*> (& octree->nodes[i].children[2]),sizeof(size_t));
		nodefile.write(reinterpret_cast<char*> (& octree->nodes[i].children[3]),sizeof(size_t));
		nodefile.write(reinterpret_cast<char*> (& octree->nodes[i].children[4]),sizeof(size_t));
		nodefile.write(reinterpret_cast<char*> (& octree->nodes[i].children[5]),sizeof(size_t));
		nodefile.write(reinterpret_cast<char*> (& octree->nodes[i].children[6]),sizeof(size_t));
		nodefile.write(reinterpret_cast<char*> (& octree->nodes[i].children[7]),sizeof(size_t));
		// write data pointer
		nodefile.write(reinterpret_cast<char*> (& octree->nodes[i].data),sizeof(size_t));
	}
	nodefile.close();
	cout << " Done." << endl;
	return 1;
}

int readOctreeHeader(Octree* octree, std::string filename){
	cout << "  reading octree header from " << filename << " ... " << endl;
	ifstream headerfile;
	headerfile.open(filename.c_str(), ios::in);

	string line; int version; bool done = false;
	headerfile >> line >> version;
	if (line.compare("#octreeheader") != 0) {cout << "    Error: first line reads [" << line << "] instead of [#mavox]" << endl; return 0;}
	cout << "    headerfile version: " << version << endl;

	while(headerfile.good() && !done) {
		headerfile >> line;
		if (line.compare("end") == 0) done = true; // when we encounter data keyword, we're at the end of the ASCII header
		else if (line.compare("min") == 0) {headerfile >> octree->min[0] >> octree->min[1] >> octree->min[2];}
		else if (line.compare("max") == 0) {headerfile >> octree->max[0] >> octree->max[1] >> octree->max[2];}
		else if (line.compare("size") == 0) {headerfile >> octree->size[0] >> octree->size[1] >> octree->size[2];}
		else if (line.compare("gridlength") == 0) {headerfile >> octree->gridlength;}
		else if (line.compare("n_leafnodes") == 0) {headerfile >> octree->n_leafnodes;}
		else if (line.compare("n_nonleafnodes") == 0) {headerfile >> octree->n_nonleafnodes;}
		else { cout << "  unrecognized keyword [" << line << "], skipping" << endl;
			char c; do { c = headerfile.get(); } while(headerfile.good() && (c != '\n'));
		}
	}

	headerfile.close();
	cout << "    grid size: "<< octree->gridlength << "x" << octree->gridlength << "x" << octree->gridlength << " voxels" << endl;
	double percentfilled = ((double) octree->n_leafnodes / (double) (octree->gridlength*octree->gridlength*octree->gridlength))*100.0f;
	cout << "    filled voxels: "<< octree->n_leafnodes << " (" << percentfilled << " %)" << endl;
	return 1;
}

int writeOctreeHeader(Octree* octree, std::string filename){
	cout << "  writing octree header to " << filename << " ... ";
	ofstream headerfile;
	headerfile.open(filename.c_str(), ios::out);

	headerfile << "#octreeheader 1" << endl;
	headerfile << "min " << octree->min[0] << " " << octree->min[1] << " " << octree->min[2] << endl;
	headerfile << "max " << octree->max[0] << " " << octree->max[1] << " " << octree->max[2] << endl;
	headerfile << "size " << octree->size[0] << " " << octree->size[1] << " " << octree->size[2] << endl;
	headerfile << "gridlength " << octree->gridlength << endl;
	headerfile << "n_leafnodes " << octree->n_leafnodes << endl;
	headerfile << "n_nonleafnodes " << octree->n_nonleafnodes << endl;

	headerfile << "end" << endl;

	headerfile.close();
	cout << " Done." << endl;
	return 1;
}

int readOctree(std::string basefilename, Octree*& octree){
	cout << "Reading octree from cache..." << endl;

	// compute input
	size_t splitpoint = basefilename.find_last_of(".");
	stringstream octreecachefile, nodefile, leafdatafile;
	octreecachefile << basefilename.substr(0,splitpoint) << ".octreecache";
	nodefile << basefilename.substr(0,splitpoint) << ".octreenodes";
	leafdatafile << basefilename.substr(0,splitpoint) << ".octreedata";

	// start reading octree
	octree = new Octree(); // create empty octree
	
	// read header and print statistics
	readOctreeHeader(octree,octreecachefile.str());
	readOctreeNodes(octree,nodefile.str());
	readOctreeData(octree,leafdatafile.str());

	return 1;
}

int writeOctree(std::string basefilename, Octree* octree){
	cout << "Writing octree to cache..." << endl;
	// compute outputfiles
	size_t splitpoint = basefilename.find_last_of(".");
	stringstream octreecachefile, nodefile, leafdatafile;
	octreecachefile << basefilename.substr(0,splitpoint) << ".octreecache";
	nodefile << basefilename.substr(0,splitpoint) << ".octreenodes";
	leafdatafile << basefilename.substr(0,splitpoint) << ".octreedata";

	//write octree parts
	writeOctreeHeader(octree,octreecachefile.str());
	writeOctreeNodes(octree,nodefile.str());
	writeOctreeData(octree,leafdatafile.str());
	return 1;
}

