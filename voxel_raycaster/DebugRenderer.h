#ifndef DEBUGRENDERER_H_
#define DEBUGRENDERER_H_
#include "Renderer.h"
#include "Octree.h"
#include "misc_math.h"

class DebugRenderer : public Renderer
{
public:
	DebugRenderer(void);
	void Render(RenderContext const& rc,  Octree const* tree, unsigned char* texture_array) const;
	void RenderWork(RenderContext const& rc,  Octree const* tree, unsigned char* texture_array) const;
	std::string getName() const;
	~DebugRenderer(void);
};

#endif /* SIMPLEOCTREERENDERER_H_ */