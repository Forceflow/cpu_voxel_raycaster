#include "octree_io.h"

using namespace std;

int writeOctreeNodes(Octree* octree, std::string filename){
	cout << "  writing octree nodes to " << filename << " ... ";

	return 1;
}

int writeOctreeHeader(Octree* octree, std::string filename){
	cout << "  writing octree header to " << filename << " ... ";
	ofstream headerfile;
	headerfile.open(filename.c_str());

	headerfile << "#octreeheader" << endl;
	headerfile << "min " << octree->min[0] << " " << octree->min[1] << " " << octree->min[2] << endl;
	headerfile << "max " << octree->max[0] << " " << octree->max[1] << " " << octree->max[2] << endl;
	headerfile << "size " << octree->size[0] << " " << octree->size[1] << " " << octree->size[2] << endl;
	headerfile << "gridlength " << octree->gridlength << endl;
	headerfile << "n_leafnodes " << octree->n_leafnodes << endl;
	headerfile << "n_nonleafnodes " << octree->n_nonleafnodes << endl;

	headerfile.close();
	cout << " Done." << endl;
	return 1;
}

int writeOctree(Octree* octree, std::string basefilename){
	cout << "Writing octree to cache..." << endl;
	// compute outputfiles
	size_t splitpoint = basefilename.find_last_of(".");
	stringstream octreecachefile, nodefile, leafdatafile;
	octreecachefile << basefilename.substr(0,splitpoint) << ".octreecache";
	nodefile << basefilename.substr(0,splitpoint) << ".octreenodes";
	leafdatafile << basefilename.substr(0,splitpoint) << ".octreedata";

	//write octree parts
	writeOctreeHeader(octree,octreecachefile.str());
	//writeOctreeNodes(octree,octreecachefile.str());
	return 1;
}

