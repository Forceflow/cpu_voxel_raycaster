#include "octree_io.h"

using namespace std;

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
		// write dat apointer
		nodefile.write(reinterpret_cast<char*> (& octree->nodes[i].data),sizeof(size_t));
	}
	nodefile.close();
	cout << " Done." << endl;
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
	writeOctreeNodes(octree,nodefile.str());
	writeOctreeData(octree,leafdatafile.str());
	return 1;
}

