#include "NormalRenderer.h"

using namespace std;

NormalRenderer::NormalRenderer(void) : Renderer("normal")
{
}

void NormalRenderer::Render(const RenderContext& rc, const Octree const* tree, unsigned char* texture_array) const{
	// Get the number of processors in this system
	int iCPU = omp_get_num_procs();
	omp_set_num_threads(iCPU);
	// declare variables we use in loop
	int x, index, partindex;
	vec3 to_light;
	float factor;
	TreeTraverser t;
	DataPoint* v;

#pragma omp parallel for private(x,t,v,index,factor,to_light)
	for(int y = 0; y < rc.n_y; y++){
		partindex = y*(rc.n_y*4);
		for(x = 0; x < rc.n_y; x++) {
			index = partindex + x*4; // index in char array computation (part 2)
			t = TreeTraverser(tree,rc.getRayForPixel(x,y));
			while((!t.isTerminated())){
				if(t.stack.back().t0.max()> 0.0f){
					if(t.getCurrentNode()->isLeaf()){
						if(t.getCurrentNode()->hasData()){
							vec3 normal = tree->data[t.getCurrentNode()->data].normal;
							normal = normalize(normal);
							float r = 255 * ((normal[0]+1.0f)/2.0f);
							float g = 255 * ((normal[1]+1.0f)/2.0f);
							float b = 255 * ((normal[2]+1.0f)/2.0f);

							texture_array[index] = (unsigned char) r;
							texture_array[index+1] = (unsigned char) g;
							texture_array[index+2] = (unsigned char) b;
							texture_array[index+3] = (unsigned char) 1;
							break;
						}
					}
				}
				t.step();
			}
		}
	}
}

NormalRenderer::~NormalRenderer(void)
{
}
