#ifndef OCTREE_IO_H_
#define OCTREE_IO_H_
#pragma once

#include "Octree.h"
#include <string>
#include <string>
#include <fstream>
#include <iostream>

using namespace std;

int writeOctree(Octree* octree, std::string basefilename);

#endif /* OCTREE_H_ */