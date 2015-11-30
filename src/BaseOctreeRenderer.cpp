#include "BaseOctreeRenderer.h"

using namespace std;

BaseOctreeRenderer::BaseOctreeRenderer(void) : Renderer("base")
{
}

void BaseOctreeRenderer::Render(const RenderContext& rc, const Octree const* tree, unsigned char* texture_array) const{
	// Get the number of processors in this system
	int iCPU = omp_get_num_procs();
	omp_set_num_threads(iCPU);
	// declare variables we use in loop
	int x, index, partindex;
	TreeTraverser t;
	DataPoint* v;

#pragma omp parallel for private(x,t,v,index)
	for(int y = 0; y < rc.n_y; y++){
		partindex = y*(rc.n_y*4);
		for(x = 0; x < rc.n_y; x++) {
			index = partindex + x*4; // index in char array computation (part 2)
			t = TreeTraverser(tree,rc.getRayForPixel(x,y));
			while((!t.isTerminated())){
				if(t.getCurrentNode()->isLeaf()){
					if(t.getCurrentNode()->hasData()){
						texture_array[index] = (unsigned char) int(tree->data[t.getCurrentNode()->data].color[0] * 255.0f);
						texture_array[index+1] = (unsigned char) int(tree->data[t.getCurrentNode()->data].color[1] * 255.0f);
						texture_array[index+2] = (unsigned char) int(tree->data[t.getCurrentNode()->data].color[2] * 255.0f);
						texture_array[index+3] = (unsigned char) 1;
						break;
					}
				}
				t.step();
			}
		}
	}
}

BaseOctreeRenderer::~BaseOctreeRenderer(void)
{
}