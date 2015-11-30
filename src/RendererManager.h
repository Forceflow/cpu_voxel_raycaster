#ifndef RENDERERMANAGER_H_
#define RENDERERMANAGER_H_

#include <string>
#include <vector>
#include "Renderer.h"

class RendererManager
{
public:
	RendererManager();
	void addRenderer(Renderer* r);
	Renderer* getCurrentRenderer();
	Renderer* getRenderer(std::string name);
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

inline Renderer* RendererManager::getRenderer(std::string name){
	for(std::vector<Renderer*>::iterator it = renderers.begin() ; it != renderers.end(); ++it){
		if((*it)->name == name){
			return *it;
		}
	}
	return NULL;
}

#endif