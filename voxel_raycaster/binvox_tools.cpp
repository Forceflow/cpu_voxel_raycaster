#include "binvox_tools.h"

using namespace std;

int parseBinvoxHeader(ifstream& input, int& version, size_t& _gridlength){
	string line; input >> line;  // #binvox
	if (line.compare("#binvox") != 0) {cout << "  Error: first line reads [" << line << "] instead of [#binvox]" << endl; return 0;}
	input >> version;
	cout << "  Binvox version: " << version << endl;
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

	_gridlength = depth;
	return 1;
}

// Read a binvox file into a byte array of voxels, and store the length of this array
int readBinvoxFile(string const filename, size_t& _gridlength, VoxelData& _data){
	Timer t = Timer();
	// PART 1: reading the ASCII header
	cout << "Reading " << filename << " ..." << endl;
	ifstream input = ifstream(filename.c_str(), ios::in | ios::binary); // open filestream for input, and in binary

	int version;
	if(parseBinvoxHeader(input,version,_gridlength) == 0){ return 0; };

	// PART 2: reading binary voxel data
	size_t gridsize = _gridlength * _gridlength * _gridlength; // this the maximum index we can go to (worst case scenario)
	cout << "  allocating " << (gridsize*sizeof(DataPoint))/1024/1024 << " MB of memory for voxel data ... ";
	_data = VoxelData(_gridlength,_gridlength,_gridlength);
	for(size_t x = 0; x<_gridlength; x++){
		for(size_t y = 0; y<_gridlength; y++){
			for(size_t z = 0; z<_gridlength; z++){
				_data(x,y,z) = DataPoint();
			}
		}
	}
	cout << "Done." << endl;
	byte value, count;
	size_t index = 0; size_t end_index = 0; size_t voxels_filled = 0;
	size_t bytesread = 0;

	input.unsetf(ios::skipws);  // need to read every byte now
	input >> value;  // read the linefeed char

	CoordCounter counter = CoordCounter(_gridlength);

	while((end_index < gridsize) && input.good()) {
		input >> value >> count;
		bytesread+=2;
		if (input.good()) {
			end_index = index + count; // read value (first byte) and count (second byte) pair
			if (end_index > gridsize) {
				cout << "  error: index exceeds binary data size: incorrect binvox dimension header?" << endl; 
			}
			for(size_t i=index; i < end_index; i++) {
				size_t x = counter.x;
				size_t y = counter.z; // because in binvox, y and z are switched
				size_t z = counter.y; // because in binvox z axis is switched
				if(value == 1){
						 float r = 255 - ((float(x)/_gridlength)*255);
						 float g = 255 - ((float(y)/_gridlength)*255);
						 float b = 255 - ((float(z)/_gridlength)*255);
						_data(x,y,z) = DataPoint(1, RGBColor(r,g,b));
				} else {
					_data(x,y,z) = DataPoint();
				}
				counter.count();
			}
			if (value) { voxels_filled += count;}
			index = end_index;
		}
	}
	input.close();
	// write statistics
	float filledpercent = ((float)voxels_filled / (float)end_index)*100;
	float compression = ((float)bytesread / (float)end_index)*100.0; // how much we read divided by one byte for every voxel
	cout << "  read " << end_index << " voxels (" << voxels_filled << " filled, " << filledpercent 
		 << "%) which form a " << _gridlength << "x" << _gridlength << "x" << _gridlength << " grid" << endl;
	cout << "  read " << bytesread << " bytes of RLE-encoded voxel data (compression " << compression << "%)" << endl;

	cout << "  finished in " << t.getTimeMilliseconds() << " ms." << endl;
	return 1;
}