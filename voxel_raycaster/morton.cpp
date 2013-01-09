#include "morton.h"

using namespace std;

uint64_t mortonEncode(uint64_t x, uint64_t y, uint64_t z){
	uint64_t answer = 0;
	for (uint64_t i = 0; i < (sizeof(uint64_t) * CHAR_BIT); ++i) {
		answer |= ((x & ((uint64_t)1 << i)) << 2*i) | ((y & ((uint64_t)1 << i)) << (2*i + 1)) | ((z & ((uint64_t)1 << i)) << (2*i + 2));
	}
	return answer;
}

vector<uint64_t> mortonDecode(uint64_t morton){
	vector<uint64_t> answer(3,(uint64_t) 0);
	for (uint64_t i = 0; i < (sizeof(uint64_t) * CHAR_BIT); ++i) {
		answer[0] |= ((morton & ((uint64_t) 1 << 3*i)) >> (3*i)-i);
		answer[1] |= ((morton & ((uint64_t) 1 << (3*i+1))) >> (3*i+1)-i);
		answer[2] |= ((morton & ((uint64_t) 1 << (3*i+2))) >> (3*i+2)-i);
	}
	return answer;
}

int parseMoctreeHeader(ifstream& input, int& version, size_t& gridlength){
	string line; input >> line; 
	if (line.compare("#moctree") != 0) {
		cout << "  error: first line reads [" << line << "] instead of [#moctree]" << endl; return 0;
	}
	input >> version;
	int depth, height, width;
	depth = -1;
	bool done = false;
	// Until we read a data keyword, read the ASCII header
	while(input.good() && !done) {
		input >> line;
		if (line.compare("data") == 0) {
			done = true; // when we encounter data keyword, we're at the end of the ASCII header
		} 
		else if (line.compare("dim") == 0) { 
			input >> depth >> height >> width; 
		}
		else { 
			cout << "  unrecognized keyword [" << line << "], skipping" << endl;
			char c;
			do {c = input.get();} while(input.good() && (c != '\n')); // skip to endline
		}
	}

	// Return if any of the following tests fail
	if (!done){cout << "  error reading header" << endl; return 0;}
	if (depth == -1){cout << "  missing dimensions in header" << endl; return 0;}
	if((depth != height) || (depth != width) || (width != height))
		{cout << "  not a cubic octree" << endl;return 0;}
	gridlength = depth;
	return 1;
}

bool readMoctreeFile(std::string const filename, size_t& _gridlength, bool** _morton_array){
	Timer t = Timer();
	cout << "Reading " << filename << " ..." << endl;
	ifstream input = ifstream(filename.c_str(), ios::in | ios::binary); // open filestream for input, and in binary

	// Parse header
	int version;
	if(parseMoctreeHeader(input,version,_gridlength) == 0){return 0;};
	cout << "  moctree version: " << version << endl;
	cout << "  grid size: "<< _gridlength << "x" << _gridlength << "x" << _gridlength << " voxels" << endl;

	// Allocate memory
	size_t gridsize = _gridlength*_gridlength*_gridlength;
	cout << "  allocating " << (gridsize*sizeof(bool))/1024/1024 << " MB of memory for moctree data ... "; cout.flush();
	*_morton_array = new bool[gridsize];
	memset(*_morton_array,0,gridsize*sizeof(bool));
	cout << "Done." << endl;

	// Read binary data part
	byte value,count;
	input.unsetf(ios::skipws); // also read whitespace now
	input >> value;  // read the linefeed char (TODO: necessary?)

	size_t index = 0; size_t end_index = 0; 
	size_t voxels_filled = 0; 
	size_t bytesread = 0;

	cout << "  reading moctree data ... "; cout.flush();
	while((end_index < gridsize) && input.good()) {
		bytesread+=2;
		input >> value >> count; // read value (first byte) and count (second byte) pair
		if (input.good()) {
			end_index = index + count; // value has to be writting count times
			if (end_index > gridsize) {
				cout << "  error: index exceeds binary data size: incorrect moctree header?" << endl;
				delete[] _morton_array;return 0;
			}
			for(int i=index; i < end_index; i++) {
				(*_morton_array)[i] = (value == 1); // write the value count times
			}
			if (value) {voxels_filled += count;} // statistics
			index = end_index;
		}
	}
	input.close();
	cout << "Done." << endl;

	// print statistics
	float filledpercent = ((float)voxels_filled / (float)end_index)*100;
	float compression = ((float)bytesread / (float)end_index)*100.0; // how much we read divided by one byte for every voxel
	cout << "  read " << end_index << " voxels (" << voxels_filled << " filled, " << filledpercent 
		 << "%)" << endl;
	cout << "  read " << bytesread << " bytes of RLE-encoded voxel data (compression " << compression << "%)" << endl;
	cout << "  finished in " << t.getTimeMilliseconds() << " ms." << endl;
	return 1;
}

void writeMoctreeFile(std::string const filename, size_t gridlength, bool const *morton_array){
	cout << "Writing " << filename << " ..." << endl;
	// create moctree file
	ofstream file;
	file.open(filename, ios::out|ios::binary);

	// write ASCII header
	file << "#moctree 1" << endl;
	file << "dim " << gridlength << " " << gridlength << " " << gridlength << endl;
	file << "data" << endl;

	// binary data
	char* memblock = new char [2];

	// Run length encoding of morton array
	size_t gridsize = gridlength*gridlength*gridlength;
	size_t current_index = 0;
	size_t byteswritten = 0;
	size_t voxels_filled = 0;
	while(current_index < gridsize){
		bool value = morton_array[current_index];
		int count = 0;
		while(current_index < gridsize && morton_array[current_index] == value && count < 255){
			count++;
			current_index++;
		}
		memblock[0] = value;
		memblock[1] = count;
		if(value){voxels_filled+=count;} // statistics
		byteswritten+=2;
		file.write(memblock,2);
	}

	file.close();
	// statistics
	float filledpercent = ((float)voxels_filled / (float)gridsize)*100;
	float compression = ((float)byteswritten / (float)gridsize)*100.0; // how much we read divided by one byte for every voxel
	cout << "  wrote " << gridsize << " voxels (" << voxels_filled << " filled, " << filledpercent 
		 << "%) which form a " << gridlength << "x" << gridlength << "x" << gridlength << " grid" << endl;
	cout << "  wrote " << byteswritten << " bytes of RLE-encoded voxel data (compression " << compression << "%)" << endl;
	
	delete[] memblock;
}
