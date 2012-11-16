#ifndef DIFFUSEOCTREERENDERER_H_
#define DIFFUSEOCTREERENDERER_H_
#include "Renderer.h"
#include "Octree.h"
#include "TreeTraverser.h"
#include "misc_math.h"

class DiffuseOctreeRenderer : public Renderer
{
public:
	DiffuseOctreeRenderer(void);
	void Render(RenderContext const& rc, Octree const* tree, unsigned char* texture_array) const;
	std::string getName() const;
	~DiffuseOctreeRenderer(void);
};

#endif /* DIFFUSEOCTREERENDERER_H_ */