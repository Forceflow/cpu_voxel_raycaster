#include "Octree.h"
#include <string>

using namespace std;

int writeOctree(Octree* octree, std::string basefilename){
	
}

int writeOctreeHeader(Octree* octree, std::string basefilename){

	// compute outputfile
	size_t splitpoint = basefilename.find_last_of(".");
	stringstream octreecachefile;
	octreecachefile << basefilename.substr(0,splitpoint) << ".octreecache";



}