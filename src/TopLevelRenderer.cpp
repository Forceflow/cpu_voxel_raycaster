#include "TopLevelRenderer.h"

using namespace std;

TopLevelRenderer::TopLevelRenderer(void) : Renderer("level"), maxlevel(1)
{
}

void TopLevelRenderer::Render(const RenderContext& rc, const Octree const* tree, unsigned char* texture_array) const{
	// Get the number of processors in this system
	int iCPU = omp_get_num_procs();
	omp_set_num_threads(iCPU);
	// declare variables we use in loop
	int x, index, partindex;
	vec3 to_light;
	float diffuse_factor,r,g,b,distancecut;
	TreeTraverser t;
	DataPoint* v;

	// how many depths?
	float depth = log2(tree->gridlength)+2;
	float step = rc.n_y / depth;

#pragma omp parallel for private(x,t,v,index,diffuse_factor,to_light,r,g,b)
	for(int y = 0; y < rc.n_y; y++){
		partindex = y*(rc.n_y*4);
		for(x = 0; x < rc.n_x; x++) {
			index = partindex + x*4; // index in char array computation (part 2)
			t = TreeTraverser(tree,rc.getRayForPixel(x,y));
			while((!t.isTerminated())){
				if(t.stack.size() >= (y / step)){

					VoxelData* data = tree->data;

					r=0.0f;
					g=0.0f;
					b=0.0f;
					for(int i = 0; i<rc.lights.size(); i++){
						if(rc.lights[i].active){
							to_light = rc.lights[i].position - t.getCurrentPosition();
							vec3 s = to_light + (rc.camera->e_ - t.getCurrentPosition());
							s = normalized(s);

							distancecut = 1.0f /(
								rc.lights[i].CONSTANT_ATTENUATION +
								rc.lights[i].LINEAR_ATTENUATION*len(to_light) +
								rc.lights[i].QUADRATIC_ATTENUATION*len2(to_light));

							to_light = normalized(to_light);

							// Diffuse
							diffuse_factor = data[t.getCurrentNode()->data].normal DOT to_light;
							r +=  std::max(0.0f,diffuse_factor) * distancecut * rc.lights[i].diffuse[0];
							g +=  std::max(0.0f,diffuse_factor) * distancecut * rc.lights[i].diffuse[1];
							b +=  std::max(0.0f,diffuse_factor) * distancecut * rc.lights[i].diffuse[2];

							// Phong
							float phong_factor = pow(s DOT data[t.getCurrentNode()->data].normal, rc.lights[i].SHININESS);
							r +=  std::max(0.0f,phong_factor) * distancecut * rc.lights[i].specular[0];
							g +=  std::max(0.0f,phong_factor) * distancecut * rc.lights[i].specular[1];
							b +=  std::max(0.0f,phong_factor) * distancecut * rc.lights[i].specular[2];

						}
					}
					texture_array[index] = (unsigned char) clampf(255*(r),0,255);
					texture_array[index+1] = (unsigned char) clampf (255*(g),0,255);
					texture_array[index+2] = (unsigned char) clampf (255*(b),0,255);
					texture_array[index+3] = (unsigned char) 1;

					/*DataPoint* data = tree->nonleafdata;
					if(t.getCurrentNode()->isLeaf()){
						data = tree->leafdata;
					}

					texture_array[index] = (unsigned char) int(data[t.getCurrentNode()->data].color[0] * 255.0f);
					texture_array[index+1] = (unsigned char) int(data[t.getCurrentNode()->data].color[1] * 255.0f);
					texture_array[index+2] = (unsigned char) int(data[t.getCurrentNode()->data].color[2] * 255.0f);
					texture_array[index+3] = (unsigned char) 1;*/


					break; // we stop here, break the while loop
				}
				t.step();
			}
		}
	}
}

TopLevelRenderer::~TopLevelRenderer(void)
{
}
