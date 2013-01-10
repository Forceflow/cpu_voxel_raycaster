#include "avox_tools.h"

using namespace std;

int parseAvoxHeader(ifstream& input, int& version, size_t& gridlength){
	string line; input >> line;  // #binvox
	if (line.compare("#avox") != 0) {cout << "  Error: first line reads [" << line << "] instead of [#avox]" << endl; return 0;}
	input >> version;

	size_t depth, height, width; // should all be equal - binvox only supports cubic grids
	depth = -1;
	bool done = false;

	while(input.good() && !done) {
		input >> line;
		if (line.compare("data") == 0) done = true; // when we encounter data keyword, we're at the end of the ASCII header
		else if (line.compare("dim") == 0) {input >> depth >> height >> width;}
		else { cout << "  unrecognized keyword [" << line << "], skipping" << endl;
			char c;
			do { c = input.get(); } while(input.good() && (c != '\n'));
		}
	}
	if (!done) {cout << "  error reading header" << endl; return 0;}
	if (depth == -1) {cout << "  missing dimensions in header" << endl; return 0;}
	if((depth != height) || (depth != width) || (width != height)){
		cout << "  not a cubic grid" << endl; return 0;
	}
	if(!isPowerOf2(depth)){
		cout << "  grid length is not a power of 2" << endl; return 0;
	}

	gridlength = depth;
	return 1;
}

int readAvoxFile(std::string const filename, size_t& _gridlength, VoxelData& _data){
	Timer t = Timer();
	cout << "Reading " << filename << " ..." << endl; cout.flush();
	ifstream input = ifstream(filename.c_str(), ios::in); // open filestream for input
	int version;
	if(parseAvoxHeader(input,version,_gridlength) == 0){return 0;};
	cout << "  avox version: " << version << endl;
	cout << "  grid size: "<< _gridlength << "x" << _gridlength << "x" << _gridlength << " voxels" << endl;

	size_t gridsize = _gridlength * _gridlength * _gridlength; // this the maximum index we can go to (worst case scenario)
	cout << "  allocating " << (gridsize*sizeof(DataPoint))/1024/1024 << " MB of memory for voxel data ... "; cout.flush();
	_data = VoxelData(_gridlength,_gridlength,_gridlength);
	for(size_t x = 0; x<_gridlength; x++){
		for(size_t y = 0; y<_gridlength; y++){
			for(size_t z = 0; z<_gridlength; z++){
				_data(x,y,z) = DataPoint();
			}
		}
	}
	cout << "Done." << endl;

	size_t x,y,z;
	float nx,ny,nz;
	cout << "  reading annotated voxel data ... "; cout.flush();
	while(input.good()){
		input >> x >> y >> z >> nx >> ny >> nz;
		size_t rx = x;
		size_t ry = y;
		size_t rz = z;
		//nz = -nz;
		float r = (float(rx)/_gridlength)*50;
		float g = (float(ry)/_gridlength)*50;
		float b = (float(rz)/_gridlength)*50;
		//float r = (float) (rand() % 150) + 1;
		//float g = (float) (rand() % 150) + 1;
		//float b = (float) (rand() % 150) + 1;
		DataPoint s = DataPoint(1,vec3(r,g,b),vec3(nx,ny,nz));
		_data(rx,ry,rz) = s;
	}
	cout << "Done." << endl;
	cout << "  finished in " << t.getTimeMilliseconds() << " ms." << endl;
	return 1;
}