#ifndef MAVOX_TOOLS_H_
#define MAVOX_TOOLS_H_

#include <string>
#include <fstream>
#include <iostream>
#include "util.h"
#include <omp.h>
#include "DataPoint.h"
#include "Octree.h"
#include "OctreeBuilder.h"
#include "perlin_noise.hpp"

int readMavoxFile(std::string const filename, Octree*& tree);

#endif