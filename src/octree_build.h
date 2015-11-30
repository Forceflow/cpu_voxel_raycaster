#ifndef OCTREE_BUILD_H_
#define OCTREE_BUILD_H_
#pragma once

#include "Octree.h"
#include "octree_io.h"
#include <string>
#include <fstream>
#include <iostream>

using namespace std;

int readOctree(std::string basefilename, Octree*& octree);

#endif /* OCTREE_H_ */