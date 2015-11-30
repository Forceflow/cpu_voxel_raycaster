#ifndef WORKOCTREERENDERER_H_
#define WORKOCTREERENDERER_H_
#include "Renderer.h"
#include "Octree.h"
#include "TreeTraverser.h"
#include "misc_math.h"

class WorkOctreeRenderer : public Renderer
{
public:
	WorkOctreeRenderer(void);
	void Render(RenderContext const& rc, Octree const* tree, unsigned char* texture_array) const;
	~WorkOctreeRenderer(void);
};

#endif /* WORKOCTREERENDERER_H_ */