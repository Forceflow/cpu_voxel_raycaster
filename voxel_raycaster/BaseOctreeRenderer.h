#ifndef BASEOCTREERENDERER_H_
#define BASEOCTREERENDERER_H_
#include "Renderer.h"
#include "Octree.h"
#include "TreeTraverser.h"
#include "misc_math.h"

class BaseOctreeRenderer : public Renderer
{
public:
	BaseOctreeRenderer(void);
	void Render(RenderContext const& rc, Octree const* tree, unsigned char* texture_array) const;
	std::string getName() const;
	~BaseOctreeRenderer(void);
};

#endif /* BASEOCTREERENDERER_H_ */