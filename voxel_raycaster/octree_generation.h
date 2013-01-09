#ifndef OCTREE_GENERATION_H_
#define OCTREE_GENERATION_H_
#pragma once

#include "DataPoint.h"
#include "morton.h"
#include "Octree.h"
#include "Node.h"
#include <iostream>
#include <math.h>
#include <TriMesh.h>
#include <omp.h>
#include <algorithm>
#include "misc_math.h"

void generateOctree(size_t gridlength, VoxelData const *data, bool const *morton_array, Octree** _result);

#endif