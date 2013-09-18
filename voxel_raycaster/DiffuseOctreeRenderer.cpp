#include "DiffuseOctreeRenderer.h"

using namespace std;

DiffuseOctreeRenderer::DiffuseOctreeRenderer(void) : Renderer("diffuse")
{
}

void DiffuseOctreeRenderer::Render(const RenderContext& rc, const Octree const* tree, unsigned char* texture_array) const{
	// Get the number of processors in this system
	int iCPU = omp_get_num_procs();
	omp_set_num_threads(iCPU);
	// declare variables we use in loop
	int x, index, partindex;
	vec3 to_light;
	float diffuse_factor,r,g,b,distancecut;
	TreeTraverser t;
	DataPoint* v;

#pragma omp parallel for private(x,t,v,index,diffuse_factor,to_light,r,g,b)
	for(int y = 0; y < rc.n_y; y++){
		partindex = y*(rc.n_y*4);
		for(x = 0; x < rc.n_y; x++) {
			index = partindex + x*4; // index in char array computation (part 2)
			t = TreeTraverser(tree,rc.getRayForPixel(x,y)); // DO A BASIC BOUNDING BOX TEST FOR THE TEST FIRST
			while((!t.isTerminated())){
				if(t.getCurrentNode()->isLeaf() && t.getCurrentNode()->hasData() && t.stack.back().t0.max() >0){
					
					r=0.0f;
					g=0.0f;
					b=0.0f;
					for(int i = 0; i<rc.lights.size(); i++){
						if(rc.lights[i].active){
							to_light = rc.lights[i].position - t.getCurrentPosition();
							vec3 s = to_light + (rc.camera->e_ - t.getCurrentPosition());
							s = normalize(s);

							distancecut = 1.0f /(
								rc.lights[i].CONSTANT_ATTENUATION +
								rc.lights[i].LINEAR_ATTENUATION*len(to_light) +
								rc.lights[i].QUADRATIC_ATTENUATION*len2(to_light));

							to_light = normalize(to_light);

							// Diffuse
							diffuse_factor = tree->data[t.getCurrentNode()->data].normal DOT to_light;
							r +=  std::max(0.0f,diffuse_factor) * distancecut * rc.lights[i].diffuse[0];
							g +=  std::max(0.0f,diffuse_factor) * distancecut * rc.lights[i].diffuse[1];
							b +=  std::max(0.0f,diffuse_factor) * distancecut * rc.lights[i].diffuse[2];

							// Phong
							float phong_factor = pow(to_light DOT tree->data[t.getCurrentNode()->data].normal, rc.lights[i].SHININESS);
							r +=  std::max(0.0f,phong_factor) * distancecut * rc.lights[i].specular[0];
							g +=  std::max(0.0f,phong_factor) * distancecut * rc.lights[i].specular[1];
							b +=  std::max(0.0f,phong_factor) * distancecut * rc.lights[i].specular[2];

						}
					}
					texture_array[index] = (unsigned char) clampf(255*(tree->data[t.getCurrentNode()->data].color[0] * (r +0.25)),0,255);
					texture_array[index+1] = (unsigned char) clampf (255*(tree->data[t.getCurrentNode()->data].color[1] * (g +0.25)),0,255);
					texture_array[index+2] = (unsigned char) clampf (255*(tree->data[t.getCurrentNode()->data].color[2] * (b +0.25)),0,255);
					texture_array[index+3] = (unsigned char) 1;
					break;
				}
				t.step();
			}
		}
	}
}

DiffuseOctreeRenderer::~DiffuseOctreeRenderer(void)
{
}
