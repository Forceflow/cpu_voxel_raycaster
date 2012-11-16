#ifndef RENDERER_H_
#define RENDERER_H_

#include <string>
#include <iostream>
#include <vector>
#include <omp.h>
#include "time.h"
#include <TriMesh.h>
#include "RenderContext.h"
#include "DataPoint.h"
#include "intersection.h"
#include "Octree.h"

class Renderer
{
public:
	virtual void Render(RenderContext const& rc,  Octree const* tree, unsigned char* texture_array) const = 0;
	virtual std::string getName() const = 0;
};

#endif /* RENDERER_H_ */