#ifndef DEPTHRENDERER_H_
#define DEPTHRENDERER_H_
#include "Renderer.h"
#include "Octree.h"
#include "TreeTraverser.h"
#include "misc_math.h"

class DepthRenderer : public Renderer
{
public:
	DepthRenderer(void);
	void Render(RenderContext const& rc, Octree const* tree, unsigned char* texture_array) const;
	std::string getName() const;
	~DepthRenderer(void);
};

#endif /* DEPTHRENDERER_H_ */