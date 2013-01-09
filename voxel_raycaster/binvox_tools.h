#ifndef BINVOX_TOOLS_H_
#define BINVOX_TOOLS_H_

#include <string>
#include <fstream>
#include <iostream>
#include "DataPoint.h"
#include "misc_math.h"
#include <time.h>
#include <omp.h>

typedef unsigned char byte;

int readBinvoxFile(std::string const filename, size_t& _gridlength, VoxelData& _data);

#endif /* BINVOX_TOOLS_H_ */