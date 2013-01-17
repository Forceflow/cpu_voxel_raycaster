#ifndef LEVELRENDERER_H_
#define LEVELRENDERER_H_
#include "Renderer.h"
#include "Octree.h"
#include "TreeTraverser.h"
#include "misc_math.h"

class LevelRenderer : public Renderer
{
public:

	int maxlevel;

	LevelRenderer();
	void Render(RenderContext const& rc, Octree const* tree, unsigned char* texture_array) const;
	std::string getName() const;
	~LevelRenderer(void);
};

#endif /* LEVELRENDERER_H_ */