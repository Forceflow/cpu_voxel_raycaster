
/*
 * util.cpp
 *
 *  Created on: Feb 15, 2012
 *      Author: jeroenb
 */

#include "util.h"

using namespace std; 


std::string getTimeString(){
	std::stringstream o;
	o << time(NULL);
	return o.str();
}

void writePPM(int w, int h, unsigned char* array, string name){
	ofstream myfile;
	string filename = name + ".ppm";
	myfile.open(filename.c_str());
	myfile << "P3" << std::endl << "#" << filename << endl;
	myfile << w << " " << h << endl;
	myfile << 255 << endl;

	for(int x = h-1; x >= 0; x--) {
		int partindex = x*(w*4); // index in char array computation (part 1)
		for(int y = 0; y < h ; y++) {
			int i = partindex + y*4;
			myfile << int(array[i]) << " " <<  int(array[i+1])  << " " <<  int(array[i+2])  << "   ";
		}
	}
	myfile.close();
}