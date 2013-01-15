#ifndef RENDERERMANAGER_H_
#define RENDERERMANAGER_H_

#include <vector>
#include "Renderer.h"

class RendererManager
{
public:
	RendererManager();
	void addRenderer(Renderer* r);
	Renderer* getCurrentRenderer();
	void switchRenderer();

private:
	std::vector<Renderer*> renderers;
	int current_r;
};

inline RendererManager::RendererManager() : current_r(0){

}

inline Renderer* RendererManager::getCurrentRenderer(){
	return renderers.at(current_r);
}

inline void RendererManager::switchRenderer(){
	current_r = (current_r+1) % renderers.size();
}

inline void RendererManager::addRenderer(Renderer* r){
	renderers.push_back(r);
}

#endif