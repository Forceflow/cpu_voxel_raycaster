#ifndef TOPLEVELRENDERER_H_
#define TOPLEVELRENDERER_H_
#include "Renderer.h"
#include "Octree.h"
#include "TreeTraverser.h"
#include "misc_math.h"

class TopLevelRenderer : public Renderer
{
public:

	int maxlevel;

	TopLevelRenderer();
	void Render(RenderContext const& rc, Octree const* tree, unsigned char* texture_array) const;
	std::string getName() const;
	~TopLevelRenderer(void);
};

#endif /* TOPLEVELRENDERER_H_ */