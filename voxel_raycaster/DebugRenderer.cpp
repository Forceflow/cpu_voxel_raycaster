#include "DebugRenderer.h"


DebugRenderer::DebugRenderer(void) : Renderer ("debug")
{
}

using namespace std;

void DebugRenderer::Render(const RenderContext& rc, const Octree const* tree, unsigned char* texture_array) const{
	// Get the number of processors in this system
	int iCPU = omp_get_num_procs();
	omp_set_num_threads(iCPU);
	// declare variables we use in loop
	int x, index, partindex;

#pragma omp parallel for private(x,index)
	for(int y = 0; y < rc.n_y; y++){
		partindex = y*(rc.n_y*4);
		for(x = 0; x < rc.n_y; x++) {
			index = partindex + x*4; // index in char array computation (part 2)
				texture_array[index] = (unsigned char) x;
				texture_array[index+1] = (unsigned char) y;
				texture_array[index+2] = (unsigned char) 0;
		}
	}
}

void DebugRenderer::RenderWork(const RenderContext& rc,const Octree const* tree, unsigned char* texture_array) const{
	//
}

DebugRenderer::~DebugRenderer(void)
{
}
