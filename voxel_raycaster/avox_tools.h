#ifndef AVOX_TOOLS_H_
#define AVOX_TOOLS_H_

#include <string>
#include <fstream>
#include <iostream>
#include "util.h"
#include "misc_math.h"
#include <omp.h>
#include "DataPoint.h"

int readAvoxFile(std::string const filename, size_t& _gridlength, VoxelData& _data);

#endif