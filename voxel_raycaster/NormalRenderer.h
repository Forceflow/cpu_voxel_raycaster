#ifndef NORMALRENDERER_H_
#define NORMALRENDERER_H_
#include "Renderer.h"
#include "Octree.h"
#include "TreeTraverser.h"
#include "misc_math.h"

class NormalRenderer : public Renderer
{
public:
	NormalRenderer(void);
	void Render(RenderContext const& rc, Octree const* tree, unsigned char* texture_array) const;
	std::string getName() const;
	~NormalRenderer(void);
};

#endif /* NORMALRENDERER_H_ */